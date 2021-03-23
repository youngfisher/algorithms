#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int candy(vector<int>& ratings) {
        
//    vector<int> ratings = {1,2,2};
    int len = ratings.size();
    vector<int> count(len,1);


    for(int i=1;i<len;i++){

        if(ratings[i-1]<ratings[i])
            count[i] = 1 + count[i-1];            

    }

    for(int i=len-1;i>0;i--){
        if(ratings[i]<ratings[i-1])
            count[i-1] = max(count[i]+1,count[i-1]);
    }


    int result = accumulate(count.begin(),count.end(),0); 
        
    return result;
        
    }
/*
int main(int argc, char** argv){

    vector<int> ratings = {1,2,2};

    int result = candy(ratings);

    cout << result << endl;

    return 0;
}
*/
