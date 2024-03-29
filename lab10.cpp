/* Assignment No. 10: Depth­-first search

 The DFS algorithm calls the DFSComponent function for each unvisited vertex, and then in the DFSComponent, recursively,
 each vertex is visited in depth. The graph is represented by adjacency lists and each time a vertex is reached in the
 adjacency list, it is popped. The Breadth-First Search algorithm has a running time complexity of O(V+E), where V is 
 the number of vertices and E is the number of edges in the graph. For the DFS, an undirected graph is used. As it can
 also be seen in the charts of the evaluation, DFS runs in time linearly in the size of the adjacency list representation.

 Also, Tarjan's algorithm and a Topological Sort algorithm were implemented. Tarjan's algorithm was used for determining
 the strongly connected components in a directed graph, while the topological sort was used for sorting topologically
 the nodes in a DAG (Directed Acyclic Graph). Both these algorithms use a Depth-First Search, so the complexity of each
 is O(V+E). Tarjan's algorithm finds the head of each DFS subtree in the graph, and then it prints all the nodes in that 
 subtree which is actually the strongly connected component. For finding these head nodes, the discovery time (the order 
 of a node in the DFS traversal) and the low link (the vertex with the minimum discovery time that can be reached from 
 the subtree) must be calculated for all the vertices in the graph. A node is a head if the disc_time = low_link. All
 the vertices with the same low link will be part of a SCC. The topological sort algorithm is almost the same as a DFS 
 algorithm, except that it also has a stack used for printing the topological sorting.

*/

#include "pch.h"
#include <iostream>
using namespace std;
#include "Profiler.h"

Profiler profiler("DFS");
int op = 0;

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

void push(list* l, int value) //pushes an element at the end of the list
{
	Node* pNew = createNode(value);
	if (l->length == 0)
	{
		l->pFirst = l->pLast = pNew;
	}
	else
	{
		l->pLast->pNext = pNew;
		l->pLast = pNew;
	}

	pNew->pNext = NULL;
	l->length++;
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

bool empty(list l)
{
	if (l.pFirst == NULL)
	{
		return true;
	}
	return false;
}

void DFScomponent(list* adj, int i, bool* visited)
{
	visited[i] = true;
	op++;
	cout << i << " ";
	while (!empty(adj[i]))
	{
		int neighbor = pop(&adj[i]);
		if (visited[neighbor] == false)
		{
			DFScomponent(adj, neighbor, visited);
		}
	}
}

void dfs(list* adj, int n)
{
	bool* visited = (bool*)calloc(n + 1, sizeof(bool));

	for (int i = 1; i <= n; i++)
	{
		if (visited[i] == false)
			DFScomponent(adj, i, visited);
	}
	cout << endl;
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

void addEdge(list* adj, int x, int y) //adds an edge in the undirected graph
{
	push(&adj[x], y);
	if (x != y)
	{
		push(&adj[y], x);
	}
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

void stack_pop(list* stack)
{
	if (stack->pFirst == NULL)
		return;
	if (stack->pFirst->pNext == NULL)
	{
		stack->pFirst = stack->pLast = NULL;
	}
	else
	{
		Node* prev = stack->pFirst;
		while (prev->pNext != stack->pLast)
			prev = prev->pNext;
		prev->pNext = NULL;
		stack->pLast = prev;
	}
	stack->length--;
}

void SCC_visit(int node, int* disc_time, int* low_link, bool* on_stack, list* adj, list* stack, int* k)
{
	static int step = 0;
	disc_time[node] = low_link[node] = ++step;
	on_stack[node] = true;
	push(stack, node);

	while (!empty(adj[node]))
	{
		int neighbor = pop(&adj[node]);

		if (disc_time[neighbor] == -1)
		{
			SCC_visit(neighbor, disc_time, low_link, on_stack, adj, stack, k);
			low_link[node] = min(low_link[node], low_link[neighbor]);
		}
		else if(on_stack[neighbor]==true)
		{
			low_link[node] = min(low_link[node], disc_time[neighbor]);
		}
	}

	if (low_link[node] == disc_time[node])
	{
		cout << "SSC " << (*k)++ << ": ";
		while (stack->pLast->key != node)
		{
			cout << stack->pLast->key << " ";
			on_stack[stack->pLast->key] = false;
			stack_pop(stack);
		}
		cout << stack->pLast->key << " ";
		on_stack[stack->pLast->key] = false;
		stack_pop(stack);
		cout << endl;
	}
}

void tarjan(list* adj, int n)
{
	int* disc_time = (int*)calloc(n + 1, sizeof(int));
	int* low_link = (int*)calloc(n + 1, sizeof(int));
	bool* on_stack = (bool*)calloc(n + 1, sizeof(bool));
	list stack;
	initList(&stack);

	for (int i = 1; i <= n; i++)
		disc_time[i] = low_link[i] = -1;

	int k = 1;
	for (int i = 1; i <= n; i++)
	{
		if (disc_time[i] == -1)
		{
			SCC_visit(i, disc_time, low_link, on_stack, adj, &stack, &k);
		}
	}
	cout << endl;
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

void topSortUtil(list* adj, int node, bool* visited, list* stack)
{
	visited[node] = true;
	while (!empty(adj[node]))
	{
		int neighbor = pop(&adj[node]);
		if (visited[neighbor] == false)
		{
			topSortUtil(adj, neighbor, visited, stack);
		}
	}
	push(stack, node);
}

void topological_sort(list* adj, int n)
{
	list stack;
	initList(&stack);
	bool* visited = (bool*)calloc(n + 1, sizeof(bool));

	for (int i = 1; i <= n; i++)
	{
		if (visited[i] == false)
			topSortUtil(adj, i, visited, &stack);
	}

	while (!empty(stack))
	{
		cout << stack.pLast->key << " ";
		stack_pop(&stack);
	}
	cout << endl;
}

void test()
{
	list* adj = (list*)malloc(9 * sizeof(list));
	list* adj2 = (list*)malloc(9 * sizeof(list));
	list* adj3 = (list*)malloc(9 * sizeof(list));
	for (int i = 1; i <= 8; i++)
	{
		initList(&adj[i]);
		initList(&adj2[i]);
		initList(&adj3[i]);
	}

	//undirected graph for DFS
	addEdge(adj, 1, 2);
	addEdge(adj, 1, 3);
	addEdge(adj, 2, 3);
	addEdge(adj, 3, 4);
	addEdge(adj, 4, 4);
	addEdge(adj, 5, 6);
	addEdge(adj, 5, 8);

	//directed graph for tarjan's algorithm
	push(&adj2[1], 2);
	push(&adj2[1], 3);
	push(&adj2[2], 1);
	push(&adj2[3], 4);
	push(&adj2[4], 2);
	push(&adj2[4], 7);
	push(&adj2[4], 5);
	push(&adj2[5], 6);
	push(&adj2[6], 8);
	push(&adj2[8], 5);

	//dag for the topological sort
	push(&adj3[1], 2);
	push(&adj3[1], 3);
	push(&adj3[3], 4);
	push(&adj3[4], 7);
	push(&adj3[4], 5);
	push(&adj3[5], 6);
	push(&adj3[6], 8);

	cout << "The adjacency lists of the undirected graph: " << endl;
	for (int i = 1; i <= 8; i++)
	{
		cout << i << ": ";
		printList(adj[i]);
	}
	cout << endl;

	cout << "DFS: ";
	dfs(adj, 8);
	cout << endl;

	cout << "The adjacency lists of the directed graph: " << endl;
	for (int i = 1; i <= 8; i++)
	{
		cout << i << ": ";
		printList(adj2[i]);
	}
	cout << endl;

	cout << "The strongly connected components: " << endl;
	tarjan(adj2, 8);

	cout << "The adjacency lists of the dag: " << endl;
	for (int i = 1; i <= 8; i++)
	{
		cout << i << ": ";
		printList(adj3[i]);
	}
	cout << endl;
	
	cout << "Topological sort: ";
	topological_sort(adj3, 8);
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
			dfs(adj, V);
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
			dfs(adj, V);
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

