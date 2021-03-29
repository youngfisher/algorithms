#include <vector>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "Heap.h"

using namespace std;

struct HeapNode{
    int index;
    int left;
    int right;
    int parent;

    HeapNode(const int i)
        :index{i},left{2*i},right{2*i+1},parent{i/2}{}

};

void swap(int &a, int &b){
    int temp = a;
    a = b;
    b = temp;
}


void min_Heapify(vector<int> &A, int i, int heapSize){//index i lies between 1 and length of A
/*
///recursive implementation///////////////////////////

    HeapNode current = HeapNode(i);
    int lt = current.left;
    int rt = current.right;

    int smallest = i;

    if(lt <= heapSize && A[lt-1] < A[i-1])//HeapNode is defined with staring index 1.
        smallest = lt;

    if(rt <= heapSize && A[rt-1] < A[smallest-1])
        smallest = rt;

    if(smallest != i){
        int temp = A[i-1];
        A[i-1] = A[smallest-1];
        A[smallest-1] = temp;

        min_Heapify(A, smallest,heapSize);//must be inside this if 
    }
////////////////////////////////////////////////////////////////
 */   

//////////////non-recursive implementation/////////////////////
    
    //when smallest == i, means that current node is smaller than its child. break the loop.
    for(int smallest=0;smallest != i;){//i starts from 1, thus the first comparison will always pass.

        HeapNode current = HeapNode(i);
        int lt = current.left;
        int rt = current.right;

        if(lt > heapSize && rt > heapSize)//the current node is a leaf
            break;

        smallest = lt;        

        if(lt <= heapSize && A[lt-1] > A[i-1])
            smallest = i;

        if(rt <= heapSize && A[rt-1] < A[smallest-1])
            smallest = rt;

        if(smallest != i){
            //swap value in A[i-1] and A[smallest-1]
            swap(A[i-1],A[smallest-1]);

            //assign the index of smallest value to i and operate on child node.
            i = smallest;
            smallest = 0;//to make sure next for loop test pass.

        }
    }
//////////////////////////////////////////////////////////////

}

void max_Heapify(vector<int> &A, int i, int heapSize){//index i lies between 1 and length of A

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
            swap(A[i-1],A[largest-1]);

            //assign the index of largest value to i and operate on child node.
            i = largest;
            largest = 0;//to make sure next for loop test pass.

        }
    }
//////////////////////////////////////////////////////////////

}

void BuildMinHeap(vector<int> &A){
    int len = A.size();
    int heapSize = len;
    for(int i = len/2 ;i >= 1;i--)
        min_Heapify(A,i,heapSize);
}

void BuildMaxHeap(vector<int> &A){
    int len = A.size();
    int heapSize = len;
    for(int i = len/2 ;i >= 1;i--)
        max_Heapify(A,i,heapSize);
}

/*
int main(int argc, char** argv){
    
    vector<int> input;
    int c;
    while((c=getchar())!='\n'){
        ungetc(c,stdin);//direct to input iostream
        cin >> c;
        input.push_back(c);
    }
    
    cout << "before call function: " ;
    int len = input.size();
    for(int i=0;i<len;i++)
        cout << input[i] << " " ;
    cout << endl;

    BuildMaxHeap(input);

    cout << "after call function: " ;
    for(int i=0;i<len;i++)
        cout << input[i] << " " ;
    cout << endl;

}
*/
