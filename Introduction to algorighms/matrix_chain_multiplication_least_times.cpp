#include <iostream>
#include <time.h>
#include <vector>
#include <stdio.h>
#include "matrix_chain_multiplication_least_times.h"
using namespace std;

int matrix_chain_multiplication(vector<int>& input){

//    vector<int> input;
    clock_t start,end,time;
    start = clock();


    int number = input.size();

//////////////////////input the dimension of all the matrix. p0,p1,p2,p3,...,pn

    //number of input matrix
    int n = number - 1;
    int m[number][number];
    int s[number][number];

//matirx chain start at i and ends at j.  m[i][j] store the cost to calculate the chain between i and j. thus m[i][i]=0     
    for(int i=1;i<=n;i++)
        m[i][i] = 0;
    
    int cost;
    int j=0;
    //l is the length of the chain
    for(int l=2;l<=n;l++){//for a given input matrix number n, the multiplication chain varies between 2 and n, excluding l=1
        for(int i=1;i<=n-l+1;i++){//chain starts at i. for a given chain length l, location of i may vary from 1 to n-l+1
            j=i+l-1;//for a given chain length l, j - i +1 = l
            m[i][j] = 100000000;//set this to be a very very very ... big number to represent infinite.

            for(int k=i;k<=j-1;k++){
                cost = m[i][k] + m[k+1][j] + input[i-1]*input[k]*input[j];
                if(cost<m[i][j])
                    m[i][j] = cost;
                    s[i][j] = k;
            }

        }
    }
    
    end = clock();
    time = (double)(end - start)/CLOCKS_PER_SEC;
    cout << "least times of multiplication:" << m[1][n] << endl;
    cout << "time consumed:" << time << endl;

    //to output the parenthesization of matrix, tree data structure is required. To be fulfilled.
    //output the parenthesization

    return 0;
}
