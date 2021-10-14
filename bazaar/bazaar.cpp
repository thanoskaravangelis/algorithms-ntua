#include <stdio.h>
#include <bits/stdc++.h>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string.h>
#include <utility>
using namespace std;

int N, M, i, j, q, p;
char str[2];
vector<vector<vector<int>>> offers; //3d vector for input values
vector<vector<vector<int>>> dp;     //3d vector for knapsack implementation

int items_a1, items_a2, items_a3, items_b1, items_b2, items_b3, items_c1, items_c2, items_c3; // 9 counters for 1A,1B,1C,2A,...,3C to help us find maximum number of equipment for each merchant
int items1, items2, items3;                                                                   // 3 counters each for a merchant from 1 to 3

//recursive knapsack implemented for each one of the 9 types 1A,1B,1C...3B,3C finding minimum cost to get 'b' items
int knapsack_rec(const vector<vector<int>> &arr, int numb, int i, int b)
{
    int temp, cost1, cost2;
    int vec_size = arr.size();

    if (b <= 0)
    {
        return 0;
    }
    if (i >= 1 && b >= 1 && dp[numb][i - 1][b - 1] != -1)
    {
        return dp[numb][i - 1][b - 1];
    }
    if (i < vec_size)
    {
        cost1 = knapsack_rec(arr, numb, i + 1, b);
        cost2 = knapsack_rec(arr, numb, i + 1, b - arr[i - 1][0]) + arr[i - 1][1];

        if (cost1 < 0)
        {
            temp = cost2;
        }
        else if (cost2 < 0)
        {
            temp = cost1;
        }
        else if (cost1 < 0 and cost2 < 0)
        {
            return 0;
        }
        else
        {
            temp = min(cost1, cost2);
        }
    }
    if (i == vec_size)
    {
        if (b > arr[i - 1][0])
        {
            temp = INT_MIN;
        }
        else
        {
            temp = arr[i - 1][1];
        }
    }
    if (temp != 0)
    {
        dp[numb][i - 1][b - 1] = temp;
    }
    return temp;
}

// function that helps us compute the sum for the three types A,B and C for each merchant 1,2 and 3
int find_sums(int m, int n)
{
    if (n == 0)
    {
        return 0;
    }
    else
    {
        int index = 3 * (m - 1);
        if (dp[index].size() == 0 or dp[index + 1].size() == 0 or dp[index + 2].size() == 0)
        {
            return INT_MIN;
        }
        else
        {
            int A = dp[index][0][n - 1];
            int B = dp[index + 1][0][n - 1];
            int C = dp[index + 2][0][n - 1];

            if (A < 0 or B < 0 or C < 0)
            {
                return INT_MIN;
            }

            return A + B + C;
        }
    }
}

int main()
{

    scanf("%d %d", &N, &M);
    offers.resize(9);

    for (i = 0; i < M; i++)
    {
        scanf("%s %d %d", str, &q, &p); //scan a string from the 9 possible ones to see where we should place the pair (q,p)

        if (strcmp(str, "1A") == 0)
        {
            offers[0].push_back({q, p});
            items_a1 += q;
        }
        else if (strcmp(str, "1B") == 0)
        {
            offers[1].push_back({q, p});
            items_b1 += q;
        }
        else if (strcmp(str, "1C") == 0)
        {
            offers[2].push_back({q, p});
            items_c1 += q;
        }
        else if (strcmp(str, "2A") == 0)
        {
            offers[3].push_back({q, p});
            items_a2 += q;
        }
        else if (strcmp(str, "2B") == 0)
        {
            offers[4].push_back({q, p});
            items_b2 += q;
        }
        else if (strcmp(str, "2C") == 0)
        {
            offers[5].push_back({q, p});
            items_c2 += q;
        }
        else if (strcmp(str, "3A") == 0)
        {
            offers[6].push_back({q, p});
            items_a3 += q;
        }
        else if (strcmp(str, "3B") == 0)
        {
            offers[7].push_back({q, p});
            items_b3 += q;
        }
        else if (strcmp(str, "3C") == 0)
        {
            offers[8].push_back({q, p});
            items_c3 += q;
        }
    }

    items1 = min(items_a1, min(items_b1, items_c1)); //finding the maximum size of a knapsack for merchant 1
    items2 = min(items_a2, min(items_b2, items_c2)); //finding the maximum size of a knapsack for merchant 2
    items3 = min(items_a3, min(items_b3, items_c3)); //finding the maximum size of a knapsack for merchant 3

    int items_all = items1 + items2 + items3; //maximum number of total items

    if (N > items_all)
    {
        cout << "-1";
        return 0;
    }

    dp.resize(9);
    int b;

    for (j = 0; j < 9; j++)
    {
        if (offers[j].size() == 0) //if size of vector is zero , skip
        {
            continue;
        }

        int vec_size = offers[j].size();
        dp[j].resize(vec_size);             //resize each vector dp[j] to the size of the offers[j]
        for (int i = 0; i < vec_size; i++)
        {
            dp[j][i].resize(N, -1);         //fill every position with -1 (we have 9 x N x vec_size positions)
        }

        if (j >= 0 and j <= 2)
        {
            for (int k = 0; k <= min(N, items1); k++)
            {
                b = knapsack_rec(offers[j], j, 1, k);
            }
        }
        else if (j >= 3 and j <= 5)
        {
            for (int k = 0; k <= min(N, items2); k++)
            {
                b = knapsack_rec(offers[j], j, 1, k);
            }
        }
        else
        {
            for (int k = 0; k <= min(N, items3); k++)
            {
                b = knapsack_rec(offers[j], j, 1, k);
            }
        }
    }

    M = b;

    int p, q, total, mincost = INT_MAX;

    for (p = 0; p <= N; p++)
    {
        for (q = 0; q <= N - p; q++)
        {
            if (find_sums(1, p) < 0 or find_sums(2, q) < 0 or find_sums(3, N - p - q) < 0)
            {
                total = INT_MAX;
            }
            else
            {
                total = find_sums(1, p) + find_sums(2, q) + find_sums(3, N - p - q);
                if (total < mincost)
                {
                    mincost = total;
                }
            }
        }
    }
    if (mincost == INT_MAX)
    {
        mincost = -1;
    }
    cout << mincost << "\n";

    return 0;
}
