//debug purpose

#include "Hash.h"
#include <typeinfo>


int main(){

    Hash<int> test;

    test["good"] = 1;
    test["good"] = 2;

    int c = test["good"];

    test.deletion("good");

    string character = "this is a test";
    cout << int(character[0]) << endl;

    test.hashFunction(character);

    
    vector<int> t1(10);

    vector<int> a;         
    vector<int> b(a);

    vector<list<int>> t0;
    vector<list<int>> t10(t0);

    vector<int> t11;
    vector<int> t12={4,5,6,7,8,9};
    cout<<t11.capacity()<<" "<<t12.capacity()<<endl;
    t11.swap(t12);
    cout<<t11.capacity()<<" "<<t12.capacity()<<endl;

    list<int> t2;
    t0.push_back(t2);
    cout<<t0.capacity()<<" "<<t10.capacity()<<endl;   
    t0.swap(t10);
    cout<<t0.capacity()<<" "<<t10.capacity()<<endl; 

    vector<int> A = {1,2,3};


    list<int> t3 ;
    vector<list<int>*> t20(10,nullptr);//在这个测试中，t20初始化为10个list<int>* 指针，赋值为nullptr.
    //vector<list<int>*> t20(10,nullptr) = new list<int>[10];//错误写法
    vector<list<int>> t21(10);



    t21[0].insert(50);
//    t20[0] = new list<int>;//正确写法,如果没有该句，下面insert将报错.任何使用指针的场合，都需要在同scope里有new和delete
//    t20[0]->insert(50);//t20[0]也就是第一个指针，为nullptr.但该nullptr指向的东西不存在，或者说压根就没有任何指向.作为对比，可以查看t21的内容，从t21[0]开始每一个已经初始化，其初始化的内容正是nullptr.
                       //因此必须先用new申请空间初始化才能继续

//    t20[0] = &t3;
    
//    t3.insert(50);
    t21[0].insert(100);
    t21[0].deletion(50);

    list<int>* p = &t3;

    vector<list<int>> t4;

    t4.push_back(t3);

 
    return 0;
}


