#include "screen.h"
#include "mpi.h"

Screen::Screen(int x, int y) : m_x(x), m_y(y)
{
    //ctor
    MPI_Comm_rank(MPI_COMM_WORLD,&m_comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&m_comm_size);
    if(x%m_comm_size>m_comm_rank) m_size=x/m_comm_size+1;
    else m_size=x/m_comm_size;
    if(m_comm_rank)
        m_data = new sf::Uint8[4*y*m_size];
    else
        m_data = new sf::Uint8[4*y*x];
    for(int i=0; i<m_size*m_y*4; i++)
    {
        m_data[i]=m_comm_rank*255;
    }
    /**
        Here goes object gathering
                                    **/
}

Screen::~Screen()
{
    //dtor
}
void Screen::gatherPixels()
{
    if(m_comm_rank)
        MPI_Send(m_data,m_size*4*m_y,MPI_BYTE,0,0,MPI_COMM_WORLD);
    if(!m_comm_rank)
    {
        MPI_Request reqs[m_comm_size-1];
        MPI_Status stats[m_comm_size-1];
        for(int i=1; i<m_comm_size; i++)
        {
            int td=4*m_y*getOwnedRows(0);
            MPI_Irecv(m_data+td,getOwnedRows(i)*4*m_y,MPI_BYTE,i,0,MPI_COMM_WORLD,&reqs[i-1]);
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
