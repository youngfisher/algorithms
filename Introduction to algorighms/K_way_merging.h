#ifndef K_WAY_MERGING_H
#define K_WAY_MERGING_H

#include <vector>

using namespace std;

class sorted{//this is the self-defined structure to store data. header is an int* pointer pointed to the staring address of a sorted array while len is the length of it.
    public:
        int* header;//beginning address of a sorted array
        int* current;//current position in this array. It is equal to header at the start.
        int len;//length of this array
//rule of threee
        explicit sorted(const vector<int>& A);
        sorted(const sorted& rhs);
        ~sorted();


        //define compare behavior of sorted
        bool operator<(const sorted& b);
        
        bool operator>(const sorted& b);
        
        friend void swap(sorted& first, sorted& second);

        sorted& operator=(const sorted& rhs);

        int print();
        int getValue();
        void goNext();

        bool noDataLeft();

};

vector<int> K_way_merging(vector<sorted>& input);

#endif
