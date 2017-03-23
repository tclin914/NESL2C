/** 
 * genereated by NESL2C from app2_2.nesl:
 * GMT+8: 2017-3-17 21:14:3

    source : {x+1: x in [1,2,3]};
*/

#include <stdio.h>
#include <stdlib.h>
#include "sqmacro.h"
void myFunc1(){
  {
    int x;
    struct Sequence tmp1;
    int i;
    struct Sequence app1;
    MALLOC(tmp1, 3, struct Sequence);
    SET_ELEM_I(1,tmp1,0);
    SET_ELEM_I(2,tmp1,1);
    SET_ELEM_I(3,tmp1,2);
    MALLOC(app1,tmp1.len,struct Sequence);
    for (i =0; i <app1.len;i++){
      //TODO
      GET_ELEM_I(x, tmp1,i);
      x=x+1;
      // v = quicksort(v);
      SET_ELEM_I(x, app1,i);
    }
  }
}

int main(){
  myFunc1();
  return 1;
}
