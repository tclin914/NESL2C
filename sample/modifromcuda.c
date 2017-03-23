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

#pragma pf device_private
struct Sequence D_Array;

#pragma pf global parallel
void qs_demo() {
  int i, len,_refcnt;
  struct Sequence res;

  i = 0;

  res = quicksort(D_Array);
  //atomicAdd(REFCNT(res, int), 1);
  ////++*REFCNT(res, int);
  //_refcnt++;

  len = res.len;

#pragma pf parallel_mc
  for(i=0; i<len; i++) {
    int elem;

    GET_ELEM_I(elem, res, i);
    SET_ELEM_I(elem, D_Array, i);
  }

  ////DECREF_SEQ_I(res);

  return;
}

void genTest(int *array, int size, int randomSeed) {
  int i, randBase;
  srand(randomSeed);

#if DEBUG==1
  randBase = 0;
#else
  randBase = rand()%10000;
#endif
  for(i=0; i<size; i++) {
    array[i] = i+randBase;
  }

  for(i=0; i<size; i++) {
    int des, temp;
    des = rand()%size;
    temp = array[i];
    array[i] = array[des];
    array[des] = temp;
  }
}

bool verifyQSort(int *array, int size, int randomSeed) {
  int randBase, i;

  srand(randomSeed);

#if DEBUG==1
  randBase = 0;
#else
  randBase = rand()%10000;
#endif
  for(i=0; i<size; i++) {
    if(array[i] != i+randBase)
      return false;
  }
  return true;
}

struct Sequence H_Array;

int main(int argc, char *argv[]) {
  int seed;
  struct Sequence d_Array;
  int i;
  int startTime, endTime;

  if(argc < 2) {
    printf("Usage: %s <Array size> [Random Seed]\n", argv[0]);
    exit(0);
  }

  H_Array.len = atoi(argv[1]);
  H_Array.cap = H_Array.len;

  if(argc < 3) {
    seed = time(0);
    printf("Random seed: %d\n", seed);
  }
  else {
    seed = atoi(argv[2]);
  }

  H_Array.ptr = malloc(sizeof(int)*H_Array.cap+sizeof(uint32_t));

  /* Generate initial input */
  genTest((int*)H_Array.ptr, H_Array.len, seed);

//#if DEBUG==1
  for(i=0; i<H_Array.len; i++)
    printf("%d ", ((int*)H_Array.ptr)[i]);
  printf("\n");
//#endif

//  *REFCNT(H_Array, int) = 1;
//
//  SET_HEAP_SIZE(MALLOC_HEAP_SIZE);
//
//  d_Array = H_Array;
//  CHECK_ERR(cudaMalloc(&d_Array.ptr, sizeof(int)*d_Array.cap+sizeof(uint32_t)));
//  CHECK_ERR(cudaMemcpy(d_Array.ptr, H_Array.ptr, sizeof(int)*H_Array.cap+sizeof(uint32_t), cudaMemcpyHostToDevice));
//
//  CHECK_ERR(cudaMemcpyToSymbol(D_Array, &d_Array, sizeof(struct Sequence)));
//
//  cudaDeviceSynchronize();
//
//  startTime = getTimeInMicros();
  printf("Kernel starts running...\n");
//
  memcpy(&D_Array, &H_Array, sizeof(H_Array));
  qs_demo();
//
//  endTime = getTimeInMicros();
//  printf("Running time: %f ms\n", (float)(endTime-startTime)/1000.0);
//
//  CHECK_ERR(cudaMemcpyFromSymbol(&d_Array, D_Array, sizeof(struct Sequence)));
//  cudaDeviceSynchronize();
//
//  H_Array.len = d_Array.len;
//  H_Array.cap = d_Array.cap;
//  CHECK_ERR(cudaMemcpy(H_Array.ptr, d_Array.ptr, sizeof(int)*d_Array.cap+sizeof(uint32_t), cudaMemcpyDeviceToHost));
//  cudaDeviceSynchronize();
//
//#if DEBUG==1
  for(i=0; i<H_Array.len; i++)
    printf("%d ", ((int*)H_Array.ptr)[i]);
  printf("\n");
//#endif
//
//  if(verifyQSort((int*)H_Array.ptr, H_Array.len, seed) == true) {
//    printf("Verify success!\n");
//  }
//
  free(H_Array.ptr);
//  cudaFree(d_Array.ptr);

  return 0;
}

