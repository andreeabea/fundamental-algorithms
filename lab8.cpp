/* Assignment No. 8: Disjoint Sets
 The base operations for disjoint sets are: make_set, union and find_set. The make_set operation creates a new set with a 
 given key, having a representative (parent) with the same key. The union operation makes the union between two sets containing
 two given sets and it uses the heuristic union by rank that takes into account the height of the two trees, so as to make 
 the union. The find_set operation searches for the set containing a given element and it uses the heuristic path compression
 to link all the nodes that were found on the path to the given element to the root node. These two heuristics used in the 
 union and find_set algorithms improve the complexity of them, by reducing the complexity of O(n) in the worst case for each 
 of these operations (in the worst case the sets are a linear chain of n elements). The improved running time complexity is 
 almost a linear running time in the total number of operations (m) : O(m*alpha(n)), where alpha(n) represents the inverse of 
 the Ackermann function. The inverse of the Ackermann function is a very slowly growing function and it has very small values 
 that are smaller than 5 for any n => the complexity is almost linear. Therefore, by using these two improving heuristics, the
 running time complexity for each of these 2 operations becomes O(alpha(n)) -constant running time. The make_set operation also 
 has a constant running time complexity: O(1).
 In this assignment, n varies, the number of nodes (sets) from 100 to 10000, and for each n it is build a undirected, connected,
 random graph, with random weights on edges (n nodes, 4*n edges). A special structure edge is also used to create the graph, 
 actually creating an array of edges. Each edge has a start node (v1), an end node (v2) and a weight. To build this random graph
 and assure it is connected, we firstly link the n edges one by one, like a list, then add more 3*n random edges to the graph.
 For each of these graphs, the minimum spanning tree is found by using Kruskal's algorithm. By applying Kruskal's algorithm we 
 have a complexity of O(E*logE), where E is the number of edges and is 4*n => O(n*logn).
 As the generated graph shows, it can be seen that the function (computational effort as the sum of the comparisons and assignments
 performed by each individual base operation on disjoint sets) is a linear one.

*/
#include "pch.h"
#include <iostream>
using namespace std;
#include "Profiler.h"
#define MAX_SIZE 10000

Profiler profiler("Disjoint_sets");
int op = 0;

struct set
{
	int key;
	set* parent;
	int rank;
};

struct edge
{
	set* v1, *v2;
	int weight;
};

set* makeSet(int key) //makes a new set from a given key, having as a representantive that key
{
	set* s = (set*)malloc(sizeof(set));
	s->key = key;
	s->rank = 0;
	s->parent = s;
	op+=2;
	return s;
}

set* findSet(set* s) //searches for the set that contains the element s
{
	set* x=NULL;
	if (s == NULL)
		return NULL;
	op++;
	if (s != s->parent)
	{
		x = findSet(s->parent);
	}
	else return s;
	op++;
	s->parent = x;
	return s->parent;
}

void Union(set* s1, set* s2) //makes the union by rank between 2 sets
{
	set* x = findSet(s1);
	set* y = findSet(s2);
	if (x->rank > y->rank)
	{
		y->parent = x;
		op++;
	}
	else
	{
		op++;
		x->parent = y;
		if (x->rank == y->rank)
			y->rank += 1;
	}
}

void swap(edge *a, edge *b)
{
	edge aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

int partition(edge A[], int n, int start, int end)
{
	edge x = A[end]; //the pivot is the last element in the array
	int i = start - 1;
	for (int j = start; j <= end - 1; j++)
	{
		if (A[j].weight <= x.weight) {
			i++;
			swap(&A[i], &A[j]);
		}
	}
	swap(&A[i + 1], &A[end]);
	return (i + 1);
}

void quicksort(edge A[], int n, int start, int end)
{
	if (start < end) //if the array is not empty
	{
		int p = partition(A, n, start, end); //index at the boundary of the two partitions
		quicksort(A, n, start, p - 1);
		quicksort(A, n, p + 1, end);
	}

}

void kruskal(edge edges[], int n)
{
	quicksort(edges, n, 0, n-1); //sort the edges
	for (int i = 0; i < n; i++)
	{
		if (findSet(edges[i].v1) != findSet(edges[i].v2)) //if union of the two edges will not close a cycle
		{
			cout << edges[i].v1->key << " " << edges[i].v2->key << " " << edges[i].weight << endl;
			Union(edges[i].v1, edges[i].v2);
		}
	}
}

void test()
{
	set* sets[11], *s;
	int A[10];
	cout << "The sets are: " << endl;
	//make 10 random sets
	FillRandomArray(A, 10, 1, 10, true, 0);
	for (int i = 1; i <= 10; i++)
	{
		sets[i]=makeSet(A[i-1]);
		cout << "set[" << i << "]: " << " ";
		cout << sets[i]->parent->key << " " << sets[i]->key << " " << endl;
	}
	cout << endl;
	//make 5 unions between sets from the first 5 and from the last 5
	for (int i = 1; i <= 5; i++)
		Union(sets[i], sets[10 - i + 1]);
	cout << endl;
	for (int i = 1; i <= 10; i++)
	{
		if (sets[i] != NULL)
		{
			if (i <= 5)
				cout << "Union between set[" << i << "] and set[" << 10 - i + 1 << "]: ";
			else cout << "set[" << i << "]: " << " ";
			cout << sets[i]->parent->key << " " << sets[i]->key << " " << endl;
		}
	}
	cout << endl;
	//find 5 sets
	for (int i = 1; i <= 6; i++)
	{
		cout << "findSet(set[" << i << "]): ";
		cout << findSet(sets[i])->key << " ";
		cout << endl;
	}
	cout << endl;
	cout << "The 4 edges and their weights: " << endl;
	edge e[5];
	set *s1, *s2, *s3, *s4;
	s1 = makeSet(1);
	s2 = makeSet(2);
	s3 = makeSet(3);
	s4 = makeSet(4);
	e[1].v1 = s1;
	e[1].v2 = s2;
	e[1].weight = 15;

	e[2].v1 = s2;
	e[2].v2 = s3;
	e[2].weight = 10;

	e[3].v1 = s3;
	e[3].v2 = s4;
	e[3].weight = 5;

	e[0].v1 = s4;
	e[0].v2 = s1;
	e[0].weight = 20;
	for (int i = 0; i <= 3; i++)
		cout << e[i].v1->key << " " << e[i].v2->key << " " << e[i].weight << " " << endl;
	cout << endl;
	cout << "Kruskal: " << endl;
	kruskal(e, 4);
}

int checkEdge(edge edges[], int n, int key1, int key2) //check if an unoriented edge with nodes containing key1 and key2 is already in the graph or not 
{
	for (int i = 0; i < n; i++)
		if ((edges[i].v1->key == key1 && edges[i].v2->key == key2) || (edges[i].v1->key == key2 && edges[i].v2->key == key1))
			return 0;
	return 1;
}

void runCase()
{
	int weights[4*MAX_SIZE];
	edge edges[4 * MAX_SIZE];
	for (int n = 100; n <= MAX_SIZE; n += 100)
	{
		FillRandomArray(weights, 4 * n, 10, 50000, false, 0); //random weights for all the edges
		op = 0;
		//create an array of all the added nodes to the graph
		set** nodeArray = (set**)malloc(n * sizeof(set*));
		nodeArray[0] = makeSet(0);
		//first add n edges linked like a list (to be sure the graph will be connected)
		for (int i = 0; i < n-1; i++)
		{
			nodeArray[i + 1] = makeSet(i + 1);
			edges[i].v1 = nodeArray[i];
		    edges[i].v2 = nodeArray[i+1];
			edges[i].weight = weights[i];
		}
		//add the rest of the edges randomly
		for (int i = n-1; i < 4 * n; i++)
		{
			int r[2];
			do
			{
				FillRandomArray(r, 2, 0, n - 1, true, 0);
			} while (checkEdge(edges, i, r[0], r[1]) == 0); //also check if the edge we would want to add doesn't already exist
			edges[i].v1 = nodeArray[r[0]];
			edges[i].v2 = nodeArray[r[1]];
			edges[i].weight = weights[i];
		}

		kruskal(edges, 4*n);
		profiler.countOperation("Disjoint_sets", n, op);
	}
}
int main()
{
	test();

    //runCase();
    //profiler.showReport();

	return 0;
}

