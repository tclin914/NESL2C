/** 
 * genereated by NESL2C from test2.nesl:
 * GMT+8: 2017-3-12 19:45:48
 */

#include <stdio.h>
#include <stdlib.h>
#include "sqmacro.h"
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
    struct Sequence tmp1;
    int i;
    struct Sequence elm1;
    struct Sequence tmp2;
    struct Sequence elm2;
    struct Sequence tmp3;
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
    GET_ELEM_SEQ_I(elm1,result, 0 );
    CONCAT(elm1, equal, tmp2, int, I);
    GET_ELEM_SEQ_I(elm2,result, 1 );
    CONCAT(tmp2, elm2, res, int, I);
  }
  return res;

}
#pragma pf device inline
struct tupleIF qs(int n){
  struct tupleIF res;
  struct Sequence nums;
  struct Sequence res1;
  float tm;
  int i;
  struct tupleIF tmp6;
  int elm3;
  {
    int e;
    struct Sequence tmp4;
    int res;
    NESLDIST(tmp4,100,n);
    NESLRAND_SEQ(nums,n,tmp4,e, I);

  }
  {
    int t1,t2;
    float diff;
    t1 = clock();
    t2 = clock();
    diff = ((float)(t2 - t1) / 1000000000.0F ) * 1000;
    tm = diff;
  }
  GET_ELEM_I(elm3,res1, 0 );
  res.a = elm3;
  res.b = tm;
  return res;
}
#pragma global parallel
void myFunc1(){
  qs( 100 );
}

int main(){
  myFunc1();
  return 1;
}
