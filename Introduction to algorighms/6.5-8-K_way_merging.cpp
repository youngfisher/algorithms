#include <vector>
#include <iostream>
#include "K_way_merging.h"
#include "Heap.h"

using namespace std;

//sorted can be considered as a simple chain-list

//rule of threee
sorted::sorted(const vector<int>& A){
    len = A.size();
    header = new int[len];
    current = header;

    for(int i=0;i<len;i++)
        *(header+i) = A[i];
}

sorted::sorted(const sorted& rhs){
    len = rhs.len;
    header = new int[len];
    current = header + (rhs.current - rhs.header);//attention here. can't directly assign current=header as in input[0] = input[heapSize-1]; current is different from header

    for(int i=0;i<len;i++)
        *(header+i) = *(rhs.header+i);

}

sorted::~sorted(){
    delete[] header;
    header = nullptr;
    cout << "sorted destructor" << endl;
}

//define compare behavior of sorted
bool sorted::operator<(const sorted& b){
    return *(this->current) < *(b.current);
}

bool sorted::operator>(const sorted& b){
    return *(this->current) > *(b.current);
}

void swap(sorted& first, sorted& second){
    swap(first.header,second.header);
    swap(first.current,second.current);
    swap(first.len,second.len);
};

sorted& sorted::operator=(const sorted& rhs){
    sorted temp(rhs);
    swap(*this,temp);//temp will be automatically destroyed

    return (*this);
}

int sorted::print(){
    return *current;
}

int sorted::getValue(){
    return *current;
}

void sorted::goNext(){
    current++;
}

bool sorted::noDataLeft(){

    return ((current - header)==len);
};



vector<int> K_way_merging(vector<sorted>& input){//the K sorted arrays have to be processed first into a self-defined structure sorted. sorted has three elements.

//the main idea. build a k node min-heap(or max-heap) before selecting the minimum(or maximum) in each iteration, 
//after which another number from THE sorted array the minimum(or maximum) locates in last iteration fulfill the vacuum, until no element left in all the sorted arrays.
//each node is a sorted array. There are k nodes. each iteration remove the smallest data in the k nodes. the one next to it in that array replaces it.
//total n elements, each element in the heap cost O(lgk) time. Thus a total O(n*lgk) 

    int k = input.size();//length of the vector is the number of k sorted arrays

    BuildMinHeap(input);

    int n;//total data number

    for(int i=0;i<k;i++){
        n += input[i].len;
    }

    int heapSize = k;

    vector<int> result(n,0);

    //iteration for sorting
    for(int min, i=0;i<n;i++){
        min = input[0].getValue();
        result[i] = min;
        input[0].goNext();
        if(!input[0].noDataLeft())
            min_Heapify(input,1,heapSize);
        else{
            input[0] = input[heapSize-1];//when input[0] has no data left, exchange input[0] with input[heapSize-1] and decrease the heap node by 1
            heapSize--;
            min_Heapify(input,1,heapSize);
        }
        cout << min << " ";  
    }

    return result;

}
/*
int main(){

    vector<int> A = {1,2,4,6,7};
    vector<int> B = {3,5,6,8,10,11};
    vector<int> C = {9,12,15,19,32};
    vector<int> D = {13,14,20,25};


    vector<sorted> input = {sorted(A),sorted(B),sorted(C),sorted(D)};

    vector<int> test = K_way_merging(input);
    
    return 0;
}
*/
