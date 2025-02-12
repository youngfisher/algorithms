#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include "Heap.h"

//如果要实现放不同类型变量的Queue或者Stack，需要先设计一个基类，然后从基类衍生出一些列扩展类。根据队列或者栈的构造函数参数决定调用哪一个扩展类。队列或栈内放置基类

/*
//serve as base class for a buch of derived ones like Integer, Integer*
class Value{

};
*/

//use PriorityQueue to implement a Queue data structure. First In First Out.
//使用优先队列实现队列数据结构
//This program only implements a Queus that stores one data type
//该队列只能保存同种类型数据
using namespace std;
//implemented with heap structure, not linked-list


//实现思路如下：假定要入队列元素为类型T1，第一个T1数据入队列时赋予序号1，依次类推，第n个T1数据入队列时赋予序号n。每次元素入队列时，以当前元素的序号值置于最小优先队列MinPriorityQueue中。很明显，先入队列序号小，在优先队列前方。出队列时，则用优先队列的Extract-min函数。这样就实现了
//先入现出。赋予的序号值即可认为是优先级，越小优先级越高。很明显，不仅要存储序号值，还要存储元素本来的值，故需要自定义数据类型T2，包含这两个变量。
//Stack实现与Queue实现思路一致，序号赋值部分相同，不同之处在于使用最大优先队列。后入栈的序号最大，使用最大优先队列就获得了最高优先级
//该队列运行一定时间如几天或者半月，一月后需要重新做一次复制，清零优先级计数
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
            //新写法，考虑到了复制rhs时，对rhs内优先级的数值做一次瘦身。这是因为队列的实现方式导致里面的优先级数据只会不断增加，这里通过复制将优先级数据从1开始重新计数
            size = rhs.size();
            for(int i=1;i<=size;i++){//从序号1开始遍历rhs
                Set.Insert(data(i,rhs.Set[i-1].value));//i为优先级，rhs.Set[i-1].value 为原队列中第i个的data元素的值.
            }

            //原来的写法，直接复制了rhs的内容
            //size = rhs.size;
            //Set = rhs.Set;
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

        void PrintQueue() const{

            Set.PrintQueue();

        }
        //这个队列内优先级的赋值会不断增加，如果长期运行可能有问题.最好在某个时间清零队列一次或者重新赋值一次，参见上面的赋值构造函数
        void Enqueue(T v){//入队列，未指定优先级，常规用法

        //原本简单写法，直接把当前入队列优先级设置为队列长度加1.问题在于忽略了出队列导致的队列长度与优先级不匹配
        //当前长度加出队列次数才是队列内最后一个元素的优先级
        //当前元素优先级等于最后一个元素的优先级+1
            size++;
            int current_priority = size + dequeue_count;
            Set.Insert(data(current_priority,v));

        };

        void Enqueue(int priority, T v){//入队列，指定优先级priority
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
        T   Dequeue(){
            if(size==0){
                cout << "Queue is empty. Can't Dequeue." << endl;
                return -1;
            }
            size--;
            dequeue_count ++;
            if(size==0)
                dequeue_count = 0;
            return Set.ExtractMinimum().value;
        };

    private:

        int size;//该变量保存队列当前大小
        MinPriorityQueue<data> Set;//
        int dequeue_count = 0;
    
};


#endif
