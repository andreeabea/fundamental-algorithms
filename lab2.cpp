/*Assignment No. 2: Analysis & Comparison of Bottom­-up and Top-­down Build Heap Approaches
  
  The bottom-up approach uses the heapify function n/2 times in order to build a max-heap. The heapify function
  is called n/2 times because it has to verify and maintain the max-heap property for all the non-leave nodes, 
  which are half of the total number of nodes. The time required by heapify function when called on a node of 
  height h is O(h) (it varies on the level of the node) and it is applied from each non-leave node to the 
  root. Knowing that an n-element heap has the height [lg n] and there are at most [n/2^(h+1)] nodes of any 
  height h, it can be proved by summing the number of nodes * time for each level (for each level the nb of 
  elements halves, while the nb of steps required to heapify each increases by 1) that the running time of the 
  bottom-up build heap approach is O(n).

  The top-down approach uses an insertion function which adds each new element in the heap and bubbles it from
  the bottom, upper in the heap, until it finds a larger-value parent. The running time of heapInsert on an n-
  element heap is O(lg n), since the path traced from the node to the root has the length [lg n]. As the insertion
  function is called n-1 times, it results that the running time of the top-down build heap approach is O(n*lg n).

  Because the bottom-up and top-down build heap functions use different strategies, they don't always create the 
  same heap when run on the same input array. The bottom-up approach is faster (O(n)), but it has a fixed 
  dimension, while the top-down approach even it is slower (O(n*lg n)), having a variable dimension, it could be 
  used when the size of the heap is unknown. If the size of the heap is fixed, known, the bottom-up approach is
  more efficient.

  In the average case, the arrays are randomly generated (for each algorithm the same array to make the comparison
  fair). The measurements are repeated 5 times and their average is reported. As the charts show, for small numbers
  of elements, the difference between the numbers of operations of the 2 algorithms is not that big, but as the nb
  of elements increases it can be clearly seen that the bottom-up build heap algorithm is more efficient (the number
  of operations of the bottom-up approach is smaller than the nb of operations of the top-down approach).
 
  In the worst case, the generated arrays are sorted in increasing order for both of the algorithms. The input arrays
  represent actually min-heaps from which the algorithms will build max-heaps. This requires a maximum number of 
  operations for both of the algorithms. The bottom-up approach does less operations (both assignments and comparisons)
  than the top-down approach, so it is more efficient.

  Bottom-up: O(n) operations
  Top-down: O(n*lg n) operations

*/
#include "pch.h"
#include <iostream>
#include "Profiler.h"
using namespace std;
#define MAX_SIZE 10000
#define INF 9999

Profiler profiler("build-heap");

void swap(int *a, int *b)
{
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}
void heapify(int A[], int heap_size, int i) 
{
	int l = 2 * i; //left child
	int r = 2 * i + 1; //right child
	int largest; //the index of the largest node (root, left child or right child)
	profiler.countOperation("bottom-up-comparisons", heap_size, 2);
	if (l <= heap_size && A[l] > A[i])
	{
		largest = l;
	}
	else
	{
		largest = i;
	}
	if (r <= heap_size && A[r] > A[largest])
	{
		largest = r;
	}
	if (largest != i) //if the largest node is different from the current one
	{
		profiler.countOperation("bottom-up-assignments", heap_size, 3);
		swap(&A[i], &A[largest]);
		heapify(A, heap_size, largest);
	}
}
void buildMaxHeap(int A[],int heap_size) //bottom-up strategy
{
	for (int i = heap_size / 2; i >= 1; i--) //from the non-leave nodes to the root
	{
		heapify(A, heap_size, i); //heap_length is the same as heap_size
	}
}
void heapInsert(int A[], int i, int key, int heap_length)
{
	//traverses a simple path from the node A[i] to the root; compares this node to its parent
	while (i > 1 && A[i / 2] < A[i]) //checks if the max-heap property holds
	{
		profiler.countOperation("top-down-comparisons", heap_length);
		profiler.countOperation("top-down-assignments", heap_length, 3);
		swap(&A[i], &A[i / 2]);
		i = i / 2;
	}
	profiler.countOperation("top-down-comparisons", heap_length);
}
void buildMaxHeap2(int A[],int heap_length) //top-down strategy
{
	for (int heap_size = 2; heap_size <= heap_length; heap_size++) //the nb of nodes in the heap increases
	{
		heapInsert(A, heap_size, A[heap_size], heap_length);
	}
}
void copyArray(int A[], int A2[], int n)
{
	for (int i = 1; i <= n; i++)
		A2[i] = A[i];
}
void test()
{
	int A[MAX_SIZE], A2[MAX_SIZE], heap_size, i;
	cin >> heap_size;
	for (i = 1; i <= heap_size; i++)
	{
		cin >> A[i];
	}
	copyArray(A, A2, heap_size);
	buildMaxHeap(A, heap_size);
	buildMaxHeap2(A2, heap_size);
	for (i = 1; i <= heap_size; i++)
	{
		cout << A[i] << " ";
	}
	cout << endl;
	for (i = 1; i <= heap_size; i++)
	{
		cout << A2[i] << " ";
	}
}
void runAverageCase()
{
	int A[MAX_SIZE + 1], A2[MAX_SIZE + 1];
	for (int i = 0; i <= 4; i++)
	{
		for (int n = 100; n <= MAX_SIZE; n = n + 100)
		{
			FillRandomArray(A, n, 10, 50000, false, 0);
			copyArray(A, A2, n);
			buildMaxHeap(A, n);
			buildMaxHeap2(A2, n);
		}

	}
	profiler.divideValues("bottom-up-comparisons", 5);
	profiler.divideValues("bottom-up-assignments", 5);
	profiler.divideValues("top-down-comparisons", 5);
	profiler.divideValues("top-down-assignments", 5);

	profiler.createGroup("Average Case Comparisons", "bottom-up-comparisons", "top-down-comparisons");
	profiler.createGroup("Average Case Assignments", "bottom-up-assignments", "top-down-assignments");
	profiler.addSeries("bottom-up-operations", "bottom-up-comparisons", "bottom-up-assignments");
	profiler.addSeries("top-down-operations", "top-down-comparisons", "top-down-assignments");
	profiler.createGroup("Average Case Operations", "bottom-up-operations", "top-down-operations");
}
void runWorstCase() //worst case is when the generated arrays are in increasing order (they represent min-heaps)
{
	int A[MAX_SIZE + 1], A2[MAX_SIZE + 1];
	for (int n = 100; n <= MAX_SIZE; n = n + 100)
	{
		FillRandomArray(A, n, 10, 50000, true, 1);
		copyArray(A, A2, n);
		buildMaxHeap(A, n);
		buildMaxHeap2(A2, n);
	}
	profiler.createGroup("Worst Case Comparisons", "bottom-up-comparisons", "top-down-comparisons");
	profiler.createGroup("Worst Case Assignments", "bottom-up-assignments", "top-down-assignments");
	profiler.addSeries("bottom-up-operations", "bottom-up-comparisons", "bottom-up-assignments");
	profiler.addSeries("top-down-operations", "top-down-comparisons", "top-down-assignments");
	profiler.createGroup("Worst Case Operations", "bottom-up-operations", "top-down-operations");
}
int main()
{ 
	/*runAverageCase();
	profiler.reset();
	runWorstCase();
	profiler.showReport();*/
	test();
	return 0;
}

