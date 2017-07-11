#include "city.h"
#include "tree.h"
#include <iostream>
#include <fstream>
using namespace std;

const int LIST_SIZE = 5;

city::city(int idin, int xin, int yin, int posin, bool visitedin) : id(idin), x(xin), y(yin), pos(posin), visited(visitedin) {}

city::city(city & source)
{
    x = source.x;
    y = source.y;
    pos = source.pos;
    id = source.id;
    visited = source.visited;
    neighbor_list = source.neighbor_list;
}

city::~city()
{
}

int city::dist(city * city_in)
{
    float x2 = city_in->getX();
    float y2 = city_in->getY();
    
    x2 -= x;
    y2 -= y;
    
    x2 *= x2;
    y2 *= y2;
    
    return floor(sqrt(x2 + y2) + 0.5);
}

int city::getX()
{
    return x;
}

int city::getY()
{
    return y;
}

int city::getId()
{
    return id;
}

int city::getPos()
{
    return pos;
}

city *& city::getNeighbor(int index)
{
    return neighbor_list[index];
}

int city::getNlSize()
{
    return LIST_SIZE;
}

void city::setPos(int pos_in)
{
    pos = pos_in;
}

void city::displayCoords()
{
    cout << id << " " << x << " " << y;
    return;
}

void city::outputId(ostream & output)
{
    output << id;
}

int city::writeOut(ofstream & write)
{
    if (write.is_open())
    {
        write << id << '\n';
        return 1;
    }
    
    return 0;
}

int city::copyCity(city * city_in)
{
    if (city_in == NULL)
        return 0;
        
    id = city_in->id;
    x = city_in->x;
    y = city_in->y;
    pos = city_in->pos;
    visited = city_in->visited;
    
    neighbor_list = city_in->neighbor_list;
    return 1;
}

int city::compare(const city * city_in)
{
    if (city_in->id == id && city_in->x == x && city_in->y == y)
        return 1;
        
    else
        return 0;
}

void city::buildNeighborList(deque <city*> & cities, int num_cities)
{
    tree sorted(this);
    neighbor_list.clear();
    
    for (int i = 0; i < num_cities; ++i)
        sorted += cities[i];
    
    if (LIST_SIZE < num_cities)
        sorted.buildNeighborList(neighbor_list, LIST_SIZE);       
        
    else
        sorted.buildNeighborList(neighbor_list, num_cities);
    
    cout << pos << " NEIGHBOR LIST SIZE: " << neighbor_list.size() << endl;
    sorted.clearTree();

    return;
}

bool city::nlIsEmpty()
{
    return neighbor_list.empty();
}

void city::pushToList(city *& to_add)
{
    neighbor_list.push_back(new city(*to_add));
}

void city::displayNeighborList()
{
    int size = neighbor_list.size();
    
    for (int i = 0; i < size; ++i)
    {
        cout << dist(neighbor_list[i]);
        cout << "actual index: " << i;
        cout << " stored pos: " << neighbor_list[i]->getPos();
        cout << endl;
    }
    
    return;
}

int city::getNeighborPos(int index)
{
    return neighbor_list[index]->pos;
}