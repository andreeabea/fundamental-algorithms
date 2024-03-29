/* Assignment No. 9: Breadth-First Search
 The BFS algorithm calls the BFSComponent function for each connected component in the graph. The graph is represented by 
 adjacency lists. The BFSComponent does push and pop operations, repeatedly, for a queue that stores the keys of the nodes 
 of the connected component when they are visited. The Breadth-First Search algorithm has a running time complexity of 
 O(V+E), where V is the number of vertices and E is the number of edges in the graph.
 In addition, for the demo, the resulting forest of trees from the bfs was printed. For this, in the BFSComponent I have also
 created a vector of parents, which in the BFS function was transformed in a multiway representation => the roots of all the 
 multiway trees (a multiway tree for each connected component). Then for each of these roots its corresponding multiway tree
 was printed.

 As said before, for a graph the running time of BFS depends on both V and E -> O(V+E). In the analysis, it was counted the
 number of operations performed by the BFS, which is actually the number of push and pop operations performed in the BFS. 
 Each of these operations (push and pop) has a complexity of O(1). In the BFS algorithm, there are performed 2*V such 
 operations because all the nodes are pushed in the queue and also popped from the queue, and also there are performed 2*E
 pop operations from the adjacency lists of the graph because all the nodes from the adjacency lists are popped and they appear
 twice because the graph is undirected. In the generated charts, first V is constant and E varies from 1000 to 4500, then E
 is constant and V varies between 100 and 200, and the analysis for each of these cases is performed 5 times. As it can be 
 seen from the charts, BFS runs in time linearly in the size of the adjacency list representation.

*/

#include "pch.h"
#include <iostream>
using namespace std;
#include "Profiler.h"

int op = 0;
Profiler profiler("BFS");

struct Node //list node
{
	Node *pNext;
	int key;
};

struct list
{
	Node *pFirst, *pLast;
	int length;
};

struct NodeMultiway //multiway tree node - only for the demo
{
	int key;
	int count_children;
	NodeMultiway* children[8];
};

void initList(list* l)
{
	if (l)
	{
		l->pFirst = NULL;
		l->pLast = NULL;
		l->length = 0;
	}
}

Node* createNode(int key)  //creates a node in the list
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode)
	{
		newNode->key = key;
		newNode->pNext = NULL;
	}
	return newNode;
}

void push(list* queue, int value) //pushes an element at the end of the list/queue
{
	Node* pNew = createNode(value);
	if (queue->length==0)
	{
		queue->pFirst = queue->pLast = pNew;
	}
	else
	{
		queue->pLast->pNext = pNew;
		queue->pLast = pNew;
	}
	op++;
	pNew->pNext = NULL;
	queue->length++;
}

int pop(list* queue) //pops the first element
{
	int x = -1;
	if (queue->pFirst != NULL)
	{
		x = queue->pFirst->key;
		op++;
		queue->pFirst = queue->pFirst->pNext;
		if (queue->pFirst == NULL)
		{
			queue->pLast = NULL;
		}
		queue->length--;
	}
	return x;
}

bool empty(list queue)
{
	if (queue.pFirst == NULL)
	{
		return true;
	}
	return false;
}

int getComponent(bool* v, int n) //returns the key of the first unvisited node
{
	for (int i = 1; i <= n; i++)
	{
		if (v[i] == false)
		{
			return i;
		}
	}
	return -1;
}

NodeMultiway* createNode2(int key)  //creates a node of the multiway tree 
{
	NodeMultiway* newNode = (NodeMultiway*)malloc(sizeof(NodeMultiway));
	if (newNode)
	{
		newNode->key = key;
		newNode->count_children = 0;
		for (int i = 0; i < 8; i++)  //at most 8 children
			newNode->children[i] = NULL;
	}
	return newNode;
}

NodeMultiway** transformMultiway(int parentArray[], int k, int n) //transforms a vector of parents to the multiway representation
{
	int i;
	//the roots in the forest
	NodeMultiway** root = (NodeMultiway**)malloc(k*sizeof(NodeMultiway*));
	for (int i = 0; i < k; i++)
		root[i] = NULL;

	NodeMultiway** nodesArray;
	nodesArray = (NodeMultiway**)malloc((n+1) * sizeof(NodeMultiway*));

	//initialize a nodes array with all the nodes
	for (i = 1; i <= n; i++)
	{
		nodesArray[i] = (NodeMultiway*)malloc(sizeof(NodeMultiway));
		nodesArray[i]->key = i;
		nodesArray[i]->count_children = 0;
		for (int j = 0; j < 8; j++) //at most 8 children (for the demo only)
			nodesArray[i]->children[j] = NULL;
	}

	int root_index = 0;
	for (i = 1; i <= n; i++)
	{
		if (parentArray[i] == -1) //one of the roots in the forest is found
		{
			root[root_index++] = nodesArray[i];
		}
		else
		{
			NodeMultiway* child = nodesArray[i]; //the child is the node in the nodes array
			NodeMultiway* parent = nodesArray[parentArray[i]];
			parent->children[parent->count_children++] = child; //link parent with child and increase the number of children of the parent
		}
	}
	return root; //returns the roots of the multiway forest
}

void printMultiwayTree(NodeMultiway* root, int space)
{
	if (root != NULL)
	{
		for (int i = 1; i <= space; i++)
			cout << " ";
		cout << root->key << endl;
		for (int j = 0; j < root->count_children; j++)
		{
			printMultiwayTree(root->children[j], space + 3);
		}
	}
}

void BFScomponent(list* adj, int vertex, bool* visited, int* parent)
{
	list Q;
	initList(&Q);
	push(&Q, vertex);
	visited[vertex] = true;
	parent[vertex] = -1;

	while (!empty(Q))
	{
		int curr_v = pop(&Q);

		while(!empty(adj[curr_v]))
		{
			int v = pop(&adj[curr_v]);
			if (visited[v] == false)
			{
				parent[v] = curr_v;
				visited[v] = true;
				push(&Q, v);
			}
		}
	}
}
void bfs(list* adj, int n, bool demo)
{
	bool* visited = (bool*)calloc(n+1, sizeof(bool));
	int* parent = (int*)calloc(n+1, sizeof(int*)); 
	
	//for each connected component
	int k = 0;
	int c = getComponent(visited, n);
	while (c != -1)
	{
		BFScomponent(adj, c, visited, parent);
		c = getComponent(visited, n);
		k++; //the number of connected components
	}

	//print the forest in the test
	if (demo == true)
	{
		NodeMultiway** roots = transformMultiway(parent, k, n); //the roots of the connected components' trees
		for (int i = 0; i < k; i++)
		{
			cout << "Tree " << i + 1 << ": " << endl;
			cout << endl;
			printMultiwayTree(roots[i], 0);
			cout << endl;
		}
	}
}

void addEdge(list* adj, int x, int y) //adds an edge in the undirected graph
{
	push(&adj[x], y);
	if (x != y)
	{
		push(&adj[y], x);
	}
}

int firstRand(int n, int k) //generates a random key for the first node of an edge
{
	float p_first;
	int delta = (1 - 2 * n)*(1 - 2 * n) - 8 * k;
	p_first = (2 * n - 1 - sqrt(delta)) / 2;
	int p = (int)p_first;

	if (p == p_first)
	{
		return p;
	}
	else
	{
		return p + 1;
	}
}

int secondRand(int n, int k, int first) //generates the second random key of the other node of the edge, based on the first
{
	int p = first - 1;
	int sum = p * n - p * (p + 1) / 2;
	return k - sum + first;
}

void printList(list list)
{
	Node* p = list.pFirst;
	while (p != NULL)
	{
		cout << p->key << " ";
		p = p->pNext;
	}
	cout << endl;
}

void test()
{
	list* adj = (list*)malloc(9 * sizeof(list));
	for (int i = 1; i <= 8; i++)
	{
		initList(&adj[i]);
	}
	addEdge(adj, 1, 2);
	addEdge(adj, 1, 3);
	addEdge(adj, 2, 3);
	addEdge(adj, 3, 4);
	addEdge(adj, 4, 4);
	addEdge(adj, 5, 6);
	addEdge(adj, 5, 8);

	for (int i = 1; i <= 8; i++)
	{
		cout << i << ": ";
		printList(adj[i]);
	}
	cout << endl;

    bfs(adj, 8, true);
}

list* generateGraph(int V, int E) //generate the adjacency lists of the graph
{
	list* adj = (list*)malloc((V + 1) * sizeof(list));
	for (int i = 1; i <= V; i++)
	{
		initList(&adj[i]);
	}

	int* k = (int*)malloc(E * sizeof(int));
	FillRandomArray(k, E, 1, V*(V - 1) / 2, true, 0);

	for (int i = 0; i < E; i++)
	{
		int x, y;
		x = firstRand(V, k[i]);
		y = secondRand(V, k[i], x);
		addEdge(adj, x, y);
	}
	return adj;
}

void runAnalysis()
{
	int V, E;
	V = 100;
	for (E = 1000; E <= 4500; E += 100)
	{
		op = 0;
		for (int k = 0; k < 5; k++)
		{
			list* adj = generateGraph(V, E);
			bfs(adj, V, false);
		}
		op /= 5;
		profiler.countOperation("variation_of_E", E, op);
	}

	E = 4500;
	for (V = 100; V <= 200; V += 10)
	{
		op = 0;
		for (int k = 0; k < 5; k++)
		{
			list* adj = generateGraph(V, E);
			bfs(adj, V, false);
		}
		op /= 5;
		profiler.countOperation("variation_of_V", V, op);
	}
}

int main()
{
	test();
	//runAnalysis();
	//profiler.showReport();

	return 0;
}

