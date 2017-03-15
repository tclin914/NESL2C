#include <stdio.h>
#include <stdlib.h>
#include "sqmacro.h"
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
    FILTER_1(equal, e, int, I,
             a, e,  int, I,
             (e == pivot));
    FILTER_1(greater, e, int, I,
             a, e,  int, I,
             (e > pivot));
    MAKE_SEQ_2(tmp3, struct Sequence, SEQ_I, lesser,greater);
    MALLOC(result, 2, struct Sequence);
#pragma pf parallel_rr
    for(i=0;i<2;i++){
      struct Sequence v;
      struct Sequence res;
      GET_ELEM_SEQ_I(v, tmp3, i);
      res = quicksort(v);
      SET_ELEM_SEQ_I(res, result, i);
    }
    free(tmp3.ptr);
  GET_ELEM_SEQ_I(elm1,result, 0 );
  CONCAT(elm1, equal, tmp4, int, I);
  GET_ELEM_SEQ_I(elm2,result, 1 );
  CONCAT(tmp4, elm2, res, int, I);
  free(lesser.ptr);
  free(equal.ptr);
  free(greater.ptr);
  free(result.ptr);
  free(tmp4.ptr);
}
return res;

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
  int elem2;

  // dist(100,n)
  {
  NESLDIST(tmp1, 100, n);

  // {rand(e): e in dist}
  NESLRAND_SEQ(nums, n, tmp1, e, I);
}

//  print_SEQ_I(nums);
 
  // (res1,tm) = time(quicksort(nums));
  t1 = clock();
  res1 = quicksort(nums);
  t2 = clock();
  // 1ms = 0.001
  diff = ((float)(t2 - t1) / 1000000000.0F ) * 1000; 
  tm = diff;
  //TIME(quicksort, struct SEQ_I,
  //tmp3 = time(quicksort(nums));

  // in (res1[0],tm)
  GET_ELEM_I(elem2, res1, 0);
  //res.a = ((int*)res1.ptr)[0];
  res.a = elem2;
  res.b = tm;
 
//print_SEQ_I(res1);
  return res;
}

void myFunc(){
  struct tupleIF tmp1;
  tmp1 =  qs(100000);
  print_Tuple(tmp1, I, F);
}

int main(){
  myFunc();
  return 0;
}
