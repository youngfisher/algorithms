#include <vector>

#include "Heap.h"

using namespace std;

void HeapSort(vector<int> &A, int flag){//flag==1, ascend order. flag==-1,descend order.

    if(flag==1){
        BuildMaxHeap(A);
        int len = A.size();

        for(int i=len,heapSize=len;i>=2;i--){//index for counting lies between 1 and len
            swap(A[i-1],A[0]);
            heapSize--;
            //这里不需要对A做A.pop_back()因为不存在插入
            max_Heapify(A,1,heapSize);//index 1 corresponds to A[0]
        }
    }
    else if(flag==-1){
        BuildMinHeap(A);
        int len = A.size();

        for(int i=len,heapSize=len;i>=2;i--){//index for counting lies between 1 and len
            swap(A[i-1],A[0]);
            heapSize--;
            //这里不需要对A做A.pop_back()因为不存在插入
            min_Heapify(A,1,heapSize);//index 1 corresponds to A[0]
        }        
    }

}
