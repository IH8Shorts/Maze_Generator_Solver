/* Your code here! */
#include "dsets.h"
#include <iostream>
#include <vector>

//Adds 'num' amount of elements to the disjoint set data structure
void DisjointSets::addelements(int num){
  for(int i = 0; i<num; i++){
    set_.push_back(-1);
  }
}


//finds the root of 'elem' with path compression
int DisjointSets::find(int elem){
  if(set_[elem]<0){return elem;}
  else{return (set_[elem] = find(set_[elem]));}
}


//Unions two sets together
void DisjointSets::setunion(int a, int b){
  int rootA = find(a);
  int rootB = find(b);
  if(find(a)!=find(b)){
    int newSize = set_[rootA]+set_[rootB];
    if(set_[a]<=set_[b]){
      set_[rootB] = rootA;
      set_[rootA] = newSize;
    }
    else{
      set_[rootA] = rootB;
      set_[rootB] = newSize;
    }
  }
}
