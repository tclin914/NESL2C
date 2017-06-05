//function cross_product(o,line) :((float,float),(float,float),(float,float)) ->float =
//let (xo,yo) = o; 
//    ((x1,y1),(x2,y2)) = line;
//in (x1-xo)*(y2-yo) - (y1-yo)*(x2-xo);

#include <stdio.h>
#include "pf/pf.h"

struct tuple_F_F {
  float a;
  float b;
};

struct tuple_tuple_F_F_tuple_F_F {
  struct tuple_F_F a;
  struct tuple_F_F b;
};

struct tuple_tuple_F_F_tuple_tuple_F_F_tuple_F_F {
  struct tuple_F_F a;
  struct tuple_tuple_F_F_tuple_F_F b;
};

#pragma pf device inline
float cross_product(struct tuple_tuple_F_F_tuple_tuple_F_F_tuple_F_F tmp1){
  float res;
  struct tuple_F_F tmp2;
  struct tuple_tuple_F_F_tuple_F_F tmp3;
  tmp2 = tmp1.a;
  tmp3 = tmp1.b;
  float t1,t2,t3,t4,t5,t6,t7;
  t1 = tmp3.a.a - tmp2.a;
  t2 = tmp3.b.b - tmp2.b;
  t3 = t1*t2;

  t4 = tmp3.a.b-tmp2.a;
  t5 = tmp3.b.a-tmp2.b;
  t6 = t4*t5;

  t7 = t3-t6;
  res = t7;
  return res;
}

#pragma pf global parallel
void myFunc1(){
  struct tuple_F_F tmp4,tmp5,tmp6;
  struct tuple_tuple_F_F_tuple_F_F tmp7;
  struct tuple_tuple_F_F_tuple_tuple_F_F_tuple_F_F tmp8;
  float res;
  tmp4.a = 1.0;
  tmp4.b = 1.0;
  tmp5.a = 5.0;
  tmp5.b = 2.4;
  tmp6.a = 4.1;
  tmp6.b = 4.3;
  tmp7.a = tmp5;
  tmp7.b = tmp6;
  tmp8.a = tmp4;
  tmp8.b = tmp7;
  res = cross_product(tmp8);
  printf("res = %f\n",res);
}

int main(){
  myFunc1();
  return 1;
}
