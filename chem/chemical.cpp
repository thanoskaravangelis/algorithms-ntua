#include<stdio.h>
#include<cstdio>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<utility>
using namespace std;
#define MAXIMUM 100000000;

int **costs;            //array for substances' mixing costs
int ** A ;              //array for the cost of mixing each substance with another
int ** dp;              //array used for memoization in dynamic()
int N,K,i,j,ret;

//function that returns the cost for substances "from" until "to" 
int cost (int from , int to ) 
{
    return (costs[to][to] + costs[from][from] - costs[to][from] - costs[from][to])/2;
}

//function to initialize arrays for costs and dynamic programming
void initialize() 
{

    for(int i = 1; i < N+1; i++)
    {
        costs[i][i] = costs[i][i - 1] + costs[i - 1][i] - costs[i - 1][i - 1];  
  	    for(int j = i+1; j < N+1; j++)
        {
            costs[i][j] = A[i][j] + costs[i][j - 1] + costs[i - 1][j] - costs[i - 1][j - 1];
            costs[j][i] = costs[i][j];
        }     
    }

    dp = new int*[K+1];
    for (int i = 0; i < K+1; i++)
    {
        dp[i] = new int [N+1];
    }

    dp[0][0] = 0;
    for (int i = 1; i < N+1; i++)
    {
        dp[0][i] = cost (0,i);
    }

    ret = dp[0][N];

    return ; 
}

//recursive function that fills the dp array accordingly using binary search 
void dynamic(int j,int left, int right, int leftlim ,int rightlim)
{
    if (left > right) 
    {
        return;  
    }
    
	int index , cur_cost ;
    int m = (left + right) / 2;
	dp[j-1][m] = MAXIMUM;

	for (int i = leftlim; i <= m and i <= rightlim; i++) 
    {
		cur_cost = cost(i-1,m);

        if (dp[j-1][m] > dp[j-2][i-1] + cur_cost) 
        {
			dp[j-1][m] = dp[j-2][i-1] + cur_cost;
			index = i;
		}
	}


    dynamic(j, left, m-1, leftlim, index);
    dynamic(j, m+1, right, index, rightlim);
}

int main() 
{
    scanf("%d %d",&N,&K);

    A = new int*[N+1];
    costs = new int*[N+1];

    for (i = 0; i < N+1; i++)
    {
        A[i] = new int [N+1];
        costs[i] = new int [N+1];
        for(j = 0; j < N+1; j++)
        {
            A[i][j] = 0;
            costs[i][j] = 0;
        }
    }

    for (i = 1; i < N+1; i++)
    {
        for (j = i+1; j < N+1; j++)
        {
            scanf("%d", &A[i][j]);
            A[j][i] = A[i][j];
        }
    }

    initialize();

    for (i = 2; i < K+1; i++)
    {
	    dynamic(i, i, N, i, N);

		for(j = 0; j < N+1; j++) 
        {
            dp[i-2][j]=dp[i-1][j];
        }
		ret = dp[i-1][N];	
	}

    printf("%d\n",ret);

    return 0;
}