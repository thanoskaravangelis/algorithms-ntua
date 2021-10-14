#include <iostream>
using namespace std;
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>


// global variables
int N, M;

/* array of 9 2d vectors, one vector for each signifnicant sub-problem */ 
vector<vector<int>>mainVec[9];

/*  Vector with 9 rows that will keep in each row (each row->different kind of item) the quantities of the sets of the category 
    Will be read using the q(cat, i) function
    Row 0: 1A
    Row 1: 1B
    Row 2: 1C
    Row 3: 2A
    Row 4: 2B
    Row 5: 2C
    Row 6: 3A
    Row 7: 3B
    Row 8: 3C
*/
vector<vector<int>> d(9); 

/*  same philosophy as the d vector, but keeps the price if the sets 
    will be read using the p(cat, i) function */
vector<vector<int>> s(9);



/*  fills the vectors d and s 
    called by readInput() */
void DSvectorsFill() {
    int i, index;
    int quantity, cost;
    char tmp[2];
    for (i=0; i<M; i++) {
        //cin >> X;
        scanf("%2s", tmp);
        string X = tmp;
        
        //cout << "x = " << X << endl;
        scanf("%i", &quantity);
        //cin >> quantity;
        //cout << "quantity = " << quantity << endl;
        //cin >> cost;
        scanf("%i", &cost);
        //cout << "cost = " << cost << endl;
        
        if (X=="1A") { // d[0], s[0]
            index = 0;
        }
        else if (X=="1B") { // d[1], s[1]
            index = 1;
        }
        else if (X=="1C") { // d[2], s[2]
            index = 2;
        }
        else if (X=="2A") { // d[3], s[3]
            index = 3;
        }
        else if (X=="2B") { // d[4], s[4]
            index = 4;
        }
        else if (X=="2C") { // d[5], s[5]
            index = 5;
        }
        else if (X=="3A") { // d[6], s[6]
            index = 6;
        }
        else if (X=="3B") { // d[7], s[7]
            index = 7;
        }
        else if (X=="3C") { // d[8], s[8]
            index = 8;
        }
        d[index].push_back(quantity);
        s[index].push_back(cost);
    }
}
void mainVectorsInit(){  
    for (int index=0; index<9; index++) {
        for (int i=0; i<int(d[index].size()); i++) {
            vector<int> line;
            for (int k=0; k<N; k++) {
                line.push_back(-1);
            }
            mainVec[index].push_back(line);
        }
    }
}

/* reads input and initializes some vectors */
void readInputAndInit() {
    cin >> N;
    cin >> M;
    DSvectorsFill();
    mainVectorsInit();
}

/* returns the quantity of the i-th set of the cat-th category */
int q(int cat, int i) {
    return d[cat][i-1];
}

/* returns the price of the i-th set of the cat-th category */
int p( int cat, int i) {
    return s[cat][i-1];
}

/*  fills array of the index-th category "mainVec[index]"
    i = index of the category's set that i am on now (need to check all the sets after the i-th (including this one))
    k = #sets i have to find for the category 
    called by void mainCompute()
*/
int compute(int index, int i, int k) {
    int res;
    if (i>=1 and k>=1) {
        if (mainVec[index][i-1][k-1] != -1) {
            return mainVec[index][i-1][k-1];
        }
    }
    int localN = d[index].size();
    if (i==localN) {
        if (k<=0) {    // i saw all the items and i don't need anything else
            res = 0;
        }
        else if (k>0) {
            if (k>q(index, i)) {  // if i don't change it => overflow !! ATTENTION !!
                res = INT_MIN;
            }
            else {
                res = p(index, i);
            }
        }
    }
    else if (i<localN) {
        if (k<=0) {
            res = 0;
        }
        else {
            int c1 = compute(index, i+1, k);
            int c2 = compute(index, i+1, k-q(index, i)) + p(index, i);
            if (c1<0) {
                res = c2;
            }
            else if (c2<0) {
                res=c1;
            }
            else if (c1<0 and c2<0) {
                res=0;
            }
            else {
                res = min(c1, c2);
            }
        }
    }

    if (res!=0) {
        mainVec[index][i-1][k-1] = res;
    }
    return res;
}

/*  fills all the 9 main dp arrays 
    using compute( main array index (items category i.e 1A, 1B, ...), 1, K) */
void mainCompute() {
    for (int index=0; index<9; index++) {
        if (d[index].size()==0) {
            continue;
        }
        for (int k=0; k<=N; k++) {
            int temp = compute(index, 1, k);
        }
    }
}

/* computes the cost of buying n items A, n items B and n items C from salesman "salesman" */
int takeFrom(int salesman, int n) {
    if (n==0) {
        return 0;
    }
    else {
        int costA, costB, costC;
        int offset = 3*(salesman-1);
        if (mainVec[offset].size()==0 or mainVec[offset+1].size()==0 or mainVec[offset+2].size()==0) {
            return INT_MIN;
        }
        else {
            costA = mainVec[offset][0][n-1];
            costB = mainVec[offset+1][0][n-1];
            costC = mainVec[offset+2][0][n-1];
            if (costA<0 or costB<0 or costC<0) {
                return INT_MIN;
            }
            return (costA + costB + costC);
        }
    }
}

int vectorsJoin() {
    int n1, n2;
    int cost1, cost2, cost3, totalCost, min;
    min = INT_MAX;
    for (n1=0; n1<=N; n1++) {
        for (n2=0; n2<=N-n1; n2++) {
            cost1 = takeFrom(1, n1);
            cost2 = takeFrom(2, n2);
            cost3 = takeFrom(3, N-n2-n1);
            if (cost1 <0  or cost2 <0 or cost3 <0) {
                totalCost = INT_MAX;
            }
            else {
                totalCost = cost1 + cost2 + cost3;
                if (totalCost<min) {
                    min = totalCost;
                } 
            }
        }
    }
    if (min==INT_MAX) {
        min = -1;
    }
    return min;
}



int main() {
    readInputAndInit();
    mainCompute();
    for(int i = 0 ; i<9; i++){
        for(unsigned int j=0 ; j<d[i].size(); j++){
            for(int k=0; k<N; k++)
                cout <<"i:" << i << " j:" << j << " k:" << k <<" cost: " << mainVec[i][j][k] << "\n";
        }
    }
    cout << vectorsJoin() << endl;
}
