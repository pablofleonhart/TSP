#include "city.h"
#include "algorithms.h"
#include <iostream>
#include <deque>
#include <cstdlib>
#include <chrono>
#include <sys/time.h>
#include <sys/resource.h>

using namespace std;

int main( int argc, char * argv[] )
{
    srand( time( 0 ) );  
    
    tsp test(argv[1]);              //read in command line input
    auto start = std::chrono::high_resolution_clock::now();
    test.nearest_neighbor();        //run nearest neighbor function
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
	cout << std::chrono::duration_cast<std::chrono::milliseconds>( elapsed ).count() << endl;
    return 0;
}