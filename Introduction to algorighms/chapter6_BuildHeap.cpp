#include <vector>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "Heap.h"

using namespace std;

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

//注释代理类实现，已用模板类代替
/*
class MaxPriorityQueue{
    
    public:
        MaxPriorityQueue(const vector<int>& A){
            this->Set = A;
            this->len = A.size();
            BuildMaxHeap(this->Set);
            cout << "copy constructor of MaxPriorityQueue" << endl;
            
        }
        MaxPriorityQueue(vector<int>&& A){
            len = A.size();
            Set = std::move(A);
            BuildMaxHeap(Set);
            cout << "move constructor of MaxPriorityQueue" << endl;
        }
        ~MaxPriorityQueue(){
            cout << "MaxPriorityQueue destructor" << endl;
        }

        void PrintQueue() const{

            for(int i=0;i<this->len;i++)
                cout << this->Set[i] << " ";
                cout << endl;
        }

        int Maximum() const{           
            cout << this->Set[0] << endl;
            return this->Set[0];
        }

        int ExtractMaximum(){
            int heapSize = len;
            if(heapSize < 1)
                cout << "heap underflow";
            int max = this->Set[0];
            this->Set[0] = this->Set[heapSize-1];
            heapSize--;
            //注意，上方heapSize-1意味着vector容器的长度应减少1。同时，剔除的元素与最后一位调换了位置，故而set.popback()即可。如果不执行这一步，剔除的元素仍然在
            //set里，在后续执行insert时将会出问题
            Set.pop_back();
            max_Heapify(this->Set,1,heapSize);
            cout << max << endl;
            return max;
        }

        void IncreaseKey(int i, int key){//index i ranges between 1 and length of the heap
            if(key < this->Set[i-1]){
                cout << "new key is smaller than current key" << endl;
                return;
            }
            this->Set[i-1] = key;
            HeapNode current = HeapNode(i);
            int parent = current.parent;
            while(current.index > 1 && this->Set[parent-1] < this->Set[current.index-1]){
                swap(this->Set[parent-1],this->Set[current.index-1]);
                current = HeapNode(parent);
                parent  = current.parent;
            }

        }

        void Insert(int key){
            int heapSize = len;
            heapSize++;
            this->Set.push_back(-1000);
            this->IncreaseKey(heapSize,key);
        }

    private:
        vector<int> Set;
        int len;
       
};

class MinPriorityQueue{
    
    public:
        MinPriorityQueue(const vector<int>& A){
            this->Set = A;
            this->len = A.size();
            BuildMinHeap(this->Set);
            cout << "copy constructor of MinPriorityQueue" << endl;
            
        }
        MinPriorityQueue(vector<int>&& A){
            len = A.size();
            Set = std::move(A);
            BuildMinHeap(Set);            
            cout << "move constructor of MinPriorityQueue" << endl;
        }
        ~MinPriorityQueue(){
            cout << "MinPriorityQueue destructor" << endl;
        }

        void PrintQueue() const{

            for(int i=0;i<this->len;i++)
                cout << this->Set[i] << " ";
                cout << endl;
        }

        int Minimum() const{           
            cout << this->Set[0] << endl;
            return this->Set[0];
        }

        int ExtractMinimum(){
            int heapSize = len;
            if(heapSize < 1)
                cout << "heap underflow";
            int min = this->Set[0];
            this->Set[0] = this->Set[heapSize-1];
            heapSize--;
            //注意，上方heapSize-1意味着vector容器的长度应减少1。同时，剔除的元素与最后一位调换了位置，故而set.popback()即可。如果不执行这一步，剔除的元素仍然在
            //set里，在后续执行insert时将会出问题
            Set.pop_back();
            min_Heapify(this->Set,1,heapSize);
            cout << min << endl;
            return min;
        }

        void DecreaseKey(int i, int key){//index i ranges between 1 and length of the heap
            if(key > this->Set[i-1]){
                cout << "new key is larger than current key" << endl;
                return;
            }

            this->Set[i-1] = key;
            HeapNode current = HeapNode(i);
            int parent = current.parent;
            while(current.index > 1 && this->Set[parent-1] > this->Set[current.index-1]){
                swap(this->Set[parent-1],this->Set[current.index-1]);
                current = HeapNode(parent);
                parent  = current.parent;
            }

        }

        void Insert(int key){
            int heapSize = len;
            heapSize++;
            this->Set.push_back(1000000000);//vector容器承担了扩容申请空间的工作
            this->DecreaseKey(heapSize,key);
        }

    private:
        vector<int> Set;
        int len;
       
};

PriorityQueue::PriorityQueue(const vector<int>& A, string queueType){
    if(queueType == "max"){
        maxQueue = new MaxPriorityQueue(A);//must use new to allocate new memory
        cout << "copy constructor of PriorityQueue" << endl;
    }
    else if(queueType == "min"){
        minQueue = new MinPriorityQueue(A);
        cout << "copy constructor of PriorityQueue" << endl;
    }
}

PriorityQueue::PriorityQueue(vector<int>&& A, string queueType){
    if(queueType == "max"){
        maxQueue = new MaxPriorityQueue(std::move(A));
        cout << "move constructor of PriorityQueue" << endl;
    }
    else if(queueType == "min"){
        minQueue = new MinPriorityQueue(std::move(A));
        cout << "move constructor of PriorityQueue" << endl;
    }
}


PriorityQueue::~PriorityQueue(){
    if(maxQueue){
        delete maxQueue;
        cout << "PriorityQueue destructor" << endl;
        maxQueue = nullptr;
    }
    if(minQueue){
        delete minQueue;
        cout << "PriorityQueue destructor" << endl;
        minQueue = nullptr;
    }
}

void PriorityQueue::PrintQueue() const{
    if(maxQueue)
        maxQueue->PrintQueue();
    else if(minQueue)
        minQueue->PrintQueue();

}

void PriorityQueue::Insert(int key){
    if(maxQueue)
        maxQueue->Insert(key);
    else if(minQueue)
        minQueue->Insert(key);

}

//only apply to MinPriorityQueue
void PriorityQueue::DecreaseKey(int i, int key){
    
    if(!minQueue){
        cout << "initialize minPriorityQueue first" << endl;
        return;
    }

    minQueue->DecreaseKey(i,key);
};

int PriorityQueue::ExtractMinimum(){

    if(!minQueue){
        cout << "initialize minPriorityQueue first" << endl;

        return 0;
    }

    return minQueue->ExtractMinimum();

};

int PriorityQueue::Minimum() const{

    if(!minQueue){
        cout << "initialize minPriorityQueue first" << endl;
        return 0;
    }

    return minQueue->Minimum();

};

//only apply to MaxPriorityQueue
void PriorityQueue::IncreaseKey(int i, int key){

    if(!maxQueue){
        cout << "initialize maxPriorityQueue first" << endl;
        return;
    }

    maxQueue->IncreaseKey(i,key);

};

int PriorityQueue::ExtractMaximum(){

    if(!maxQueue){
        cout << "initialize maxPriorityQueue first" << endl;
        return 0;
    }

    return maxQueue->ExtractMaximum();

};

int PriorityQueue::Maximum() const{

    if(!maxQueue){
        cout << "initialize maxPriorityQueue first" << endl;
        return 0;
    }

    return maxQueue->Maximum();

};

*/
///////////////////////////////////////////////////////////////////////////////////////////////////
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
