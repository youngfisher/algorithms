//implement random(a,b) with random(0,1)

//main idea is to use random(0,1) to generate each bit of a binary number, which is then transformed to decimal integer

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <time.h>
#include "chapter5_1_2.h" 

using namespace std;

int rand01(){
    return rand()%2;
}

int randomized(int leftest,int binary[]){
    int count = 0;
    for(int i=0;i<=leftest;i++){
        binary[i] = rand01();
        count += binary[i]*pow(2,i);   
    }
    return count;
}

int random(int a, int b){

    srand((unsigned)time(NULL));//set seed for rand()

    int interval = b - a;
    if(interval<=1)
        return rand01()*interval + a;

    //get the leftest bit index of the binary number that is big enough to hold the decimal interval.
    int leftest_bit = 0;
    for(int quotient=interval;quotient>1;leftest_bit++)
        quotient = quotient/2;

    int binary[100];//store each bit of the binary number
    
    //calculate the randomized number. which should be between 0 and 2^m. 2^m is the nearest one larger than interval in its series.
    //m equals leftest_bit
    int result = randomized(leftest_bit,binary);
    //randomized result has probability bigger than b - a, if so do randomied again.
    while(result>interval)
        result = randomized(leftest_bit,binary);
    
    result = result + a;
    return result;
}
/*
int main(){

 cout << random(4,11)<< endl;
}
*/
