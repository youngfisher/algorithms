# ifndef STACK_H
# define STACK_H

#include <vector>
#include "Heap.h"

using namespace std;


//参见Queue获得注释说明

//Stack实现与Queue实现思路一致，序号赋值部分相同，不同之处在于使用最大优先队列。后入栈的序号最大，使用最大优先队列就获得了最高优先级

template<typename T>
class Queue{

    private:

        class data{
    
            public:
                int priority_level;//store value from 1 to n representing its priority
                T value;//the real value that needs to store in the queue. T usually would be int, or double

                data(const int i, T v){
                    priority_level = i;
                    value = v;
                }

                data(const long int i){
                    priority_level = i;
                }

                data(const data* rhs){
                    priority_level = rhs->priority_level;
                    value = rhs->value;
                }
                
                bool operator<(const data& rhs){
                    return priority_level < rhs.priority_level;
                }

                bool operator>(const data& rhs){
                    return priority_level > rhs.priority_level;
                }

                friend void swap(data& first, data& second){
                    swap(first.priority_level,second.priority_level);
                    swap(first.value,second.value);
                } 

                data& operator=(const data& rhs){
                    data temp(rhs);
                    swap(*this,temp);//temp will be automatically destroyed

                    return (*this);
                } 

                ~data(){

                }

                friend ostream& operator<<(ostream& os, const data& item){
                    os << "priority: " << item.priority_level << " value: " << item.value << endl;
                    return os;
                }
        };


    public:
    
        
        Queue(const vector<T>& A){
            size = A.size();
            for(int i=1;i<=size;i++){//从序号1开始遍历vector
                Set.Insert(data(i,A[i-1]));//i为优先级，A[i-1]为当前优先级对应的实际值

            }
        };
        //rule of Three
        Queue(const Queue& rhs){
            size = rhs.size;
            Set = rhs.Set;
        };

        Queue(){
            size = 0;

        };
        ~Queue(){

        };

        friend void swap(Queue& first, Queue& second){
            swap(first.size,second.size);
            swap(first.Set,second.Set);
        }

        Queue& operator=(const Queue& rhs){
            Queue temp(rhs);
            swap(*this,temp);

            return *this;
        };

        void PrintStack() const{

            Set.PrintQueue();

        }

        void Push(T v){//入栈，未指定优先级，常规用法
            size++;//队列长+1，当前元素优先级等于队列长度
            Set.Insert(data(size,v));

        };

        void Push(int priority, T v){//入栈，指定优先级priority
            size++;//队列长+1
            Set.Insert(data(priority,v));

        };
//position is the place in the Queue, ranging from [1,size of Queue]. DesiredPriority is the priorty we want to set to the position,ranging from [1,size of Queue]
        //return value in the format of(priority,value). can use << to check the output
        data SetPriority(int DesiredPriority, int position){
            T v = Set.GetKey(position).value;
            Set.SetKey(position,data(DesiredPriority,v));

            return Set.GetKey(position);

        }
        //return value in the format of(priority,value).can use << to check the output
        data  GetPriority(int position){
            return Set.GetKey(position);
        }

        //return value is the value of the Dequeued
        T   Pop(){
            if(size==0){
                cout << "Stack is empty. Can't Pop." << endl;
                return -1;
            }
            size--;
            return Set.ExtractMaximum().value;
        };

    private:

        int size;//该变量保存队列当前大小
        MaxPriorityQueue<data> Set;//
    
};


# endif