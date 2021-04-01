#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <string>

using namespace std;

struct HeapNode{
    int index;
    int left;
    int right;
    int parent;

    HeapNode(const int i)
        :index{i},left{2*i},right{2*i+1},parent{i/2}{}

};


void BuildMaxHeap(vector<int> &A);
//i is the index for counting vector A, ranges between 1 and A.size().heapSize <= A.size()

void max_Heapify(vector<int> &A, int i, int heapSize);


void BuildMinHeap(vector<int> &A);
//i is the index for counting vector A, ranges between 1 and A.size().heapSize <= A.size()

void min_Heapify(vector<int> &A, int i, int heapSize);

void HeapSort(vector<int> &A, int flag);//sorting from small to big if flag == 1. flag == -1 means the opposite

//!!!!!!!!attention
//don't use copy constructor, move constructor, copy assignment operator, move assignment operator on MaxPriorityQueue, MinPriorityQueue and PriorityQueue
class MaxPriorityQueue;
class MinPriorityQueue;

class PriorityQueue{//surrogate class to hide information of MaxPriorityQueue and MinPriorityQueue
    public:
        PriorityQueue(const vector<int>& A, string queueType);//queueType == "max" or "min"
        PriorityQueue(vector<int>&& A, string queueType);
//        PriorityQueue(const PriorityQueue& rhs);
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
