/*Assignment No. 1: Analysis & Comparison of Direct Sorting MethodsThe 3 direct sorting methods implemented are: Insertion sort - using linear insertion, Selection sort and Bubble sort.

In the best case, the generated arrays are already sorted in an increasing order and some operations will not be done.
As we can see in the charts, in the best case, the bubble sort algorithm does the smallest number of operations, having 
a complexity of O(n). The insertion sort algorithm also has a complexity of O(n) in the best case, but the running time
of bubble sort is smaller because the number of assignments is 0 when the array is sorted. Selection sort has a O(n^2)
time complexity, so it is not efficient in this case, compared to insertion sort or bubble sort. Therefore, if we have
an already sorted array the best is to use bubble sort (for example if we also need to check if the array is sorted).
Also, the number of comparisons of insertion sort is the same as for bubble sort, so if we have arrays with small number
of elements insertion sort is the best to use because bubble sort is not that efficient in the other cases.
Insertion sort: O(n) assignments, O(n) comparisons
Selection sort: O(n) assignments, O(n^2) comparisons
Bubble sort: O(1) assignments, O(n) comparisons

In the average case, the arrays are randomly generated (for each algorithm the same array to make the comparison fair).
The measurements are repeated 5 times and their average is reported. As the charts show, all three sorting methods
have a time complexity of O(n^2). Even though the number of assignments of insertion sort is greater than the nb. of 
assignments of selection sort, the number of operations of insertion sort is still the smallest, so it would be the most
efficient to use between these three sorting methods. The bubble sort method uses the biggest number of operations => 
not efficient.
Insertion sort: O(n^2) assignments, O(n^2) comparisons
Selection sort: O(n) assignments, O(n^2) comparisons
Bubble sort: O(n^2) assignments, O(n^2) comparisons

In the worst case, the generated arrays are sorted in decreasing order for all the algorithms. As the selection sort
always does the same number of operations, in the worst case it is the most efficient because it does the smallest 
number of operations, compared to insertion sort or bubble sort, bubble sort being the most inefficient. The worst case
performance for all these algorithms is O(n^2).
Insertion sort: O(n^2) assignments, O(n^2) comparisons
Selection sort: O(n) assignments, O(n^2) comparisons
Bubble sort: O(n^2) assignments, O(n^2) comparisons

Stability: insertion sort and bubble sort are stable because the order of the same elements does not change, while
selection sort is unstable

*/
#include "pch.h"
#include <iostream>
#include "Profiler.h"
#include <time.h>
#define MAX_SIZE 10000
using namespace std;

Profiler profiler("demo");

void swap(int *a, int *b) //swaps two integers
{
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}
void insertionsort(int a[], int n) //stable algorithm
{
	int buff,j;
	for (int i = 1; i < n; i++)
	{
		buff = a[i];
		profiler.countOperation("Insertion_assignments",n); // nb of assignments increases with 1
		j = i - 1;
		//in the best case the program doesn't enter the while loop
		while (j >= 0 && a[j] > buff)
		{   //in the worst case, both the nb. of assignments and comparisons are incremented i times
			profiler.countOperation("Insertion_assignments", n);
			profiler.countOperation("Insertion_comparisons", n);
			a[j + 1] = a[j];
			j--;
		}
		profiler.countOperation("Insertion_comparisons", n);
		profiler.countOperation("Insertion_assignments", n); // nb of assignments increases with 1
		a[j + 1] = buff;
	}
	// => at the end of the for loop 2n-2 assignments and n-1 comparisons in the best case
	// => (n-1)(n+4)/2 assignments and (n-1)(n+2)/2 comparisons in the worst case
}
void selectionsort(int a[], int n) //not stable
{
	int i, j, min_i;
	//has the same number of comparisons and the same of assignments for all the cases (best,average,worst)
	//because the program enters the 2 for loops every time (there is no condition)
	for (i = 0; i < n - 1; i++)
	{
		min_i = i;
		for (j = i + 1; j < n; j++) // n-i-1 iterations
		{
			if (a[j] < a[min_i])
				min_i = j;
			profiler.countOperation("Selection_comparisons", n);
		}
		profiler.countOperation("Selection_assignments", n, 3); //increment with 3 the nb of assignments because the swap operation does 3 assignments
		swap(&a[i], &a[min_i]);
	}
	//=> 3n-3 assignments and n(n-1)/2 comparisons
}
void bubblesort(int a[], int n) //stable
{
	int i;
	bool swapped = true;
	Operation op = profiler.createOperation("Bubble_assignments", n);
	while (swapped)
	{
		swapped = false;
		//in the best case the program goes through this for loop just once => nb of comparisons = n-1
		for (i = 0; i <= n - 2; i++)
		{
			profiler.countOperation("Bubble_comparisons", n);
			//for the best case op (the nb of assignment) is never counted => nb of assignments = 0
			if (a[i] > a[i + 1])
			{
				op.count(3); //the nb. of assignments is increased with 3
				swap(&a[i], &a[i + 1]);
				swapped = true;
			}
		}
	}
	// => 3*n(n-1)/2 assignments and n(n-1) comparisons in the worst case
}
void test()
{
	int v[] = { 5,2,8,1,10,4 }, i, v1[6], v2[6];
	for (i = 0; i < 6; i++)
	{
		v1[i] = v[i];
		v2[i] = v[i];
	}
	insertionsort(v, 6);
	for (i = 0; i < 6; i++)
		cout << v[i] << " ";
	cout << endl;
	selectionsort(v1, 6);
	for (i = 0; i < 6; i++)
		cout << v1[i] << " ";
	cout << endl;
	bubblesort(v2, 6);
	for (i = 0; i < 6; i++)
		cout << v2[i] << " ";
	cout << endl;
}

int main()
{
	int a[MAX_SIZE],n;

	//best case - increasing order arrays for all the 3 algorithms
	for (n = 100; n <= MAX_SIZE; n = n + 100)
	{
		FillRandomArray(a, n, 10, 50000, true, 1);
		bubblesort(a, n);
	}
	for (n = 100; n <= MAX_SIZE ; n = n + 100)
	{
		FillRandomArray(a, n, 10, 50000, true, 1);
		insertionsort(a, n);
	}
	for (n = 100; n <= MAX_SIZE; n = n + 100)
	{
		FillRandomArray(a, n, 10, 50000, true, 1);
		selectionsort(a, n);
	}
	profiler.createGroup("Best case assignments", "Insertion_assignments", "Selection_assignments","Bubble_assignments");
	

	profiler.reset();
	
	//average case
	for (int i = 0; i < 5; i++)
	{
		for (n = 100; n <= MAX_SIZE; n = n + 100)
		{
			FillRandomArray(a, n, 10, 50000, false, 0);
			int a1[MAX_SIZE], a2[MAX_SIZE];
			for (int j = 0; j < n; j++)
			{
				a1[j] = a[j];
				a2[j] = a[j];
			}
			insertionsort(a, n);
			selectionsort(a1, n);
			bubblesort(a2, n);
		}
		
	}
	profiler.divideValues("Insertion_assignments", 5);
	profiler.divideValues("Insertion_comparisons", 5);
	profiler.divideValues("Selection_assignments", 5);
	profiler.divideValues("Selection_comparisons", 5);
	profiler.divideValues("Bubble_assignments", 5);
	profiler.divideValues("Bubble_comparisons", 5);

	profiler.createGroup("Average case assignments", "Insertion_assignments", "Bubble_assignments");
	profiler.createGroup("Average case comparisons", "Insertion_comparisons", "Selection_comparisons", "Bubble_comparisons");
	
	profiler.reset();
	
	//worst case - decreasing ordered arrays
	for (n = 100; n <= MAX_SIZE; n = n + 100)
	{
		FillRandomArray(a, n, 10, 50000, true, 2);
		bubblesort(a, n);
	}
	for (n = 100; n <= MAX_SIZE; n = n + 100)
	{
		FillRandomArray(a, n, 10, 50000, true, 2);
		insertionsort(a, n);
	}
	for (n = 100; n <= MAX_SIZE; n = n + 100)
	{
		FillRandomArray(a, n, 10, 50000, true, 2);
		selectionsort(a, n);
	}
	profiler.createGroup("Worst case assignments", "Insertion_assignments", "Selection_assignments", "Bubble_assignments");
	profiler.createGroup("Worst case comparisons", "Selection_comparisons", "Bubble_comparisons");
	
	profiler.showReport();
	test();
	return 0;
}

