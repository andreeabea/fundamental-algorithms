/* Assignment No. 5: Search Operation in Hash Tables
Open Addressing with Quadratic Probing

 Insert and search operations in a hashtable were implemented using open addressing and quadratic probing which means that the
 final address is not completely determined by the hash code, it also depends on the elements which are already in the hash table.
 Not more than one element can be stored at a certain index (closed hashing) and each entry contains either an element of the
 dynamic set or NIL. When searching for an element, we successively examine the table until we find it or until we are sure
 that it is not in the table and to perform an insertion we probe the hash table until an empty slot is found to put the key in.
 Instead of being fixed in order (which requires O(n) search time), the sequence of positions probed depends upon the key being
 inserted.
 Both the insert and search algorithms perform O(1) operations, so these are very efficient in most of the cases, more efficient
 than inserting and searching in linked lists for example.
 The search operation is evaluated in the average case, 5 runs being performed for each filling factor given. First, random
 elements are inserted in the hashtable until the filling factor is the one we want. For performing the search of the m (=3000) 
 elements such that half of them can be found and the others not, a random array of 1500 indexes from the hash table was generated
 and for each it was checked if that slot in the hashtable is occupied (if not, replace the index) and for the other half elements 
 to be searched, an array of elements greater than the greatest possible element in the hashtable is generated, such that if we search
 these elements in the hashtable none of it will be found.
 As the table shows, as the filling factor of the hashtable increases, the efforts for the found and not-found elements increase, too.
 Also, from the table we can observe that, for each filling factor, the average effort (the expected number of probes) for the
 successful searches is approximately 1/alpha*ln(1/(1-alpha)) (also assuming that each key is equally likely to be searched for)
 and for the unsuccessful searches is 1/(1-alpha).
 */
#include "pch.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include "Profiler.h"
using namespace std;
#define N 9973

int op; //number of operations

typedef struct e
{
	int id;
	char name[30];
}Entry;

int f(int x, int n, int i)
{
	return (x + (3 * i + i * i)) % n;
}

void initHash(Entry hashT[], int n) //initializes the hash table
{
	for (int i = 0; i < n; i++)
	{
		hashT[i].id = 0;
		hashT[i].name[0] = '\0';
	}
}

int hashInsert(Entry hashT[], Entry e, int n) //returns the position where a new element is inserted (if a new slot is occupied)
{
	int i = 0;
	do
	{
		int j = f(e.id, n, i);
		if (hashT[j].id == 0) 
		{
			hashT[j].id = e.id;
			strcpy_s(hashT[j].name, e.name);
			return j;
		}
		else if (hashT[j].id == e.id)
		{
			strcpy_s(hashT[j].name, e.name);
			return -1;
		}
		else
		{
			i++;
		}
	} while (i < n);

	return -1;
}
int hashSearch(Entry hashT[], int e, int n)
{
	int i = 0;
	int j;
	op=0;
	do
	{
		j = f(e, n, i);
		op++;
		if (hashT[j].id == e)
			return j;
		i++;
	} while (hashT[j].id != 0 && i < n);
	return -1;
}

void printHash(Entry hashT[], int n)
{
	for (int i = 0; i < n; i++)
	{
		if (hashT[i].id != 0)
			cout << i << " " << hashT[i].id << " " << hashT[i].name << endl;
	}
	cout << endl;
}
void test()
{
	Entry HashT[N];
	int n = 11;
	initHash(HashT, n);
	Entry e1, e2, e3, e4, e5, e6;
	e1.id = 1;
	e2.id = 14;
	e3.id = 5;
	e4.id = 25;
	e5.id = 3;
	e6.id = 10;
	strcpy_s(e1.name, "Andreea");
	strcpy_s(e2.name, "Victor");
	strcpy_s(e3.name, "Bianca");
	strcpy_s(e4.name, "Radu");
	strcpy_s(e5.name, "Ioana");
	strcpy_s(e6.name, "Andrei");
	hashInsert(HashT, e1, n);
	hashInsert(HashT, e2, n);
	hashInsert(HashT, e3, n);
	hashInsert(HashT, e4, n);
	hashInsert(HashT, e5, n);
	hashInsert(HashT, e6, n);
	printHash(HashT, n);
	strcpy_s(e2.name, "Tudor");
	hashInsert(HashT, e2, n);
	printHash(HashT, n);
	int i = hashSearch(HashT, e1.id, n);
	int j = hashSearch(HashT, e2.id, n);
	int k = hashSearch(HashT, e3.id, n);
	int l = hashSearch(HashT, e4.id, n);
	int m = hashSearch(HashT, e5.id, n);

	cout << i << " " << HashT[i].id << " " << HashT[i].name << endl;
	cout << j << " " << HashT[j].id << " " << HashT[j].name << endl;
	cout << k << " " << HashT[k].id << " " << HashT[k].name << endl;
	cout << l << " " << HashT[l].id << " " << HashT[l].name << endl;
	cout << m << " " << HashT[m].id << " " << HashT[m].name << endl;
	i = hashSearch(HashT, 6, n);
	if (i != -1)
	{
		cout << i << " " << HashT[i].id << " " << HashT[i].name << endl;
	}
	else cout << "not found";
}
void checkArray(int A[], Entry hashT[]) //checks if all the elements in the array are indexes of occupied slots in the table
{
	for (int i = 0; i < 1500; i++)
	{
		while (hashT[A[i]].id==0) //if not, replaces the element (which represents an index in the hash table)
		{
			if (A[i] == N-1)
				A[i] = 0;
			else A[i]++;
		}
	}
}
void runAverageCase()
{
	int A[1500], n;
	float alfa[5] = { 0.8f, 0.85f ,0.9f ,0.95f ,0.99f };
	Entry hashT[N];
	initHash(hashT, N);
	int fillness = 0;
	float fillingFactor = 0;
	ofstream f;
	f.open("table.txt");
	f << "Filling factor|" << " " << "Avg Effort Found|" << " " << "Max Effort Found|" << " " << "Avg Effort Not-Found|" << " " << "Max Effort Not-Found" << endl;
	for (int i = 0; i < 5; i++) //for each filling factor - alpha
	{
		
		//insert elements in the hashtable
		while (fillingFactor < alfa[i])
		{
			//cout<< "\r" << fillingFactor;
			FillRandomArray(A, 1, 1, 50000, true, 0);
			Entry x;
			x.id = A[0];
			strcpy_s(x.name, "nume");

			if (hashInsert(hashT, x, N) >= 0) //if a new slot is occupied (a new element is inserted)
			{
				fillness++;
			}
			fillingFactor = (float)fillness / N;
		}
		int totalFound = 0, totalNotFound = 0, maxFound = 0, maxNotFound = 0;
		float avgFound, avgNotFound;
		for (int count = 0; count < 5; count++)
		{
			//generate indexes of the 1500 (m/2, m=3000) elements that will surely be found
			FillRandomArray(A, 1500, 0, N - 1, true, 0);
			checkArray(A, hashT);
			//search them
			int j;
			for (j = 0; j < 1500; j++)
			{
				op = 0;
				hashSearch(hashT, hashT[A[j]].id, N);
				if (op > maxFound)
					maxFound = op;
				totalFound += op;
			}
			//generate the m/2 elements that will never be found and search for them
			FillRandomArray(A, 1500, 50001, 100000, true, 0);
			for (j = 0; j < 1500; j++)
			{
				op = 0;
				hashSearch(hashT, A[j], N);
				if (op > maxNotFound)
					maxNotFound = op;
				totalNotFound += op;
			}
		}
		avgFound = (float)totalFound / 1500;
		avgNotFound = (float)totalNotFound / 1500;
		avgFound /= 5;
		avgNotFound /= 5;

		f << setw(10) << alfa[i] << setw(15) << avgFound << setw(16) << maxFound << setw(20) << avgNotFound << setw(22) << maxNotFound << endl;
	}
	f.close();
}
int main()
{
	//test();
	runAverageCase();
	return 0;
}
