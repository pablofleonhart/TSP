#include "algorithms.h"
#include <iostream>
#include <deque>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace std;

float done = 0;

tsp::tsp(const char * filename)
{
    num_cities = readFile(filename);
}

tsp::tsp(tsp & source)
{
    int size = source.original_list.size();
    for (int i = 0; i < size; ++i) {
        original_list.push_back(new city(*source.original_list[i]));
    }

    size = source.solution.size();

    for (int i = 0; i < size; ++i) {
        solution.push_back(new city(*source.solution[i]));
    }

    num_cities = source.num_cities;
}

tsp::~tsp()
{
    original_list.clear();
    solution.clear();
}

float tsp::readFile(const char * filename)
{
    float added = 0;
    float id_read = 0;
    float x_read = 0;
    float y_read = 0;

    original_list.clear();
    ifstream file( filename );

    if (!file)
    {
        cout << "error";
        return 0;
    }

    stringstream ss;
    ss << file.rdbuf();
    file.close();
    string line = "";

    while ( ss >> line && line != "NODE_COORD_SECTION" ){}

    while ( ss >> line && line != "EOF" )
    {
        id_read = strtof( ( line ).c_str(),0 );
        ss >> x_read >> y_read;
        original_list.push_back( new city( id_read, x_read, y_read, id_read, false ) );
        ++added;
    }
    
    return added;
}

float tsp::bruteForceWrapper()
{
    float distance = 0;
    float min_distance = 9999999;
    deque <city*> best_path;

    if (solution.empty())
        nearestNeighborBasic(0);

    bruteForce(best_path, min_distance, num_cities);
    copyCityDeque(best_path, solution);

    distance = getSolutionDistance();
    writeSolution("bruteforce.txt");
    return distance;
}

void tsp::bruteForce(deque <city*> & best_path, float & min_distance, float cities_left)
{
    float current_dist = 0;

    signal(SIGTERM, endOpt);
    for (float i = 0; !done && i < cities_left; ++i)
    {
        rotate(cities_left-1);
        current_dist = getSolutionDistance();
        if (current_dist < min_distance)
        {
            min_distance = current_dist;
            cout << min_distance << endl;
            copyCityDeque(solution, best_path);
        }

        bruteForce(best_path, min_distance, cities_left - 1);
    }

   return;
}

void tsp::rotate( int pos)
{
    solution.push_front(solution[pos]);
    solution.erase(solution.begin() + pos+1);

    return;
}

float tsp::nearestNeighbor()
{
    solution.clear();
    float total_dist = 0;
    float best_start_distance = 9999999;
    float last_run = 0;

    for (int i = 0; !done && i < num_cities; ++i)
    {
        nearestNeighborBasic(i);
        last_run = twoChange();
        if (last_run < best_start_distance)
        {
            best_start_distance = last_run;
            cout << best_start_distance << endl;
            break;
        }
    }

    total_dist = getSolutionDistance();
    if (best_start_distance <= total_dist)
        return best_start_distance;

    else
    {
        cout << "Writing solution " << total_dist << endl;
        writeSolution(OUTPUT_FN);
        return best_start_distance;
    }
}

float tsp::nearestNeighborBasic( int start_index)
{
    float cities_added = 0;
    float closest = 9999999;
    float total_dist = 0;
    float current_dist = 0;
    float closest_index = 0;
    float current_num = num_cities;
    deque <city*> temp;
    copyCityDeque(original_list, temp);

    solution.clear();
    solution.push_back(original_list[start_index]);
    original_list.erase(original_list.begin() + start_index);
    --current_num;
    ++cities_added;
    while(current_num != 0)
    {
        closest = 9999999;
        for (int i = 0; i < current_num; ++i)
        {
            current_dist = original_list[i]->dist(solution[cities_added-1]);
            if (current_dist < closest)
            {
                closest_index = i;
                closest = current_dist;
            }
        }

        total_dist += closest;
        solution.push_back(original_list[closest_index]);
        original_list.erase(original_list.begin() + closest_index);

        --current_num;
        ++cities_added;
        }

    copyCityDeque(temp, original_list); 
    return total_dist + solution[0]->dist(solution[cities_added-1]);
}

float tsp::twoChange()
{
    deque <city*> new_path;
    float min_distance = getSolutionDistance();
    bool start_over = false;

    signal(SIGTERM, endOpt);
    while(!done)
    {
        start_over = false;
        for (float i = 1; i < num_cities && !start_over; ++i)
        {
            for (int j = i+1; j < num_cities-1 && !start_over; ++j)
            {
                if (solution[i-1]->dist(solution[j]) + solution[i]->dist(solution[j+1]) < solution[i-1]->dist(solution[i]) + solution[j]->dist(solution[j+1]))
                {
                    swapTwo(i, j);
                    min_distance = getSolutionDistance();
                    start_over = true;
                }

                else
                    start_over = false;
            }
         }

         if (!start_over)
            break;
    }
    return min_distance;
}

float tsp::twoOpt()
{
    deque <city*> new_path;
    float min_distance = getSolutionDistance();
    int k = 0;

        for (int i = 1; i < num_cities ; ++i)
        {
            k = 1;
            fixPositions();
            while (k <= 5 && solution[i]->dist(solution[i]->getNeighbor(k)) < solution[i-1]->dist(solution[i]))
            {
                swapTwo(i, solution[i]->getNeighborPos(k));
                min_distance = getSolutionDistance();
                ++k;
                cout << min_distance << endl;
            }
            fixPositions();
        }


    return min_distance;
}

float tsp::swapTwo(int i, int k)
{
    deque <city*> temp;
    int count = 0;

    for (int x = k; x >= i; --x)
    {
        temp.push_back(solution[x]);
    }

    for (int x = i; x <= k; ++x)
    {
        solution[x] = temp[count];
        ++count;
    }
    temp.clear();
    return 1;
}

float tsp::getSolutionDistance()
{
    float total_dist = 0;
    for (int i = 0; i < num_cities - 1; ++i)
    {
        total_dist += solution[i]->dist(solution[i+1]);
    }

    total_dist += solution[0]->dist(solution[num_cities-1]);
    return total_dist;
}

void tsp::writeSolution(const char * file_name)
{
    float distance = getSolutionDistance();
    ofstream write(file_name);
    if (write.is_open())
    {
        write << distance << '\n';
    }

    for (int i = 0; i < num_cities; ++i)
        solution[i]->writeOut(write);

    write.close();

    return;
}

void tsp::displayNeighborLists()
{
    for (int i = 0; i < num_cities; ++i)
     {
        cout << "LIST " << i << endl;
        solution[i]->displayNeighborList();
    }

    return;
}

void tsp::fixPositions()
{
    for ( int i = 0; i < num_cities; ++i )
        solution[i]->setPos( i );
}

void copyCityDeque( deque <city*> & source, deque <city*> & dest )
{
    int length = source.size();
    dest.clear();
    for ( int i = 0; i < length; ++i )
        dest.push_back( new city( *source[i] ) );
}

void endOpt( int signum )
{
    cout << "\nOut of time\n";
    done = 1;
}