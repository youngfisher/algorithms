#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <string>

using namespace std;

void BuildMaxHeap(vector<int> &A);
//i is the index for counting vector A, ranges between 1 and A.size().heapSize <= A.size()
void max_Heapify(vector<int> &A, int i, int heapSize);

void BuildMinHeap(vector<int> &A);
//i is the index for counting vector A, ranges between 1 and A.size().heapSize <= A.size()
void min_Heapify(vector<int> &A, int i, int heapSize);

void HeapSort(vector<int> &A, int flag);//sorting from small to big if flag == 1. flag == -1 means the opposite

class MaxPriorityQueue;
class MinPriorityQueue;

class PriorityQueue{//surrogate class to hide information of true class MaxPriorityQueue and MinPriorityQueue
    public:
        PriorityQueue(const vector<int>& A, string queueType);//queueType == "max" or "min"
        PriorityQueue(vector<int>&& A, string queueType);
        ~PriorityQueue();
          
        //shared functions
        void PrintQueue() const;
        void Insert(int key);
        
        //only apply to MinPriorityQueue
        void DecreaseKey(int i, int key);
        int ExtractMinimum();
        int Minimum() const;

        //only apply to MaxPriorityQueue
        void IncreaseKey(int i, int key);
        int ExtractMaximum();
        int Maximum() const;        

    private:
        MaxPriorityQueue* maxQueue{nullptr};
        MinPriorityQueue* minQueue{nullptr};

};

#endif
