#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <string>
#include <iostream>

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
//当然rule of Three/Five 默认需要遵守

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
            //这里不需要对A做A.pop_back()因为sort过程不存在插入，加上pop_back()也不会错误
            max_Heapify(A,1,heapSize);//index 1 corresponds to A[0]
        }
    }
    else if(flag==-1){
        BuildMinHeap(A);
        int len = A.size();

        for(int i=len,heapSize=len;i>=2;i--){//index for counting lies between 1 and len
            swap(A[i-1],A[0]);
            heapSize--;
            //这里不需要对A做A.pop_back()因为sort过程不存在插入,加上pop_back()也不会错误
            min_Heapify(A,1,heapSize);//index 1 corresponds to A[0]
        }        
    }
};

//使用模板类优先队列，需要在自定义T类型内重载> <比较运算符，swap(),赋值运算符(如果T类型内都是内建类型，可以不定义使用默认构造函数即可).定义构造函数T(int n)
template<typename T>
class MaxPriorityQueue{
    
    public:
        MaxPriorityQueue(const vector<T>& A){
            this->Set = A;//vector能使用=赋值
//            Set(A);////不可使用构造函数赋值.vector的源代码里构造函数有explicit关键字，阻止了隐式类型转换
            this->len = A.size();
            BuildMaxHeap(this->Set);
//            cout << "normal constructor of MaxPriorityQueue" << endl;
            
        }
        MaxPriorityQueue(vector<T>&& A){
            len = A.size();
            Set = std::move(A);
            BuildMaxHeap(Set);
//            cout << "move constructor of MaxPriorityQueue" << endl;
        }
        MaxPriorityQueue(const MaxPriorityQueue& rhs){//copy constructor
            Set = rhs.Set;
            len = rhs.len;
//            cout << "copy constructor of MaxPriorityQueue" << endl;
        }
               
        friend void swap(MaxPriorityQueue& first, MaxPriorityQueue& second){

            swap(first.Set,second.Set);
            swap(first.len,second.len);
        };

        MaxPriorityQueue(){//default constructor or you can say empty constructor

            len = 0;
//            cout << "empty constructor of MaxPriorityQueue" << endl;

        }

        MaxPriorityQueue& operator=(const MaxPriorityQueue& rhs){
            MaxPriorityQueue temp(rhs);///////////构造函数调用时类型必须与定义时相同，比如定义const调用时不是const很可能出错
            swap(*this,temp);//temp will be automatically destroyed
//            cout << "copy assignment operator of MaxPriorityQueue" << endl;

            return (*this);
        }; 

        ~MaxPriorityQueue(){
//            cout << "MaxPriorityQueue destructor" << endl;
        }

        void PrintQueue() const{

            for(int i=0;i<this->len;i++)
                cout << this->Set[i] << " ";
                cout << endl;
        }

        T Maximum() const{           
            cout << this->Set[0] << endl;
            return this->Set[0];
        }

        T ExtractMaximum(){
            int heapSize = len;
            if(heapSize < 1)
                cout << "heap underflow";
            T max = this->Set[0];
            this->Set[0] = this->Set[heapSize-1];
            heapSize--;
            //注意，上方heapSize-1意味着vector容器的长度应减少1。同时，剔除的元素与最后一位调换了位置，故而set.popback()即可。如果不执行这一步，剔除的元素仍然在
            //set里，在后续执行insert时将会出问题
            Set.pop_back();
            len--;
            max_Heapify(this->Set,1,heapSize);
//            cout << max << endl;
            return max;
        }

        void SetKey(int i, T key){//index i ranges between 1 and length of the heap
            if(key > this->Set[i-1]){
//                cout << "new key is larger than current key" << endl;
                
//new key is larger than current key. As all element in the heap already stored in max_heap way, we only need to compare it with its parent and parent's parent
                this->Set[i-1] = key;
                HeapNode current = HeapNode(i);
                int parent = current.parent;
                while(current.index > 1 && this->Set[parent-1] < this->Set[current.index-1]){
                    swap(this->Set[parent-1],this->Set[current.index-1]);
                    current = HeapNode(parent);
                    parent  = current.parent;
                }
            }

            else if(key < Set[i-1]){
//                cout << "new key is smaller than current key" << endl;
// new key is smaller than current key. As all element in the heap already stored in max_heap way, we need to compare it with its children and children's children, which is exactly max_heapify
                int heapSize = this->len;
                this->Set[i-1] = key;
                max_Heapify(this->Set,i,heapSize);
            }


        }

        //index i ranges between 1 and length of the heap
        T GetKey(int i){
            return this->Set[i-1];

        }

        void Insert(T key){

            T infinite = T(-10000000000);//构造一个非常小的T,亦即优先级非常低，因为是最大优先队列
            this->Set.push_back(infinite);
            len++;
            int heapSize = len;            
            this->SetKey(heapSize,key);
        }

    private:
        vector<T> Set;
        int len;
       
};
//T can be any type, usually it would be built-in type such as int, float, double, char and so on
//使用模板类优先队列，需要在自定义T类型内重载> <比较运算符，swap(),赋值运算符(如果T类型内都是内建类型，可以不定义使用默认构造函数即可).定义构造函数T(int n){n表示优先级}
template<typename T>
class MinPriorityQueue{
    
    public:
        MinPriorityQueue(const vector<T>& A){//由给定vector<T>构造优先队列
            this->Set = A;//vector能使用=赋值
//            Set(A);//不可使用构造函数赋值.vector的源代码里构造函数有explicit关键字，阻止了隐式类型转换
            this->len = A.size();
            BuildMinHeap(this->Set);
            cout << "normal constructor of MinPriorityQueue" << endl;
            
        }

        MinPriorityQueue(vector<T>&& A){//由给定
            len = A.size();
            Set = std::move(A);
            BuildMinHeap(Set);            
            cout 
            << "move constructor of MinPriorityQueue" << endl;
        }

        MinPriorityQueue(const MinPriorityQueue& rhs){//copy constructor
            Set = rhs.Set;
            len = rhs.len;
            cout << "copy constructor of MinPriorityQueue" << endl;
        }
               
        friend void swap(MinPriorityQueue& first, MinPriorityQueue& second){

            swap(first.Set,second.Set);
            swap(first.len,second.len);
        };

        MinPriorityQueue(){//default constructor or you can say empty constructor

            len = 0;
            cout << "empty constructor of MinPriorityQueue" << endl;

        }

        MinPriorityQueue& operator=(const MinPriorityQueue& rhs){
            MinPriorityQueue temp(rhs);///////////构造函数调用时类型必须与定义时相同，比如定义const调用时不是const很可能出错
            swap(*this,temp);//temp will be automatically destroyed

            cout << "copy assignment operator of MinPriorityQueue" << endl;

            return (*this);
        }; 
        

        ~MinPriorityQueue(){
            cout << "MinPriorityQueue destructor" << endl;
        }

        void PrintQueue() const{

            for(int i=0;i<this->len;i++)
                cout << this->Set[i] << " ";
            cout << endl;
        }

        T Minimum() const{           
            cout << this->Set[0] << endl;
            return this->Set[0];
        }

        T ExtractMinimum(){
            int heapSize = len;
            if(heapSize < 1)
                cout << "heap underflow";
            T min = this->Set[0];
            this->Set[0] = this->Set[heapSize-1];
            heapSize--;
            //注意，上方heapSize-1意味着vector容器的长度应减少1。同时，剔除的元素与最后一位调换了位置，故而set.popback()即可。如果不执行这一步，剔除的元素仍然在
            //set里，在后续执行insert时将会出问题
            Set.pop_back();
            len--;
            min_Heapify(this->Set,1,heapSize);
            cout << min << endl;
            return min;
        }
        //index i ranges between 1 and length of the heap
        void SetKey(int i, T key){
            if(key < this->Set[i-1]){
//                cout << "new key is smaller than current key" << endl;
//new key is smaller than current key. As all element in the heap already stored in min_heap way, we only need to compare it with its parent and parent's parent                
                this->Set[i-1] = key;
                HeapNode current = HeapNode(i);
                int parent = current.parent;
                while(current.index > 1 && this->Set[parent-1] > this->Set[current.index-1]){
                    swap(this->Set[parent-1],this->Set[current.index-1]);
                    current = HeapNode(parent);
                    parent  = current.parent;
                }
            }
            else if(key > Set[i-1]){
//                cout << "new key is larger than current key" << endl;
// new key is larger than current key. As all element in the heap already stored in min_heap way, we need to compare it with its children and children's children, which is exactly min_heapify
                int heapSize = this->len;
                this->Set[i-1] = key;
                min_Heapify(this->Set,i,heapSize);
            }


        }
        //index i ranges between 1 and length of the heap
        T GetKey(int i){
            return this->Set[i-1];

        }

        void Insert(T key){

            long int infinite = 10000000;
            T temp = T(infinite);//构造一个优先级数值非常大的T，亦即优先级非常低，因为是最小优先队列
            this->Set.push_back(infinite);//vector容器承担了扩容申请空间的工作
            len++;
            int heapSize = len;            
            this->SetKey(heapSize,key);
        }

    private:
        vector<T> Set;
        int len;
       
};

#endif
