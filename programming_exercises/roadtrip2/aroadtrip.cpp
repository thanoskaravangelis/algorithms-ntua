#include<stdio.h>
#include<cstdio>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
using namespace std;

int n,k,d,t,i,ts,tf,cs,cf,fin=1e9+1,max_cap=0;
int ret = 1e9+1 ; 
int * di;

struct node{
    int p,v;
};
node * cars;                                                                             

bool goodie(int mid){
    
    int tnow = 0 , L;                               //Set variable tnow to zero 
    for (i=1;i<=k+1;i++) {
        L=di[i]-di[i-1];                            //L needs to be  covered by our car
        if (L*cs>mid)                               //If I can't cover the distance economically
            return false;                           //This capacity can't work
        else if (L*cf<=mid)                         //Else if I can cover it only with sport mode
            tnow+=L*tf;                             //Add time needed to time covered
        else {
            int rest = mid - L;                     //Or else I can cover it with a combination 
            tnow+=L*ts - rest*tf;                   //of the two modes, add again the time needed to time covered 4
        }
            if(tnow > t )
                return false;
    }                           
    if(tnow > t or tnow < 0)                        //If in the end time covered bigger than T 
        return false ;                              //failure
    return true ;                                   //else success!
}
int main (){

    scanf("%d %d %d %d",&n,&k,&d,&t);               //Inserting N,K,D,T

    di = new int [k+2];
    cars = new node [n];

    for (i=0; i<n; i++)
        scanf("%d %d",&cars[i].p, &cars[i].v);      //Inserting cars (price , fuel capacity)
    
    for (i=1; i<=k; i++) 
        scanf("%d ",&di[i]);                        //Inserting stations' distance from Sorting City
    
    
    scanf("%d %d %d %d",&ts,&cs,&tf,&cf);           //Inserting Ts,Cs,Tf,Cf

    di[0]=0;                                        //First "station" is at x=0
    sort(di+1,di+k+1);                              //Sort the k inserted stations
    di[k+1] = d;                                    //Last "station" is Select City

    int l=1 , b=1e9+1;                              //We perform a binary search to find
    while (l<=b) {                                  //the optimal value of fuel capacity needed.

        int middle = (b+l)/2 ;
        if(goodie(middle)) {                        //If middle succeeds
            ret = middle;                           //Keep middle as return value 
            b = middle - 1;                         //Search if there are any smaller capacities that work
        }
        else 
            l = middle + 1;                         //Doesn't work.Search for bigger.`
    }

    if (ret == 1e9 + 1)                             
            cout << "-1\n" ;
    else {
        for(i=0;i<n;i++) {                          //Search for capacities that are >= of the optimal
            if (cars[i].v >= ret)                   
                fin = min(fin , cars[i].p);         //and detect the minimum price out of those 
        }
        if (fin != 1e9 + 1)
            cout << fin << "\n" ;                   //print minimum price
        else 
            cout << "-1\n";                         //if there are no capacities found ,print -1.
    }
    return 0;
}
