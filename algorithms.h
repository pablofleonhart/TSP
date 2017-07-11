//algorithms.h - Prototypes for tsp class, which contains algorithms used
//to solve the traveling salesman problem.
//Written by Sam Best for CS325
//Last modified 3/12/2013

#ifndef _algorithms_h
#define _algorithms_h

#include <deque>
#include <fstream>
#include "city.h"
#include <vector>
#include <signal.h>

using namespace std;

const char OUTPUT_FN[] = "mysolution.txt";

class tsp
{
    public:
        tsp( const char * filename );
        tsp( tsp & source );
        ~tsp();
        float readFile( const char * filename );
        void writeSolution( const char * file_name );
        float bruteForceWrapper();
        float nearestNeighbor();
        float nearestNeighborBasic( int start_index );
        float twoChange();
        float twoOpt();
        float swapTwo( int i, int k );
        float getSolutionDistance();
        void displayNeighborLists();
        void fixPositions();
        void rotate( int pos );
    private:
        void bruteForce( deque <city*> & best_path, float & min_distance, float cities_left );
        deque <city*> original_list;
        deque <city*> solution;
        int num_cities;
};

void copyCityDeque( deque <city*> & source, deque <city*> & dest );
void endOpt( int signum );
#endif