/* Assignment No. 7: Multi-way Trees
Transformations between different representations
 Linear time transformations were implemented in this assignment, firstly the transformation from the parents' vector representation
 to a multiway tree, then the transformation from the multiway tree representation to a binary tree representation. The first 
 representation (vector of parents) is given, while for the others I used 2 different node structures. The first node structure (for 
 the multi-way tree) besides its value field, it also has a field with the number of children and a field with a pointer to the vector 
 containing the children. The second node structure contains the usual fields for a binary tree node data structure, where the left 
 node pointer represents actually the first child of a node in the multi-way representation and the right node pointer represents the 
 right sibling.

 The algorithm that transforms the parent array to a multi-way tree, firstly creates an array of nodes (n nodes => O(n) complexity),
 then it traverses the array of nodes in order to find the root (based on the parent array too) and also to create the links between 
 the parents and their children ( O(n) running time complexity again). Therefore, the complexity of this algorithm is O(n)+O(n) which 
 is O(n).
 The algorithm that transforms a multi-way tree representation to a binary tree representation contains a single call of the 
 add_children function (which is called only if the multi-way tree's root is not null). The add_children function adds recursively all 
 the nodes in the binary tree, starting from the root, firstly taking the nodes in the left branch, then adding the right child nodes.
 Adding each node takes O(1), therefore adding all of the multi-way nodes to the binary tree takes O(n) operations => linear.
 So, the transformation from a parent vector to a binary tree representation has a complexity of O(n) ( O(n) + O(n) ).

 Additional memory was used for these transformations, for the first transform O(n) (when creating the nodes array which results in a
 n-node multi-way tree) and for the second transform also O(n) (when creating a binary tree with n nodes).

*/
#include "pch.h"
#include <iostream>
using namespace std;

struct NodeV2 //multiway tree node
{
	int value;
	int count_children;
	NodeV2** children;
};
struct NodeV3 //binary tree node
{
	int value;
	NodeV3* left_child;
	NodeV3* right_sibling;
};

NodeV2* transformMultiway(int parentArray[], int n)
{
	int i;
	NodeV2* root = NULL;
	NodeV2** nodesArray;
	nodesArray = (NodeV2**)malloc(n*sizeof(NodeV2*));
	//initialize a nodes array with all the nodes
	for (i = 0; i < n; i++) 
	{
		nodesArray[i] = (NodeV2*)malloc(sizeof(NodeV2));
		nodesArray[i]->value = i+1;
		nodesArray[i]->count_children = 0;
		nodesArray[i]->children = (NodeV2**)malloc(n*sizeof(NodeV2*));
		for (int j = 0; j < n; j++)
			nodesArray[i]->children[j] = NULL;
	}
	
	for (i = 0; i < n; i++)
	{
		if (parentArray[i] == -1) //the root is found
		{
			root = nodesArray[i];
		}
		else
		{
			NodeV2* child = nodesArray[i]; //the child is the node in the nodes array
			NodeV2* parent = nodesArray[parentArray[i]-1];
			parent->children[parent->count_children++] = child; //link parent with child and increase the number of children of the parent
		}
	}
	return root; //returns the root of the multiway tree
}

NodeV3* createNode(int key) //create a node in a binary tree
{
	NodeV3* p = (NodeV3*)malloc(sizeof(NodeV3));
	p->value = key;
	p->left_child = NULL;
	p->right_sibling = NULL;
	return p;
}

void add_children(NodeV2* v2, NodeV3* v3) //adds the nodes recursively
{
	if (v2->children[0]!=NULL) //adds the nodes on the left
	{
		v3->left_child = createNode(v2->children[0]->value);
		v3 = v3->left_child;
		add_children(v2->children[0], v3);
	}
	for (int i = 1; i < v2->count_children; i++) //the rest of the children on the right branch
	{
		if (v2->children[i] != NULL)
		{
			v3->right_sibling = createNode(v2->children[i]->value);
			v3 = v3->right_sibling;
			add_children(v2->children[i], v3);
		}
	}
}

NodeV3* transformBinary(NodeV2* root) //transforms a multiway tree to a binary tree
{
	if (root == NULL)
		return NULL;
	else
	{
		NodeV3* current = createNode(root->value); //first, create the root
		add_children(root, current); //add the rest of the nodes

		return current;
	}
}

void printMultiway(NodeV2* root, int space)
{
	if (root != NULL)
	{
		for (int i = 1; i <= space; i++)
			cout << " ";
		cout << root->value << endl;
		for (int j = 0; j < root->count_children; j++)
		{
			printMultiway(root->children[j], space + 3);
		}
	}
}

void printBinaryTree(NodeV3 *root, int space)
{
	if (root == NULL)
		return;
	space += 3;
	printBinaryTree(root->right_sibling, space);
	cout << endl;
	for (int i = 1; i < space; i++)
		cout << " ";
	cout << root->value;
	printBinaryTree(root->left_child, space);
}

int main()
{
	int parents[] = {2,7,5,2,7,7,-1,5,2}, n=9;
	cout << "Parent vector: " << endl;
	for (int i = 0; i < 9; i++)
		cout << parents[i] << " ";
	cout << endl;
	cout << endl << "Multi-way representation: " << endl;
	cout << endl;
	NodeV2* root=transformMultiway(parents, n);
	printMultiway(root, 0);
	cout << endl;
	NodeV3* root2 = transformBinary(root);
	cout << "Binary representation: " << endl;
	printBinaryTree(root2, 0);
	return 0;
}


