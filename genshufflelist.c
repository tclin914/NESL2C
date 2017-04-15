#include <stdio.h>
#include <stdlib.h>
#include "sqmacro.h"
#include <math.h>

int isthesame(struct Sequence a, struct Sequence b){
  int len = a.len;
  int elm1, elm2;
  int i =0 ;
  for( i =0; i <len; i++){
    GET_ELEM_I(elm1,a,i); 
    GET_ELEM_I(elm2,b,i); 
    if(elm1!=elm2) {printf("[ERROR] Sequence a and b are not the same.\n"); return 0;}
  }
  printf("Sequence a and b are identical.\n");
  return 1;
}

int main(int argc, char** argv){
  
  if(argc ==1) {printf("input size please!\n"); return 0;}
  if(argc >2) {printf("how much argument you think you can use?\n"); return 0;}

  int length = atoi(argv[1]);
  char filename[100]; sprintf(filename,"shuffledList%d",length);
  FILE *fptr;
  fptr = fopen(filename,"w+");

  struct Sequence a = genShuffledList(0,length);
  seed = 123456789;
  struct Sequence b = genShuffledList(0,length);
  if(isthesame(a,b)){
    seed = 123456789;
    struct Sequence c = genShuffledList(0,length);
    if(isthesame(c,b)){
      fprintf(fptr, "[ %d ]\n( ", c.len);
      for(int i =0; i<c.len;i++){
        int elm ;
        GET_ELEM_I(elm , c, i);
        fprintf(fptr, "%d ",elm);
      } 
      fprintf(fptr, ")\n");
    }else{
    printf("ERROR\n");
    }

  }else{
    printf("ERROR\n");
  }

  return 0;
}
