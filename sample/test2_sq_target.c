/** 
 * genereated by NESL2C from test2.nesl:
 * GMT+8: 2017-3-15 15:55:54
 */

#include <stdio.h>
#include <stdlib.h>
#include "sqmacro.h"


#pragma pf device parallel
struct Sequence  quicksort(struct Sequence  a){
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
    *REFCNT(lesser, int)-=1;
    FILTER_1(equal, e, int, I,
             a, e,  int, I,
             (e == pivot));
    *REFCNT(equal, int)-=1;
    FILTER_1(greater, e, int, I,
             a, e,  int, I,
             (e > pivot));
    *REFCNT(greater, int)-=1;
    MAKE_SEQ_2(tmp1, struct Sequence, SEQ_I, lesser,greater);
    *REFCNT(tmp1, struct Sequence)-=1;
    MALLOC(result, 2, struct Sequence);
#pragma pf parallel_rr
    for(i=0;i<2;i++){
      struct Sequence v;
      struct Sequence res;
      GET_ELEM_SEQ_I(v, tmp1, i);
      res = quicksort(v);
      *REFCNT(res, int)-=1;
      SET_ELEM_SEQ_I(res, result, i);
      DECREF_SEQ_I(res);
      *REFCNT(result, struct Sequence)-=1;
      DECREF_SEQ_SEQ_I(tmp1);
    }
    GET_ELEM_SEQ_I(elm1,result, 0 );
    CONCAT(elm1, equal, tmp2, int, I);
    *REFCNT(tmp2, int)-=1;

    GET_ELEM_SEQ_I(elm2,result, 1 );
    CONCAT(tmp2, elm2, res, int, I);
    DECREF_SEQ_I(lesser);
    DECREF_SEQ_I(equal);
    DECREF_SEQ_I(greater);
    DECREF_SEQ_SEQ_I(result);
    DECREF_SEQ_I(tmp2);
  }
  return res;

}

#pragma pf device parallel
struct tupleIF qs(int  n){
  struct tupleIF res;
  struct Sequence nums;
  struct Sequence res1;
  float tm;
  int i;
  struct tupleIF tmp6;
  int elm3;
  struct Sequence app2;
  {
    int e;
    struct Sequence tmp4;
    int res;
    NESLDIST(tmp4,100,n);
    //NESLRAND_SEQ(nums,n,tmp4,e,I);
    MALLOC(app2, tmp4.len, struct Sequence);
    for(i=0;i<app2.len;i++){
      GET_ELEM_I(e, tmp4,i);
      e = RAND_I(e);
      SET_ELEM_I(e, app2,i);
    } 
    FREE(tmp4);
  }
  MALLOC(nums, app2.len, struct Sequence);
  for(i =0;i<app2.len;i++){
    int tmp;
    GET_ELEM_I(tmp, app2, i);
    SET_ELEM_I(tmp, nums, i);
  }
  FREE(app2);

  {
    int t1,t2;
    float diff;
    t1 = clock();
    res1 = quicksort(nums);
    t2 = clock();
    diff = ((float)(t2 - t1) / 1000000000.0F ) * 1000;
    tm = diff;
  }
  print_SEQ_I(res1);
  GET_ELEM_I(elm3,res1, 0 );
  res.a = elm3;
  res.b = tm;
  return res;

}
#pragma global parallel
void myFunc1(){
  {
    struct tupleIF  tmp7;
    tmp7 = qs( 100 );
    print_Tuple(tmp7, I, F);
  }
}
int main(){
  myFunc1();
  return 1;
}
