#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/System.hpp>

class Screen
{
    public:
        Screen(int x, int y);
        virtual ~Screen();
        void gatherPixels();
        sf::Uint8* getPixels();
    protected:
        int getOwnedRows(int rank);
    private:
        int m_comm_rank;
        int m_comm_size;
        sf::Uint8* m_data;
        int m_size;
        int m_yoffset;
        int m_x;
        int m_y;
};

#endif // SCREEN_H
