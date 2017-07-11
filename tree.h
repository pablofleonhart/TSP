#ifndef _tree_h
#define _tree_h

#include "city.h"
using namespace std;

class treeNode
{
    public:
        treeNode();
        treeNode *& getLeft();
        treeNode *& getRight();
        int setCity(city *& city_in);
        city *& getCity();
    protected:
        city * tree_city;
        treeNode * left;
        treeNode * right;
};

class tree
{
    public:
        tree();
        tree(city * city_in);
        ~tree();
        tree(tree & source);
        void addToTree(city *& to_add);
        void displayDetailedTree();
        void clearTree();
        void buildNeighborList(deque <city*> & neighbor_list, int size);
        bool operator == (tree & tree_in);
        bool operator != (tree & tree_in);
        tree & operator += (city * to_add);
        tree & operator += (tree & to_add);
        tree & operator = (tree & source);
        tree operator + (city * to_add);
        tree operator + (tree & tree_in);
        friend tree operator + (city * city_in,  tree & tree_in);
        friend ostream & operator << (ostream & output,  tree & source);
    private:
        city * owner_city;
        treeNode * root;
        void copy(treeNode * source, treeNode *& dest);
        void insert(treeNode *& root, city *& to_add);
        void traverseAndDisplay(treeNode * root);
        void traverseAndAdd(treeNode * source, tree & dest);
        void removeAll(treeNode *& root);
        void traverseAndBuild(treeNode * root, deque <city*> & neighbor_list, int size);
        bool isEqual(treeNode * root_one = NULL, treeNode * root_two = NULL);
};

void traverseAndOutput(ostream & output, treeNode * root);
#endif