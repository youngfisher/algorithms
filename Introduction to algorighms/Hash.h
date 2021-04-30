// this hash class does not implement copy constructor, copy assignment.
//support [] assignment like normal array. similar to usage in python.

#ifndef HASH_H
#define HASH_H

#include <vector>
#include <string>
#include <iostream>
#include "list.h"

using namespace std;

template<typename T>
class Hash{

    private:

        struct Paired{
            string key;
            T      value;

            Paired(string k, T val):key{k},value{val}{}
            //use string only to initialize
            Paired(string k):key{k}{}

            bool operator==(const Paired& rhs){
                //when searching Paired data in the list, same key means the same Paired data.
                return key==rhs.key;
            }

            bool operator!=(const Paired& rhs){
                //when searching Paired data in the list, same key means the same Paired data.
                return key!=rhs.key;                
            }


        };

        //both key and value need to be stored in the chain-list
        vector<list<Paired>> set;
        const int p = 14; //significant_bit
        int table_size;

    
    public:
        //使用算法导论p231-p232乘法作为hash函数计算方式.该方法的计算过程采用2个32(w)位数相乘(64位机器)，最后结果又取小数部分32位数的从左至右p位有效数字。具体算法请详见算法导论.
        //取p位有效数字导致key的取值范围为[0,2^p-1]. table_size上限则为2^p.
        //故而下方直接将hash表大小设置为2^p.实际可以修改成任何值.程序采用动态扩容.    
        Hash(){
            int initial_size = (1 << p);//左移p位即2^p
            vector<list<Paired>> temp(initial_size);
            set.swap(temp);
            table_size = set.size();

        } 


        unsigned int hashFunction(string& key){

            //step1. use Horner rule to calculate k.
            unsigned long long k = 0;
            for(int i=0;i<key.length();i++){
                k = (k << 3) + key[i];
            }
            k = k % table_size;

            //step2. use multiplication method from CLRS.
            unsigned int m = (1 << p);
            double A = 0.6180339887;//(sqrt(5)-1)/2. 
            unsigned long long s = 2654435769;//s = (A << 32)

            unsigned long long Y = ((unsigned long long)1<<32)-1;
            unsigned int r0 = (k*s)& Y;//r0 = k*s%2^32.取余公式 X & (2^N-1)

            unsigned int result = (r0 >> (32-p));//右移32-p位，即可获得左侧p位的有效值
            
            //hash值大于表容量上限.扩容后容量1.5*hash值
            if(result > table_size){
                table_size = 1.5*result;
                set.resize(table_size);
            }

            return result; 

        };
        
        //here & is essential for the purpose of l-value reference, without which you would not be able to use test["abc"] = 1;
        T& operator[](const char* ch){

            string key(ch);

            unsigned int hashVal = hashFunction(key);
//            list<T> current_list = set[hashVal];

            bool found = set[hashVal].hasValue(Paired(key));
            //key found
            if(found){
                cout << "key found." << endl;
                return set[hashVal].get(Paired(key)).value;
            }
            else{
                cout << "key not found. Creat now." << endl;
                set[hashVal].insert(Paired(key));
                return set[hashVal].get(Paired(key)).value;
            }
                
        }

        void deletion(const char* ch){
            string key(ch);
            unsigned int hashVal = hashFunction(key);

            bool found = set[hashVal].hasValue(Paired(key));

            if(found){
                set[hashVal].deletion(Paired(key));
            }

        };

};

#endif
