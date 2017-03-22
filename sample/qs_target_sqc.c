/** 
 * genereated by NESL2C from qs_target.nesl:
 * GMT+8: 2017-3-22 22:0:27
 */

#include <stdio.h>
#include <stdlib.h>
#include "sqmacro.h"
#pragma pf device inline
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
    //BIND->APPBODY2
    {
      struct Sequence v;
      struct Sequence tmp1;
      struct Sequence res;
      int i;
      struct Sequence app1;
      MALLOC(tmp1, 2, struct Sequence);
      MALLOC(app1,tmp1.len,struct Sequence);
      for (i =0; i <app1.len;i++){
        GET_ELEM_I(v,tmp1,i);
        SET_ELEM_I(quicksort,app1,i);
      }
    }
    //end of BIND->APPBODY2
    GET_ELEM_SEQ_I(elm1,result, 0 );
    CONCAT(elm1, equal, tmp2, int, I);
    GET_ELEM_SEQ_I(elm2,result, 1 );
    CONCAT(tmp2, elm2, res, int, I);
    //TODO *DECREF
  }
  return res;

}
#pragma pf device inline
struct tupleIF qs(int  n){
  // FIXME typeAnalysis should find type-mismatch.
  //struct tupleIF res;
  struct Sequence res;
  struct Sequence nums;
  //BIND->APPBODY2
  {
    int e;
    struct Sequence tmp4;
    struct Pair_I tmp5;
    int res;
    int i;
    struct Sequence app2;
    NESLDIST(tmp4,100,n);
    MALLOC(app2,tmp4.len,struct Sequence);
    for (i =0; i <app2.len;i++){
      //TODO declare a tmp var with type of action.
      //TODO consider insert indepent tmp node when pfcheck!!!
      int tmp;
      GET_ELEM_I(e,tmp4,i);
      // TODO generate "tmp =" sqcodegen(action);
      tmp = rand(e);
      // TODO generate "tmp,app2,i"
      SET_ELEM_I(tmp,app2,i);
      // WRONG: 
      //SET_ELEM_I(rand,app2,i);
    }
    print_SEQ_I(app2);
  }
  //end of BIND->APPBODY2
  //TODO return res = quicksort(nums);
    res = quicksort(nums);
  // WRONG_GEN:
  // quicksortreturn res;

}
void myFunc1(){
  {
    struct tupleIF  tmp6;
    tmp6 = qs( 100 );
    print_Tuple(tmp6, I, F);
  }
}

int main(){
  // FIXME srand(time(0)) shouldn't be here.
  srand(time(0));
  myFunc1();
  return 1;
}
