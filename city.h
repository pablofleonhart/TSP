#ifndef _city_h
#define _city_h

#include "math.h"
#include <fstream>
#include <vector>
#include <deque>


using namespace std;

class city
{
    public:
        city(int idin, int xin, int yin, int posin, bool visitedin);
        city(city & source);
        ~city();
        int dist( city * city_in );
        int getX();
        int getY();
        int getId();
        int getPos();
        int getNlSize();
        city *& getNeighbor(int index);
        int getNeighborPos(int index);
        void setPos(int pos_in);
        void displayCoords();
        void outputId(ostream & output);
        int writeOut(ofstream & write);
        int copyCity(city * city_in);
        int compare(const city * city_in);
        void buildNeighborList(deque <city*> & cities, int num_cities);
        void pushToList(city *& to_add);
        bool nlIsEmpty();
        void displayNeighborList();
    private:
        int id;
        int x;
        int y;
        int pos;
        bool visited;
        deque <city*> neighbor_list;
};

#endif