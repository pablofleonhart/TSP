#include "tree.h"
#include <iostream>
using namespace std;

treeNode::treeNode() : tree_city(NULL), left(NULL), right(NULL) {}

treeNode *& treeNode::getLeft()
{
    return left;
}

treeNode *& treeNode::getRight()
{
    return right;
}

int treeNode::setCity(city *& city_in)
{
    if (!city_in)
        return 0;
        
    tree_city = city_in;
    return 1;
}

city *& treeNode::getCity()
{
    return tree_city;
}

tree::tree() : owner_city(NULL), root(NULL) {}

tree::tree(city * city_in) : owner_city(city_in), root(NULL) {}

tree::~tree()
{
    clearTree();
}

tree::tree(tree & source)
{
    copy(source.root, root);
}

void tree::copy(treeNode * source, treeNode *& dest)
{
    if (!source)
        dest = NULL;
        
    else
    {
        dest = new treeNode;
        dest->setCity(source->getCity());
        copy(source->getLeft(), dest->getLeft());
        copy(source->getRight(), dest->getRight());
    }
}

void tree::addToTree(city *& to_add)
{
    insert(root, to_add);
}

void tree::insert(treeNode *& root, city *& to_add)
{
    if (!root)
    {
        root = new treeNode;
        root->setCity(to_add);
        root->getLeft() = root->getRight() = NULL;
    }
    
    else if (root->getCity()->dist(owner_city) < owner_city->dist(to_add))
        insert(root->getLeft(), to_add);
        
    else
        insert(root->getRight(), to_add);
}

void tree::displayDetailedTree()
{
    traverseAndDisplay(root);
}

void tree::traverseAndDisplay(treeNode * root)
{
    if (!root)
        return;
    else
    {
        traverseAndDisplay(root->getRight());
        cout << root->getCity()->dist(owner_city) << endl;
        traverseAndDisplay(root->getLeft());
    }
}

void tree::traverseAndAdd(treeNode * source, tree & dest)
{
    if (!source)
        return;
        
    else
    {
        traverseAndAdd(source->getLeft(), dest);
        traverseAndAdd(source->getRight(), dest);
        dest.addToTree(source->getCity());
    }
}

void tree::clearTree()
{
    removeAll(root);
}

void tree::removeAll(treeNode *& root)
{
    if (!root)
        return;
        
    removeAll(root->getLeft());
    removeAll(root->getRight());
    
    delete root;
    root = NULL;
}

bool tree::isEqual(treeNode * root_one, treeNode * root_two)
{   
    if ((!root_one && root_two) || (root_one && !root_two))
        return false;
        
    else if (!root_one && !root_two)
        return true;
        
    return (!root_one->getCity()->compare(root_two->getCity())
        && isEqual(root_one->getLeft(), root_two->getLeft())
        && isEqual(root_one->getRight(), root_two->getRight()));
}

void tree::buildNeighborList(deque <city*> & neighbor_list, int size)
{
    traverseAndBuild(root, neighbor_list, size);
}

void tree::traverseAndBuild(treeNode * root, deque <city*> & neighbor_list, int size)
{
    if (!root)
        return;
           
    else
    {
        traverseAndBuild(root->getRight(), neighbor_list, size-1);
        neighbor_list.push_back(root->getCity());
        traverseAndBuild(root->getLeft(), neighbor_list, size-1);
    }
}

tree & tree::operator += ( city * to_add)
{
    addToTree(to_add);
    return *this;
}

tree & tree::operator += ( tree & to_add)
{
    tree temp;
    if (this == &to_add)
    {
        temp = to_add;
        traverseAndAdd(temp.root, *this);
        return *this;
    }
    
    traverseAndAdd(to_add.root, *this);
    return *this;
}

tree & tree::operator = ( tree & source)
{
    if (this == &source)
        return *this;
        
    if (root)
        clearTree();
        
    copy(source.root, root);
    return *this;
}

tree tree::operator + ( city * to_add)
{
    tree temp;
    copy(root, temp.root);
    temp.addToTree(to_add);
    return temp;
}

tree tree::operator + ( tree & tree_in)
{
    tree temp = tree_in;
    traverseAndAdd(root, temp);
    return temp;
}

bool tree::operator == ( tree & tree_in)
{
    if (this == &tree_in)
        return true;
        
    else if (!root && !tree_in.root)
        return true;
    
    else
        return isEqual(root, tree_in.root);
}

bool tree::operator != ( tree & tree_in)
{
    if (this == &tree_in)
        return false;
        
    else if (!root && !tree_in.root)
        return false;
    
    else
        return !isEqual(root, tree_in.root);
}

tree operator + ( city * item_in,  tree & tree_in)
{
    tree temp = tree_in;
    temp.addToTree(item_in);
    return temp;
}

ostream & operator << (ostream & output,  tree & source)
{
    traverseAndOutput(output, source.root);
    return output;
}

void traverseAndOutput(ostream & output, treeNode * root)
{
        if (!root)
            return;
            
        traverseAndOutput(output, root->getLeft());
        root->getCity()->outputId(output);
        output << endl;
        traverseAndOutput(output, root->getRight());
}    