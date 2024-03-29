/* Assignment No. 4: Merge k Ordered Lists Efficiently

   An efficient method to merge k ordered lists having a total number of elements n is to use min-heaps in order to
   always store the smallest k elements from the list. At each step the root of the heap (the smallest element in the
   heap) will be extracted and inserted in the final, merged list which will have at the end of the algorithm the length n.

   The algorithm is analysed in the average case, where the lists are randomly generated, but in an increasing order.
   The measurements are repeated 5 times and their average is reported. In the first chart, there are 3 different cases
   depending on k: when k is 5, 10 and 100. There are generated k random sorted lists for each value of k, so that the
   combined number of elements in all the lists (n) varies between 100 and 10000. As the curves show, it can be seen 
   that as the number of input lists (k) increases, the number of operations becomes larger. The time complexity of the
   algorithm when n is varying and k is constant is linear (O(n)). For the second chart, there is just one case shown,
   for when k is varying from 10 to 500 and n remains the same (10000). The chart and the table show that the algorithm 
   performs a big number of operations, all of them depending on k. Also, it can be seen that the running time complexity
   of the algorithm in this case is O(lg k).

   Overall, when both k and n vary, the algorithm has a complexity of O(nlogk).
*/
#include "pch.h"
#include <iostream>
#include "Profiler.h"
using namespace std;
#define MAX_SIZE 10001
#define INF 99999

Profiler profiler("mergeklists");
int op = 0;

typedef struct node {
	int k;
	node *pNext;
}Node;

typedef struct List {
	int length;
	Node *pFirst, *pLast;
}List;

typedef struct heap {  //heap special structure
	int list_index, key;
}Heap;

void swap(Heap *a, Heap *b) //swaps two elements in a heap
{
	Heap aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

void initList(List* list)  //initializes a singly linked list
{
	if (list)
	{
		list->length = 0;
		list->pFirst = list->pLast = NULL;
	}
}

Node* createNode(int key)  //creates a node
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode)
	{
		newNode->k = key;
		newNode->pNext = NULL;
	}
	return newNode;
}

void insertNode(List* list, int key)  //inserts a node in the singly linked list
{
	Node* newNode = createNode(key);
	if (list->length == 0)
	{
		list->pFirst = newNode;
		list->pLast = newNode;
	}
	else
	{
		list->pLast->pNext = newNode;
		newNode->pNext = NULL;
		list->pLast = newNode;
	}
	list->length++;
}

void heapInsert(Heap h[], int i, int heap_length) //inserts an element in the heap, holding the min-heap property
{
	//traverses a simple path from the node h[i] to the root; compares this node to its parent
	while (i > 1 && h[i / 2].key > h[i].key) //checks if the min-heap property holds
	{
		op += 4; //counts 3 assignments + 1 comparison
		swap(&h[i], &h[i / 2]);
		i = i / 2;
	}
	op++; //counts one more comparison
}

void buildMinHeap(Heap h[], int heap_length) //top-down strategy; builds a heap
{
	for (int heap_size = 2; heap_size <= heap_length; heap_size++) //the nb of nodes in the heap increases
	{
		heapInsert(h, heap_size, heap_length);
	}
}

void pushHeap(Heap h[], int &heap_size, int key, int heap_length, int index) //pushes an element in the heap
{
	heap_size = heap_size + 1;
	op++;
	h[heap_size].key = key;
	h[heap_size].list_index = index;
	heapInsert(h, heap_size, heap_length);
}

int popFirst(List* list) //pops the first element in a list and returns its key
{
	int x = -1;
	if (list->pFirst != NULL)
	{
		x = list->pFirst->k;
		op++;
		list->pFirst = list->pFirst->pNext;
		if (list->pFirst == NULL)
		{
			list->pLast = NULL;
		}
		list->length--;
	}
	return x;
}

void heapify(Heap h[], int heap_size, int i) //min-heapify
{
	int l = 2 * i; //left child
	int r = 2 * i + 1; //right child
	int smallest; //the index of the smallest node (root, left child or right child)
	op += 2; //counts 2 comparisons
	if (l <= heap_size && h[l].key < h[i].key)
	{
		smallest = l;
	}
	else
	{
		smallest = i;
	}
	if (r <= heap_size && h[r].key < h[smallest].key)
	{
		smallest = r;
	}
	if (smallest != i) //if the smallest node is different from the current one
	{
		op += 3;
		swap(&h[i], &h[smallest]);
		heapify(h, heap_size, smallest);
	}
}

Heap extractMin(Heap h[], int &n) //extracts the first element (the smallest) in the heap
{
	Heap min = h[1];
	op++;
	h[1] = h[n];
	n--; //decreases the size of the heap
	heapify(h, n, 1); 
	return min;
}

List merge(List* list, int k, int n) //merges k sorted lists with a total of n elements
{
	List mList;
	initList(&mList);
	Heap h[MAX_SIZE];
	int i, heap_size=0, x;
	for (i = 0; i < MAX_SIZE; i++) //initializes the min-heap
		h[i].key = INF;

	for (i = 0; i < k; i++)
	{
		x = popFirst(&list[i]); //pops the first element in the list i and stores its key in x
		pushHeap(h, heap_size, x, n, i); //pushes in the heap the first element of each list
	}
	buildMinHeap(h, k); //builds the min heap with the k smallest elements
	while (heap_size >= 3) //while there are at least 3 elements in the heap
	{
		Heap min = extractMin(h, heap_size); //extracts the minimum element in the heap (the first)
		op++;
		insertNode(&mList, min.key); //inserts it in the merged list
		if (list[min.list_index].pFirst!=NULL) //if there are still elements in the list from which we last inserted an element in the merged list
		{
			pushHeap(h, heap_size, list[min.list_index].pFirst->k, n,min.list_index); //pushes in the heap the first element of that list
			popFirst(&list[min.list_index]); //pops it out from that list
		}
	}

	op++;
	insertNode(&mList, h[1].key); //insert the smallest element that remained in the heap
	List l1 = list[h[1].list_index];
	List l2 = list[h[2].list_index];

	while (l1.pFirst!=NULL && l1.pFirst->k < h[2].key) //find the place of the second element from the heap
	{
		op+=2;
		insertNode(&mList, l1.pFirst->k);
		popFirst(&l1);
	}
	op++;
	insertNode(&mList, h[2].key); //insert the second element that remained in the heap

	//merge the 2 remaining lists that still have elements and insert their keys in the final merged list
	while(l1.pFirst!=NULL && l2.pFirst!=NULL)
		if (l1.pFirst->k < l2.pFirst->k)
		{
			op+=2;
			insertNode(&mList, l1.pFirst->k);
			popFirst(&l1);
		}
		else if (l1.pFirst->k > l2.pFirst->k)
		{
			op+=2;
			insertNode(&mList, l2.pFirst->k);
			popFirst(&l2);
		}
		else
		{
			op += 2;
			insertNode(&mList, l1.pFirst->k);
			popFirst(&l1);
			insertNode(&mList, l2.pFirst->k);
			popFirst(&l2);
		}
	while (l1.pFirst != NULL)
	{
		op++;
		insertNode(&mList, l1.pFirst->k);
		popFirst(&l1);
	}
	while (l2.pFirst != NULL)
	{
		op++;
		insertNode(&mList, l2.pFirst->k);
		popFirst(&l2);
	}

	return mList;
}

void printList(List list) 
{
	Node* p = list.pFirst;
	while (p != NULL)
	{
		cout << p->k << " ";
		p = p->pNext;
	}
	cout << endl;
}

void deallocate(List* list)
{
	Node* p;
	while (list->pFirst != NULL)
	{
		p = list->pFirst;
		list->pFirst = list->pFirst->pNext;
		free(p);
	}
	list->pLast = NULL;
	list->length = 0;
}

void generateLists(List* list, int n, int k) //generates k random lists having a total of n elements
{
	int i, A[MAX_SIZE];
	int heap_length = 0;
	for (i = 0; i < k; i++)
	{
		initList(&list[i]);
		//equally (or almost equally) distribute the number of elements (n) to the k lists
		if (i < n%k) 
		{
			FillRandomArray(A, n / k + 1, 10, 50000, true, 1);
			for (int j = 0; j < n / k + 1; j++)
			{
				insertNode(&list[i], A[j]);
			}
		}
		else {
			FillRandomArray(A, n / k, 10, 50000, true, 1);
			for (int j = 0; j < n / k; j++)
			{
				insertNode(&list[i], A[j]);
			}
		}
		printList(list[i]);
	}
}

List runMerge(int n, int k) //runs the merge and returns the merged list
{
	List mergedList;
	List* list = (List*)malloc(k * sizeof(List));
	generateLists(list, n, k);
	initList(&mergedList);
	mergedList = merge(list, k, n);
	for (int i = 0; i < k; i++)
	{
		deallocate(&list[i]);
	}
	return mergedList;
}
void runAverageCase()
{
	List mergedList;
	int k[] = { 5,10,100 };
	int n, count;
	for (count = 0; count < 5; count++)
	{
		for (int i = 0; i < 3; i++)
		{
			for (n = 100; n <= MAX_SIZE; n = n + 100)
			{

				op = 0;
				mergedList = runMerge(n, k[i]);
				if (i == 0)
				{
					profiler.countOperation("mergek1-operations", n, op);
				}
				else if (i == 1)
				{
					profiler.countOperation("mergek2-operations", n, op);
				}
				else profiler.countOperation("mergek3-operations", n, op);
				{
					deallocate(&mergedList);
				}
			}
		}
	}
	profiler.divideValues("mergek1-operations", 5);
	profiler.divideValues("mergek2-operations", 5);
	profiler.divideValues("mergek3-operations", 5);
	profiler.createGroup("merge-operations", "mergek1-operations", "mergek2-operations", "mergek3-operations");
	
	n = MAX_SIZE-1;
	op=0;
	for (count = 0; count < 5; count++)
	{
		for (int k = 10; k <= 500; k = k + 10)
		{
				op = 0;
				mergedList = runMerge(n, k);
				profiler.countOperation("merge-operations1", k, op);
				deallocate(&mergedList);
		}
	}
	profiler.divideValues("merge-operations1", 5);
}
void test(int n, int k)
{
	List mergedList;
	mergedList= runMerge(n, k);
	printList(mergedList);
}
int main()
{
	int n=20, k=4;
	test(n,k);
	//runAverageCase();
	//profiler.showReport();
	return 0;
}



