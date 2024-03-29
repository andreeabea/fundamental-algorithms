/* Assignment No. 6: Dynamic Order Statistics
 The management operations of an order statistics tree are implemented in this assignment. Firstly, the build tree function
 constructs a balanced binary search tree with n nodes, the nodes having the values from 1 to n. The tree is also augmented,
 each node having a size field that holds the size of the sub-tree rooted at that node, besides the necessary fields. The 
 algorithm for building the tree uses a divide and conquer approach. The selection function selects the node having the ith 
 smallest key in the tree, similarly to the quick-select algorithm that returns the ith smallest element of an unordered array.
 The OS-delete algorithm deletes a node from the augmented bst, in almost the same way as from a normal bst, except that the
 size information from the nodes must be updated after each deletion.

 In the average case, n varies from 100 to 10000, for each n the measurements being repeated 5 times (building a tree with n
 nodes, then selecting and deleting each node by using a randomly selected index). As the charts also show, the complexity of 
 select and delete management operations is O(h) for each, where h is the height of the binary search tree (h=lgn because the 
 initial constructed tree is balanced), while building the tree has a running time performance of O(n), where n is the number 
 of nodes in the binary search tree. The build tree algorithm does the smallest number of operations, while the deletion does 
 the most operations (assignments and comparisons). The overall complexity is O(n*lgn).

*/
#include "pch.h"
#include <iostream>
#include "Profiler.h"
using namespace std;
#define MAX_SIZE 10000

Profiler profiler("Dynamic Order Statistics");
int opDelete = 0, opSelect = 0, opBuild = 0;

typedef struct node
{
	int value;
	struct node* left;
	struct node* right;
	int size;
}Node;

Node* createNode(int key)  //creates a node
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode)
	{
		newNode->value = key;
		newNode->left= NULL;
		newNode->right = NULL;
		newNode->size = 1;
	}
	return newNode;
}

Node* buildTree(int n, int left, int right) //builds a balanced BST containing the keys from 1 to n
{
	if (left <= right)
	{
		Node* pNode = createNode(left);
		int m = (left + right + 1) / 2;
		opBuild += 3;
		pNode->value = m;
		pNode->left = buildTree(n, left, m - 1);
		pNode->right = buildTree(n, m + 1, right);
		//initialize the size field of each node
		if (pNode->left != NULL && pNode->right != NULL)
			pNode->size = pNode->left->size + pNode->right->size + 1;
		else if (pNode->left != NULL)
			pNode->size = pNode->left->size + 1;
		else if (pNode->right != NULL)
			pNode->size = pNode->right->size + 1;

		return pNode;
	}
	else return NULL;
}

Node* OS_select(Node* root, int k) //selects the node with the ith smallest key from the tree
{
	if (root!=NULL)
	{
		int size;
		if(root->left!=NULL)
			size = root->left->size + 1;
		else size = 1;
		opSelect++;
		if (size==k)
			return root;
		if (root->left!=NULL && size > k)
			return OS_select(root->left, k);
		else if(root->right!=NULL)
			return OS_select(root->right, k - size);
	}
	return NULL;
}
Node* findMin(Node* root) //finds the node with the minimum key from a tree/subtree rooted at root
{
	while (root->left != NULL)
	{
		root = root->left;
		opDelete++;
	}
	return root;
}
Node* findParent(Node* root, Node* x) //finds the parent of a given node in the tree
{
	Node *p=NULL; //the parent of node x
	if (x == NULL)
		return p;
	while (root != NULL && x != root)
	{
		opDelete+=2;
		p = root;
		root->size--; //keeps the size information consistent, without increasing the complexity of the algorithm -for deleting
		if (x->value < root->value)
			root = root->left;
		else root = root->right;
	}
	return p;
}
Node* OS_delete(Node* root, Node* z) 
{
	if (root == NULL)
		return root;
	Node* deleted = NULL; //the node that is deleted
	Node* child = NULL; //its child

	if (z->left == NULL || z->right == NULL) //it has at most one child
		deleted = z;
	else deleted = findMin(z->right); //if it the node to be deleted has two children

	if (deleted->left != NULL)
		child = deleted->left; //deleted has no right child
	else child = deleted->right; //no left child, or no child at all
	
	//in findParent also the size of each node from the root to the deleted node is decremented (it doesn't increase complexity)
	Node* parent = findParent(root, deleted); //y's parent
	Node* copy = deleted; // a copy of y (=x's parent)

	if (child != NULL) //is not a leaf
		copy = parent; //x's parent becomes y's parent

	if (parent == NULL) //if deleted was the root
		root = child;
	else if (deleted == parent->left) //link deleted's parent to its child
		parent->left = child;
	else parent->right = child;

	opDelete++;
	z->value = deleted->value; //copy the deleted node's key (value) to z
	free(deleted);

	return root;
}

void printTree(Node *root, int space)
{
	if (root == NULL)
		return;
	space += 3;
	printTree(root->right, space);
	cout << endl;
	for (int i = 1; i < space; i++)
		cout << "   ";
	cout << root->value << "/" << root->size;
	printTree(root->left, space);
}

void test()
{
	int n = 11;
	Node* root = buildTree(n, 1, n);
	cout << "the initial tree: \n";
	printTree(root, 0);
	for (int i = 0; i < 3; i++)
	{
		int r[1];
		if (n == 1)
			r[0] = 1;
		else FillRandomArray(r, 1, 1, n, true, 0);
		Node* x = OS_select(root, r[0]);
		cout <<"\n\n" << "the tree after deleting node " << x->value << " : \n";
		root = OS_delete(root, x);
		n--;
		printTree(root, 0);
	}
}

void runAverageCase()
{
	for (int n = 100; n <= MAX_SIZE; n+=100)
	{
		opSelect = 0;
		opDelete = 0;
		opBuild = 0;
		for (int i = 0; i < 5; i++)
		{
			Node* root = buildTree(n, 1, n);
			int nodes = n;
			for (int j = 0; j < n; j++)
			{
				int r[1];
				if (nodes == 1)
					r[0] = 1;
				else FillRandomArray(r, 1, 1, nodes, true, 0);
				Node* x = OS_select(root, r[0]);
				root = OS_delete(root, x);
				nodes--;
			}
		}
		opSelect /= 5;
		opDelete /= 5;
		opBuild /= 5;
		profiler.countOperation("operations-select", n, opSelect);
		profiler.countOperation("operations-delete", n, opDelete);
		profiler.countOperation("operations-build", n, opBuild);
		profiler.countOperation("operations", n, opSelect + opDelete + opBuild);
	}
	
	profiler.showReport();
}
int main()
{
	test();
	//runAverageCase();
	return 0;
}

