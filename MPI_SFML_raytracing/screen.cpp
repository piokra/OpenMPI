#include "screen.h"
#include "mpi.h"
#include <iostream>
#include "geometry.h"
#include "sphere.h"
#include <math.h>
#include <fstream>
Screen::Screen(int x, int y) : m_x(x), m_y(y)
{
    //ctor
    std::fstream file;
    MPI_Comm_rank(MPI_COMM_WORLD,&m_comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&m_comm_size);
    m_size=getOwnedRows(m_comm_rank);
    std::cout << m_size << std::endl;
    if(m_comm_rank)
        m_data = new sf::Uint8[4*y*m_size];
    else
        m_data = new sf::Uint8[4*y*x];
    for(int i=0; i<m_size*y; i++)
    {
        m_data[i*4+3]=255;
        m_data[i*4+0]=0;
        m_data[i*4+1]=0;
        m_data[i*4+2]=0;
    }
    m_yoffset=0;
    int i;
    for(i=0; i<m_comm_rank; i++)
        m_yoffset+=getOwnedRows(i);
    /**
        Here goes object gathering
                                    **/

    /**
            Rank 0 load objects
                                    **/

    /**
            Broadcast the number of objects
                                                **/

    int count,type;
    Sphere* sphere;
    Point3d pt(500,500,2500);
    if(!m_comm_rank)
    {
        file.open("input.dat2");
        file >> count;
    }
    MPI_Bcast(&count,1,MPI_INT,0,MPI_COMM_WORLD);


    /**
            Broadcast the item type and load it
                                                    **/
    for(int i=0; i<count; i++)
    {
        Object* obj;
        file >> type;

        MPI_Bcast(&type,1,MPI_INT,0,MPI_COMM_WORLD);

        switch(type)
        {
        case 0: //case Sphere
            obj=new Sphere(pt,0);
            if(!m_comm_rank)
                obj->readObject(file);
            obj->broadcastObject();
            m_objects.push_back(obj);
            break;
        case 1: //case Triangle

            break;
        default:
            std::cout << "INVALID OBJECT TYPE\n" << std::endl;
        }
    }
    if(!m_comm_rank)
        file.close();
}

Screen::~Screen()
{
    //dtor
}
void Screen::gatherPixels()
{
    if(m_comm_rank)
        MPI_Send(m_data,getOwnedRows(m_comm_rank)*4*m_y,MPI_BYTE,0,0,MPI_COMM_WORLD);
    if(!m_comm_rank)
    {
        MPI_Request reqs[m_comm_size-1];
        MPI_Status stats[m_comm_size-1];
        int td=4*m_y*getOwnedRows(0);
        for(int i=1; i<m_comm_size; i++)
        {

            MPI_Irecv(m_data+td,getOwnedRows(i)*4*m_y,MPI_BYTE,i,0,MPI_COMM_WORLD,&reqs[i-1]);
            td+=getOwnedRows(i)*4*m_y;
        }
        MPI_Waitall(m_comm_size-1,reqs,stats);
    }
}

int Screen::getOwnedRows(int rank)
{
    if(rank<m_x%m_comm_size)
        return m_x/m_comm_size+1;
    return m_x/m_comm_size;
}
sf::Uint8* Screen::getPixels()
{
    return m_data;
}
void Screen::countPixels()
{
    Point3d pt(500,500,2500);
    Point3d pt2;
    Sphere sphere(pt,1000);
    for(int i=0; i<getOwnedRows(m_comm_rank); i++)
    {
        for(int j=0; j<m_x; j++)
        {
            //construct rays
            pt = Point3d(500,500,-500);
            pt2= Point3d(j,i+m_yoffset,0);
            pt2 = pt2-pt;
            ParamLine pl;
            pl.origin=pt;
            pl.vector=pt2;
            pl.normalize();
            sf::Color ref = countSinglePixel(j,i,1);
            m_data[4*(j+i*m_x)]=ref.r;
            m_data[4*(j+i*m_x)+1]=ref.g;
            m_data[4*(j+i*m_x)+2]=ref.b;
            m_data[4*(j+i*m_x)+3]=255;

        }
    }

}
Vector3d Screen::reflectionVector(const Vector3d& normal, const Vector3d& ray)
{
    const double cosI = -normal.dotProduct(ray);
    return ray + normal*(cosI*2);
}

Vector3d Screen::refractionVector(const Vector3d& normal, const Vector3d& ray, double n1, double n2)
{
    const double n = n1/n2;
    const double cosI = -normal.dotProduct(ray);
    const double sinT2 = n * n * (1.0 - cosI*cosI);
    if(sinT2> 1.0) return Vector3d();
    const double cosT = sqrt(1-sinT2);
    return ray*n+normal*(n*cosI-cosT);
}

double Screen::reflectance(const Vector3d& normal, const Vector3d& ray, double n1, double n2)
{
    const double n = n1/n2;
    const double cosI = -normal.dotProduct(ray);
    const double sinT2 = n * n * (1.0 - cosI*cosI);
    if(sinT2 > 1.0) return 1.0;
    const double cosT = sqrt(1.0-sinT2);
    const double rOrth = (n1*cosI-n2*cosT)/(n1*cosI+n2*cosT);
    const double rPar = (n2*cosI-n1*cosT)/(n2*cosI+n1*cosT);
    return (rOrth*rOrth+rPar*rPar)/2.0;
}
sf::Color Screen::countSinglePixel(int x, int y, int samples)
{
    const double dx=1/samples;
    const double dy=1/samples;
    const double sc = samples*samples;
    double tr=0, tg=0, tb=0;
    Vector3d clr;
    for(int i=0; i<samples; i++)
    {
        for(int j=0; j<samples; j++)
        {
            //construct rays here
            Point3d pt,pt2;
            pt = Point3d(500,500,-500);
            pt2= Point3d(x+i*dx,y+m_yoffset+j*dy,0);
            pt2 = pt2-pt;
            ParamLine pl;
            pl.origin=pt;
            pl.vector=pt2;
            pl.normalize();
            clr=countSingleRay(pl,1.,1.,0);
            tr+=clr.x;
            tg+=clr.y;
            tb+=clr.z;




        }
    }
    return sf::Color((tr/sc*255.),(tg/sc*255.),(tb/sc*255.));
}

Vector3d Screen::countSingleRay(ParamLine ray, double impact,double n, int depth)
{
    if(depth>MAXDEPTH||impact<0.0001) return Vector3d(0,0,0);

    double t=INFINITY,t2;
    Object* obj = 0;
    Vector3d ct,cr,cc;
    for(int k=0; k<m_objects.size(); k++)
    {
        t2=m_objects.at(k)->getCollision(ray);
        if(!isnan(t2))
        {
            if(t2>0.1&&t2<t)
            {
                t=t2;
                obj=m_objects.at(k);
            }

        }
    }
    if(!isnan(t))
        if(t>0.1&&t<INFINITY)
        {

            cc=obj->getProperties().getColor();
            Vector3d normal = obj->getNormal(ray.getPoint(t));
            if(normal.dotProduct(ray.vector)>0) normal=normal*-1;
            const double reflect = reflectance(normal,ray.vector,n,obj->getProperties().getRefractiveIndex());
            const double tr = obj->getProperties().getColorImpact();
            const double ti = 1-reflect;
            ParamLine pl;
            pl.origin=ray.getPoint(t);
            pl.vector=reflectionVector(normal,ray.vector);
            pl.origin=pl.getPoint(0.1);
            cr = countSingleRay(pl,impact*reflect*(1-tr),n,depth+1);
            double newn = obj->getProperties().getRefractiveIndex();
            if(abs(newn-n)<0.01) newn=1;
            pl.vector=refractionVector(normal,ray.vector,n,newn);
            pl.origin=ray.getPoint(t);
            pl.origin=ray.getPoint(0.1);
            ct = countSingleRay(pl,impact*(1-reflect)*(1-tr),newn,depth+1);
            return cc*tr+cr*(1-tr)*reflect+ct*(1-tr)*ti;


        }
    return Vector3d(0,0,0);

}

