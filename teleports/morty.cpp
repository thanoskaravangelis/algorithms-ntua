#include<stdio.h>
#include<cstdio>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
using namespace std;

int N,M,components,i,j;
int * univ, * par ,* size;

struct door {
    int s1,s2,w;
};

door * portals;

bool for_sort (door i , door j ) {          //Function to sort the portals ,based on their weights.
        return i.w < j.w;
}

int find_(int r) {
    int root = r;                           //Start from node given as root.
    while(root!= par[root])                 //Iterate until you find the root.
        root = par[root];

    while(r!=root) {                        //Path Compression
        int next = par[r];                  //We compress the path leading to the root .
        par[r] = root;                      
        r = next;
    }

    return root;                            //find_ returns the root of the path our node is in. 
}

bool attached (int r, int p) {              //If two nodes return the same root 
    return find_(r)==find_(p) ;             //they are with some path connected.
}

void union_(int r ,int p ) {
    if (attached(r,p))                      //Function that implements Union by rank.
        return;

    int root1 = find_(r);                   
    int root2 = find_(p);

    if (size[root1] < size[root2]) {        //Attach every time the smaller path to the bigger one.
        size[root2] += size[root1];
        par[root1] = root2;
    }
    else {
        size[root1] += size[root2];
        par[root2] = root1;
    }

    components--;
}

bool morties_placed_with(int index){            

    for(i=1; i<N+1; i++) {                  //Function that checks if all Morties
        if (!attached(i,univ[i]))           //can go back to their respective universes.
            return false;                   //Even if all but one of them can't go back , 
        else                                //this function returns false.
            continue;
    }
    return true;

}

int main(){

    scanf("%d %d",&N,&M);
    univ = new int [N+1];                     //array of universes and morties in them 
    portals = new door [M];                   //array for portals
    par = new int [N+1];                      //array for parent nodes 
    size = new int [N+1];                     //array for size of each component
    components = N;

    for(i = 1; i<N+1; i++) {
        scanf("%d ",&univ[i]);
        par[i] = i;
        size[i] = 1;
    }
        
    for(j = 0; j<M; j++) {
        scanf("%d %d %d",&portals[j].s1 ,&portals[j].s2 ,&portals[j].w);
    }

    sort(portals,portals + M,for_sort);                 //Sorting the portals array based on their weights.

    int ret = 0 , l = -1 , r = M+1 , prev=-1;           //Perform binary search for the portals table.
    while(l<=r) {
        int middle = (l+r)/2;

        if(prev < middle) {
            components = N;
            for (i=1; i<N+1; i++) {
                par[i] = i;                             //Initialize again .Every par[i]=i,size[i]=i
                size[i] = i;
            }

            for (j=middle; j<M; j++) {
                union_(portals[j].s1,portals[j].s2);    //Perform union only for weights bigger than what "middle" points us to.
            }
        }
        else {
            for (j=middle; j<prev+1; j++) 
                union_(portals[j].s1,portals[j].s2);    //Perform union only for the weights we didn't check before.
        }

        if(morties_placed_with(middle)) {
            ret = portals[middle].w;
            l = middle + 1;
        }
        else 
            r = middle - 1;

        prev = middle;
    }

    printf("%d\n",ret);
    return 0;
}