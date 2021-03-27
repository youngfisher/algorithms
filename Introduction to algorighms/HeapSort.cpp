#include <vector>
#include "HeapSort.h"
#include "BuildMaxHeap.h"

using namespace std;

void HeapSort(vector<int> &A){
    BuildMaxHeap(A);
    int len = A.size();

    for(int i=len,heapSize=len;i>=2;i--){//index for counting lies between 1 and len
        swap(A[i-1],A[0]);
        heapSize--;
        max_Heapify(A,1,heapSize);//index 1 corresponds to A[0]
    }

}
