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

//已用模板类代替
/*
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
*/



//as I found that those functions and classes keeps involved in other algorithm with different vector<T>, here comes the template version.
//上述函数与类不断出现在其他算法应用中，使用不同的vector<T>
//template definition of above function and class. PriorityQueue excluded since surrogate class becomes meaningless.
//每次都要修改类型很麻烦，故在此给出上述函数与类的模板定义.由于模板的声明与定义不可分离，代理类变得没有意义

//to use template version of those functions and classes, in addition to define a class T one has to provide three overloaded functions: 
//bool T::operator<(const T& b)
//bool T::operator>(const T& b)
//void swap(T& first, T& second)
//使用模板类型，定义自己的类型T时，要注意重载三个函数

template<typename T>
void BuildMaxHeap(vector<T> &A){
    int len = A.size();
    int heapSize = len;
    for(int i = len/2 ;i >= 1;i--)
        max_Heapify(A,i,heapSize);
};

//i is the index for counting vector A, ranges between 1 and A.size().heapSize <= A.size()
template<typename T>
void max_Heapify(vector<T> &A, int i, int heapSize){
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

};

template<typename T>
void BuildMinHeap(vector<T> &A){
    int len = A.size();
    int heapSize = len;
    for(int i = len/2 ;i >= 1;i--)
        min_Heapify(A,i,heapSize);
};

//i is the index for counting vector A, ranges between 1 and A.size().heapSize <= A.size()
template<typename T>
void min_Heapify(vector<T> &A, int i, int heapSize){
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
};

template<typename T>
void HeapSort(vector<T> &A, int flag){//sorting from small to big if flag == 1. flag == -1 means the opposite
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
};

template<typename T>
class MaxPriorityQueue{
    
    public:
        MaxPriorityQueue(const vector<T>& A){
            this->Set = A;
            this->len = A.size();
            BuildMaxHeap(this->Set);
            cout << "copy constructor of MaxPriorityQueue" << endl;
            
        }
        MaxPriorityQueue(vector<T>&& A){
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
        vector<T> Set;
        int len;
       
};

template<typename T>
class MinPriorityQueue{
    
    public:
        MinPriorityQueue(const vector<T>& A){
            this->Set = A;
            this->len = A.size();
            BuildMinHeap(this->Set);
            cout << "copy constructor of MinPriorityQueue" << endl;
            
        }
        MinPriorityQueue(vector<T>&& A){
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
        vector<T> Set;
        int len;
       
};

#endif
