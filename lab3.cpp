/*Assignment No. 3: Analysis & Comparison of Advanced Sorting Methods – Heapsort and Quicksort / QuickSelectIn the best case, the generated array for the heapsort algorithm is in decreasing order, representing a max-heap.Because the heapsort algorithm uses the bottom-up build heap strategy and the max-heapify function, the best casewould be when the input heap is a max-heap. The generated array in the best case for the quicksort method would bean increasing ordered array where the pivot (in the partition function) will always be the element in the middleof the array. Because we already have the partition function in quicksort, which always chooses the last elementof the array as a pivot, after having the ordered array, it should be generated another array which selected as aninput to the quicksort function, will always have set the pivot (at each recursion) as the middle element. Both heapsort and quicksort have the running time complexity of O(n*lg n), but as we can see from the charts and the tables, heapsort does more operations and also its function increases faster, as the number of elements of thearray increases. So, in this case quicksort is more efficient.In the average case, the arrays are randomly generated (for each algorithm the same array to make the comparison fair).
The measurements are repeated 5 times and their average is reported. As the charts show, the average case is closeto the best case, both algorithms having the same running time performance of O(n*lg n), but again quicksort havingless operations (comparisons and assignments) than heapsort, therefore being more efficient.In the worst case, the generated arrays for both heapsort and quicksort are in increasing order. For heapsort, ifthe array is in increasing order it will represent a min-heap which will take the maximum number of operations inorder to obtain a max-heap, thus to sort the array. For the quicksort algorithm, which always takes the pivot asthe last element in the array, the input array could be even in increasing or decreasing order such that thealgorithm will lead to the worst case. In this case, it can be seen that quicksort's function increases very fastand the algorithm also does much more operations (both comparisons and assignments) than heapsort. The time complexity of the quicksort algorithm in the worst case is O(n^2), while the heapsort's complexity still remainsO(n*lg n). Therefore, if the input array is already sorted, it would be more efficient between these two methods,to use heapsort.

Neither heapsort nor quicksort are stable. Also, quicksort is usually faster than heapsort, but in the worst case
heapsort is better because the running time is always O(n*lg n) and the heapsort strategy is optimal, while 
quicksort's running time is O(n^2) in the worst case scenario.

*/
#include "pch.h"
#include <iostream>
using namespace std;
#include "Profiler.h"
#define MAX_SIZE 10001

Profiler profiler("lab3");
void swap(int *a, int *b)
{
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}
void heapify(int A[], int n, int heap_size, int i)
{
	int l = 2 * i; //left child
	int r = 2 * i + 1; //right child
	int largest; //the index of the largest node (root, left child or right child)
	profiler.countOperation("heapsort-comparisons", n, 2);
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
		profiler.countOperation("heapsort-assignments", n, 3);
		swap(&A[i], &A[largest]);
		heapify(A, n, heap_size, largest);
	}
}
void buildMaxHeap(int A[], int n, int heap_size) //bottom-up strategy; complexity: O(n)
{
	for (int i = heap_size / 2; i >= 1; i--) //from the non-leave nodes to the root
	{
		heapify(A, n, heap_size, i); //heap_length is the same as heap_size
	}
}
void heapsort(int A[], int n)
{
	int heapsize = n;
	buildMaxHeap(A, n, heapsize);
	for (int i = 2; i <= n; i++) 
	{
		profiler.countOperation("heapsort-assignments", n, 3);
		swap(&A[n - i +2], &A[1]);
		heapsize--;
		heapify(A, n, heapsize, 1);
	}
}
int partition(int A[], int n, int start, int end)
{
	int x = A[end]; //the pivot is the last element in the array
	profiler.countOperation("quicksort-assignments", n);
	int i = start - 1;
	for (int j = start; j <= end-1; j++)
	{
		profiler.countOperation("quicksort-comparisons", n);
		if (A[j] <= x) {
			i++;
			profiler.countOperation("quicksort-assignments", n, 3);
			swap(&A[i], &A[j]);
		}
	}
	profiler.countOperation("quicksort-assignments", n, 3);
	swap(&A[i + 1], &A[end]);
	return (i + 1);
}
void quicksort(int A[], int n, int start, int end)
{
	if (start < end) //if the array is not empty
	{
		int p = partition(A, n, start, end); //index at the boundary of the two partitions
		quicksort(A, n, start, p - 1);
		quicksort(A, n, p + 1, end);
	}

}
int quickSelect(int A[], int n,int start, int end, int i)
{
	if (start == end) //if the ith element is in the right place
	{
		return A[start];
	}
	int q = partition(A, n, start, end); //index of the position where the partition stops
	int k = q - start + 1; //length of the left partition
	if (i == k)
	{
		return A[q];
	}
	else if (i < k)
	{
		return quickSelect(A, n, start, q - 1, i);
	}
	else
	{
		return quickSelect(A, n, q + 1, end, i - k);
	}
}
void copyArray(int A[], int A2[], int n)
{
	for (int i = 1; i <= n; i++)
	{
		A2[i] = A[i];
	}
}
void runAverageCase()
{
	int A[MAX_SIZE], A2[MAX_SIZE];
	for (int i = 0; i <= 4; i++)
	{
		for (int n = 100; n <= MAX_SIZE; n = n + 100)
		{
			FillRandomArray(A, n+1, 10, 50000, false, 0);
			copyArray(A, A2, n);
			heapsort(A, n);
			quicksort(A2, n, 1, n);
		}

	}
	profiler.divideValues("heapsort-assignments", 5);
	profiler.divideValues("quicksort-assignments", 5);
	profiler.divideValues("heapsort-comparisons", 5);
	profiler.divideValues("quicksort-comparisons", 5);

	profiler.createGroup("Average Case Comparisons", "heapsort-comparisons", "quicksort-comparisons");
	profiler.createGroup("Average Case Assignments", "heapsort-assignments", "quicksort-assignments");
	profiler.addSeries("heapsort-operations", "heapsort-comparisons", "heapsort-assignments");
	profiler.addSeries("quicksort-operations", "quicksort-comparisons", "quicksort-assignments");
	profiler.createGroup("Average Case Operations", "heapsort-operations", "quicksort-operations");
}
void runWorstCase() //worst case is when the generated arrays are in increasing order
{
	int A[MAX_SIZE], A2[MAX_SIZE];
	for (int n = 100; n < MAX_SIZE; n = n + 100)
	{
		FillRandomArray(A, n+1, 10, 50000, true, 1);
		copyArray(A, A2, n);
		heapsort(A, n); //the increasing ordered array actually represents a min-heap
		quicksort(A2, n, 1, n);
	}
	profiler.createGroup("Worst Case Comparisons", "heapsort-comparisons", "quicksort-comparisons");
	profiler.createGroup("Worst Case Assignments", "heapsort-assignments", "quicksort-assignments");
	profiler.addSeries("heapsort-operations", "heapsort-comparisons", "heapsort-assignments");
	profiler.addSeries("quicksort-operations", "quicksort-comparisons", "quicksort-assignments");
	profiler.createGroup("Worst Case Operations", "heapsort-operations", "quicksort-operations");
}
void generateBestCaseArr(int A[], int left, int right)
{
	int len = right - left + 1;
	if (len < 3)
	{
		return;
	}
	//the index of the element in the middle
	int middle = (right + left) / 2;
	//generate the best case of the right half of the array : [middle, right]
	generateBestCaseArr(A, middle--, right); 
	//swap the pivot (last element) with the middle one
	swap(&A[middle], &A[right]);
	//generate the best case of the left part : [left, middle)
	generateBestCaseArr(A, left, middle);
}
void runBestCase() 
{
	int A[MAX_SIZE], A2[MAX_SIZE];
	for (int n = 100; n < MAX_SIZE; n = n + 100)
	{
		FillRandomArray(A, n+1, 10, 50000, true, 1); //increasing order array
		generateBestCaseArr(A, 1, n); //generate the array that will always take the middle element as a pivot in partition function
		FillRandomArray(A2, n + 1, 10, 50000, false, 2); //decreasing ordered array
		heapsort(A2, n);
		quicksort(A, n, 1, n);
	}
	profiler.createGroup("Best Case Comparisons", "heapsort-comparisons", "quicksort-comparisons");
	profiler.createGroup("Best Case Assignments", "heapsort-assignments", "quicksort-assignments");
	profiler.addSeries("heapsort-operations", "heapsort-comparisons", "heapsort-assignments");
	profiler.addSeries("quicksort-operations", "quicksort-comparisons", "quicksort-assignments");
	profiler.createGroup("Best Case Operations", "heapsort-operations", "quicksort-operations");
}
void test()
{
	int A[MAX_SIZE], A2[MAX_SIZE], n;
	//read the nb of elements and the array
	cin >> n;
	int i;
	for (i = 1; i <= n; i++)
	{
		cin >> A[i];
	}
	copyArray(A, A2, n);
	heapsort(A, n);
	//output the sorted array (using heapsort)
	for (i = 1; i <= n; i++)
	{
		cout << A[i] << " ";
	}
	cout << endl;
	int k;
	//the index of an element in the sorted array
	cin >> k;
	int A3[MAX_SIZE];
	copyArray(A2, A3, n);
	int place = quickSelect(A3, n, 1, n, k); //the element placed on the position k in the sorted array
	cout << place << endl;
	quicksort(A2, n, 1, n);
	//output the sorted array (using quicksort)
	for (i = 1; i <= n; i++)
	{
		cout << A2[i] << " ";
	}
}
int main()
{
	test();
	/*runBestCase();
	profiler.reset();
	runAverageCase();
	profiler.reset();
	runWorstCase();
	profiler.showReport();*/
	return 0;
}

