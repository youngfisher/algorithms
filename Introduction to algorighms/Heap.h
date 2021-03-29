#ifndef HEAP_H
#define HEAP_H

#include <vector>

using namespace std;

void BuildMaxHeap(vector<int> &A);
//i is the index for counting vector A, ranges between 1 and A.size().heapSize <= A.size()
void max_Heapify(vector<int> &A, int i, int heapSize);

void BuildMinHeap(vector<int> &A);
//i is the index for counting vector A, ranges between 1 and A.size().heapSize <= A.size()
void min_Heapify(vector<int> &A, int i, int heapSize);

void HeapSort(vector<int> &A, int flag);//sorting from small to big

#endif
