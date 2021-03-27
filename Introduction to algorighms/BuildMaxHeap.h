#ifndef BUILDMAXHEAP_H
#define BUILDMAXHEAP_H

void BuildMaxHeap(vector<int> &A);
//i is the index for counting vector A, ranges between 1 and A.size().heapSize <= A.size()
void max_Heapify(vector<int> &A, int i, int heapSize);


#endif
