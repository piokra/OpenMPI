#include <SFML/Graphics.hpp>
#include "mpi.h"
#include <iostream>
#include "screen.h"
#include "geometry.h"
#include "sphere.h"
#include <sstream>

/**
    ALL INDEXES ARE AS FOLLOW (y,x)
    0,1 0,2 0,3 ... 0,n
    1,0 1,1 ...
    2,0 ...
    .
    .
    .

            **/
std::string tostr(double p, double q)
{
    std::string res;
    std::stringstream ss;
    if(q==0)
    {
        res="NAN";
        return res;
    }

    ss << p/q << std::endl;
    ss >> res;
    return res;
}

int main(int argc, char* argv[])
{
    Point3d pt(0,0,0);
    Sphere sphere(pt,1);
    ParamLine pl;
    Point3d pt2(-3,0,0);
    Point3d vec(-1,0.1,0);

    pl.origin=pt2;
    pl.vector=vec;
    pl.normalize();
    std::cout << sphere.getCollision(pl) << std::endl;
    int code=0;
    int rank=0, p;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    // Create the main window
    //sf::Uint8 data[1000*1000*4];
    Screen screen = Screen(640,640);
    screen.countPixels();
    sf::Uint8* data = screen.getPixels();
    screen.gatherPixels();

    if(!rank)
    {


        sf::RenderWindow App(sf::VideoMode(640, 480), "SFML window");

        // Load a sprite to display
        sf::Image Image;
        sf::Text str;
        str.setString(tostr(1000,1));
        str.setPosition(100,100);
        //str.SetColor(sf::Color(0,0,0));



        sf::Texture texture;
        texture.create(640,640);
        texture.update(data);
        sf::Sprite sprite;
        sprite.setColor(sf::Color(255,255,255));
        sprite.setTexture(texture);


        double ts, te;
        // Start the game loop
        while (App.isOpen())
        {
            ts=MPI_Wtime();
            screen.countPixels();
            screen.gatherPixels();

            // Process events

            sf::Event Event;

            while (App.pollEvent(Event))
            {
                // Close window : exit
                if (Event.type == sf::Event::Closed)
                {
                    code=1;
                    App.close();
                    MPI_Bcast(&code,1,MPI_INT,0,MPI_COMM_WORLD);
                }

            }

            // Clear screen
            App.clear();

            // Draw the sprite
            App.draw(sprite);
            App.draw(str);
            // Update the window
            MPI_Bcast(&code,1,MPI_INT,0,MPI_COMM_WORLD);
            App.display();
            te=MPI_Wtime();
            str.setString(tostr(1,te-ts));
        }
    }
    else
    {
        while(!code)
        {
            screen.countPixels();
            screen.gatherPixels();
            MPI_Bcast(&code,1,MPI_INT,0,MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return EXIT_SUCCESS;

}
