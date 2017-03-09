#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "sqmacro.h"
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <string.h>
int x;
struct Pair_F tmp1;

#pragma pf device parallel
struct Sequence  quicksort(struct Sequence a){
  struct Sequence res;
  if(a.len <  2 ){
    res = a;
  }else{
    int pivot;
    struct Sequence lesser;
    struct Sequence equal;
    struct Sequence greater;
    struct Sequence result;
    struct Sequence tmp3;
    int i;
    struct Sequence elm1;
    struct Sequence tmp4;
    struct Sequence elm2;
    struct Sequence tmp5;
    int _refcnt;
    GET_ELEM_I(pivot,a,a.len/ 2 );
    FILTER_1(lesser, e, int, I,
             a, e,  int, I,
             (e < pivot));
    //++*(REFCNT(lesser, int));
    //_refcnt++;
    //atomicAdd(REFCNT(lesser, int), 1);
    FILTER_1(equal, e, int, I,
             a, e,  int, I,
             (e == pivot));
    //atomicAdd(REFCNT(equal, int), 1);
    //++*(REFCNT(equal, int));
    //_refcnt++;
    FILTER_1(greater, e, int, I,
             a, e,  int, I,
             (e > pivot));
    //atomicAdd(REFCNT(greater, int), 1);
    //++*(REFCNT(greater, int));
    //_refcnt++;
    MAKE_SEQ_2(tmp3, struct Sequence, SEQ_I, lesser,greater);
    //atomicAdd(REFCNT(tmp3, struct Sequence), 1);
    //++*(REFCNT(tmp3, struct Sequence));
    //_refcnt++;
    MALLOC(result, 2, struct Sequence);
#pragma pf parallel_rr
    for(i=0;i<2;i++){
      struct Sequence v;
      struct Sequence res;
      GET_ELEM_SEQ_I(v, tmp3, i);
      res = quicksort(v);
      //atomicAdd(REFCNT(res, int), 1);
      //++*REFCNT(res, int);
      //////_refcnt++;
      SET_ELEM_SEQ_I(res, result, i);
      //DECREF_SEQ_I(res);
    }
    //atomicAdd(REFCNT(result, struct Sequence), 1);
    //++*(REFCNT(result, struct Sequence));
    ////_refcnt++;
    //DECREF_SEQ_SEQ_I(tmp3);
  GET_ELEM_SEQ_I(elm1,result, 0 );
  CONCAT(elm1, equal, tmp4, int, I);
  //atomicAdd(REFCNT(tmp4, int), 1);
  //++*REFCNT(tmp4, int);
  ////_refcnt++;
  GET_ELEM_SEQ_I(elm2,result, 1 );
  CONCAT(tmp4, elm2, res, int, I);
//DECREF_SEQ_I(lesser);
//DECREF_SEQ_I(equal);
//DECREF_SEQ_I(greater);
//DECREF_SEQ_SEQ_I(result);
//DECREF_SEQ_I(tmp4);
}
return res;

}


int neslrand(int a){
  int seed;
  int randBase;
  int des;

  seed = time(0);
  srand(seed);
  randBase = rand()%a;
  
  return randBase;
}

//struct tupleSIF time(struct 

struct tupleIF qs(int n){
  struct tupleIF res;
  struct Sequence nums;
  struct Sequence tmp1; // i in dist(100,n)
  int i ;
  //struct tupleSIF tmp3;
  struct Sequence res1; //
  float tm; 
  clock_t t1,t2;
  float diff;

  // dist(100,n)
  MALLOC(tmp1, n, struct Sequence);
  for(i = 0; i < n; i++){
    int elem = 100;
    SET_ELEM_I(elem, tmp1, i);  
  }  
  // {rand(e): e in dist}
  MALLOC(nums, n, struct Sequence);
  // i < n or i < tmp2.len
  for(i =0; i<n; i++){
    int elem;
    int e;
    GET_ELEM_I(e, tmp1,i);
    elem = neslrand(e);
    SET_ELEM_I(elem, nums, i);
  }
  
  // (res1,tm) = time(quicksort(nums));
  t1 = clock();
  res1 = quicksort(nums);
  t2 = clock();
  diff = ((float)(t2 - t1) / 1000000.0F ) * 1000; 
  tm = diff;
  //TIME(quicksort, struct SEQ_I,
  //tmp3 = time(quicksort(nums));

  // in (res1[0],tm)
  res.a = ((int*)res1.ptr)[0];
  res.b = tm;
  
  return res;
}

void myFunc(){
  struct tupleIF tmp1;
  tmp1 =  qs(100);
  printf("%d\n", tmp1.a);
  printf("%f\n", tmp1.b);

}

int main(){
  myFunc();
  return 0;
}
