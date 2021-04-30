#ifndef LIST_H
#define LIST_H


#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class list{
    private:
        struct Node{
            T data;
            Node* next;
            //first is place for insertion. inserted before first and then first = inserted
            //       inserted_1  --- inserted_2 ---  null_ptr
            //            ^             ^
            //            |             |
            //          first     insertion place(last)        
            Node(const T& element, Node* position)
                :data{element},next{position}{}

        };


        Node* first;//头指针，不是头结点
//        Node* last;//末指针
        int list_size;

        Node* find(const T& element) const{
            Node* temp = first;
            while(temp!=nullptr && temp->data!=element)
                temp=temp->next;
            return temp;                
        };

        Node* get_first() const{
            return this->first;
        };
    
    public:
        list():first{nullptr},list_size{0}{

        }
        

        ~list(){

            while(first!=nullptr){
                Node* temp = first;
                first = first->next;
                delete temp; 
                }

        };

        //从vector数组构建链表
        list(const vector<T>& element_array){
            list_size = element_array.size();

            first = new Node(element_array[0],nullptr);//初始头指针指向第一个元素
            Node* last    = first;//初始末指针指向第一个元素
            Node* temp   = first;

            for(int i=1;i<list_size;i++){
                last =  new Node(element_array[i],nullptr);//末指针指向第i个元素
                temp->next = last;//头指针不变，temp从头指针位置开始不断后移
                temp = last;
            }

        };
        
        //copy constructor
        //新元素插入header前方，头插入
         list(const list& rhs){
            //复制构造对象为空
            if(rhs.list_size==0){
                first = nullptr;
                list_size = 0;
            }
            else{
                list_size = rhs.len();

                Node* temp_rhs = rhs.first;//rhs指示位置指针
                first = new Node(temp_rhs->data,nullptr);//新建list头指针
                Node* last    = first;//新建list末指针   
                Node* temp = first; //新建list指示位置指针

                //对两个list必须有两个不同的指示位置指针temp
                for(int i=1;i<list_size;i++){
                    temp_rhs = temp_rhs->next;//rhs指示位置指针从第一个位置开始不断后移
                    last = new Node(temp_rhs->data,nullptr);//新建list末指针位置
                    temp->next = last;//新建list指示位置指针从头指针开始不断后移
                    temp = temp->next; 
                }
            }

        };

        list& operator=(const list& rhs){

            list temp = list(rhs);
            swap(*this,temp);

            return *this;
        };

        friend void swap(const list& first, const list& second){
            int temp_size     = first.len();
            Node* temp_first   = first.first;
//            Node* temp_last    = first.last;
//析构函数仅仅取决于header指针，故而只要交换header指针即可
            first.list_size  = second.list_size;
            first.first      = second.first;
//            first.last       = second.last;

            second.list_size = temp_size;
            second.first     = temp_first;
//            second.last      = temp_last;
            
        }
        
        //return length of the list
        int len() const{

            return this->list_size;
        };

/*
        Node* get_last() const{
            return this->last;
        };
*/
        bool hasValue(const T& element){
            Node* position = find(element);
            bool flag = (bool)position;

            return flag;
        };

        T& get(const T& element){
            Node* position = find(element);


            return position->data;
        };

        
        //插入到表头
        void insert(const T& element){
            
            if(this==nullptr)
                cout << "need initialization before insert " << endl;

            first = new Node(element,first);
            list_size++;

        };

        void deletion(Node* position){
            //调换position->next与position中的值，删除position->next
            //position不能是最后一个节点
            if(position->next!=nullptr){
                Node* temp = position->next;
                position->data = temp->data;
                position->next  = temp->next;
                delete temp;
            }
            else{
                Node* temp = first;
                //i从1开始到list_size-1结束，list_size-1是倒数第二个节点
                //i=1时,循环内temp赋值到第二个节点.故temp实际指向的节点总是比i多1. 故i最终需要停留在list_size-2
                for(int i=1;i<=list_size-2;i++)
                    temp = temp->next;
                //循环结束后temp正好指向倒数第二个节点
                if(list_size!=1 && temp->next!=position)
                    cout << "error occured in chained-list last node deletion." << endl;
                delete temp->next;//删除最后一个节点
                temp->next = nullptr;
//                last = temp;
            }
            list_size--;
        }

        void deletion(const T element){
            Node* position = find(element);

            if(position!=nullptr){
                deletion(position);
                cout << "found and deleted" << endl;
            }
            else
                cout << "not found" << endl;
        }

        
};


#endif
