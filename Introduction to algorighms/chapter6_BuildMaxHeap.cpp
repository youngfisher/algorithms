#include <vector>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "BuildMaxHeap.h"

using namespace std;

struct HeapNode{
    int index;
    int left;
    int right;

    HeapNode(const int i)
        :index{i},left{2*i},right{2*i+1}{}

};

void max_Heapify(vector<int> &A, int i, int heapSize){

/*
///recursive implementation///////////////////////////

    HeapNode current = HeapNode(i);
    int lt = current.left;
    int rt = current.right;

    int largest = i;

    if(lt <= heapSize && A[lt-1] > A[i-1])//HeapNode is defined with staring index 1.
        largest = lt;

    if(rt <= heapSize && A[rt-1] > A[largest-1])
        largest = rt;

    if(largest != i){
        int temp = A[i-1];
        A[i-1] = A[largest-1];
        A[largest-1] = temp;

        max_Heapify(A, largest,heapSize);//must be inside this if 
    }
////////////////////////////////////////////////////////////////
 */   

//////////////non-recursive implementation/////////////////////
    
    //when largest == i, means that current node is larger than its child. break the loop.
    for(int largest=0;largest != i;){//i starts from 1, thus the first comparison will always pass.

        HeapNode current = HeapNode(i);
        int lt = current.left;
        int rt = current.right;

        if(lt > heapSize && rt > heapSize)//the current node is a leaf
            break;

        largest = lt;        

        if(lt <= heapSize && A[lt-1] < A[i-1])
            largest = i;

        if(rt <= heapSize && A[rt-1] > A[largest-1])
            largest = rt;

        if(largest != i){
            //swap value in A[i-1] and A[largest-1]
            int temp = A[i-1];
            A[i-1] = A[largest-1];
            A[largest-1] = temp;
            
            //assign the index of largest value to i and operate on child node.
            i = largest;
            largest = 0;//to make sure next for loop test pass.

        }
    }
//////////////////////////////////////////////////////////////

}

void BuildMaxHeap(vector<int> &A){
    int len = A.size();
    int heapSize = len;
    for(int i = len/2 ;i >= 1;i--)
        max_Heapify(A,i,heapSize);
}
