// FILE: node.h 
// CLASS PROVIDED: node
//     Each node on a binarytree has the following features: 
//     letter: a character. If an internal node, the NULL character ('\0') is used.
//     probality: a double value showing the probability of occurance
//     left: a pointer to the left child
//     right: a pointer to the right child


#ifndef NODE_H
#define NODE_H

#include <iostream>	
using namespace std;

class node 
{
public:
	char letter;
	double probability;
	string code;
	node* left;
	node* right;

	node(char c, double p) 
	{
		letter = c;
		probability = p;
		left = right = NULL;
	}

	node(node* lptr, node* rptr) 
	{
		left = lptr;
		right = rptr;
		if ((lptr != NULL) && (rptr != NULL))
			probability = lptr->probability + rptr->probability;
	}
	
    bool is_leaf() const
    {
        if ((left == NULL) && (right == NULL)) return true;
        return false;
    }
};

// NON-MEMBER FUNCTION
// Define a function comparing the probability
// purpose: to sort the node list in the order of ascending probability
bool comp_prob(node* first, node* second)
{
	if ( first->probability < second->probability )
		return true;
	
	return false;
}

// Clean all nodes in the tree
void clean(node* n)
{
    if (n->is_leaf())
    {
        delete n;
        return;
    }
    
    clean(n->left);
    clean(n->right);
}

#endif
