#include <inttypes.h>
#include <stdbool.h>
#include <time.h>
#include <cuda.h>
struct Pair_I {
  int   a;
  int   b;
};


struct Pair_F {
  float   a;
  float   b;
};

struct tupleIF {
  int   a;
  float   b;
};

/* Reference count should stored with the array */
struct Sequence {
  int     len;    // Length
  int     cap;    // Capacity
  void   *ptr;
};

// shader speed. compute result in ms
#define CLOCKSPEED 1110500 
/* Macros to access sequences of various type
 * These should be generated automatically */
#define REFCNT(seq, type) ((int*)(((uint8_t*)seq.ptr)+seq.cap*sizeof(type)))

#define GET_ELEM_I(res, arr, idx) do { \
  res = ((int*)arr.ptr)[idx]; } while(0)

#define GET_ELEM_F(res, arr, idx) do { \
  res = ((float*)arr.ptr)[idx]; } while(0)

#define GET_ELEM_PAIR_F(res, arr, idx) do { \
  res.a = ((float*)arr.ptr)[idx]; \
  res.b = ((float*)arr.ptr)[arr.cap+idx]; } while(0)

#define GET_ELEM_SEQ_I(res, arr, idx) do { \
  res = ((struct Sequence*)arr.ptr)[idx]; \
} while(0)

#define GET_ELEM_SEQ_PAIR_F(res, arr, idx) do { \
  res = ((struct Sequence*)arr.ptr)[idx]; \
} while(0)

#define SET_ELEM_I(elm, arr, idx) do { \
  ((int*)arr.ptr)[idx] = elm; \
} while(0)

#define SET_ELEM_F(elm, arr, idx) do { \
  ((float*)arr.ptr)[idx] = elm; \
} while(0)

#define SET_ELEM_PAIR_F(elm, arr, idx) do { \
  ((float*)arr.ptr)[idx] = elm.a; \
  ((float*)arr.ptr)[arr.cap+idx] = elm.b; } while(0)

#define SET_ELEM_SEQ_I(elm, arr, idx) do { \
  int _refcnt = atomicAdd(REFCNT(elm, int), 1); \
  /*assert(_refcnt > 0); */\
  ((struct Sequence*)arr.ptr)[idx] = elm; \
} while(0)

/* We need the element type of the sub sequence so that we can increase the reference count */
#define SET_ELEM_SEQ_PAIR_F(elm, arr, idx) do { \
  int _refcnt = atomicAdd(REFCNT(elm, struct Pair_F), 1); \
  /*assert(_refcnt > 0); */\
  ((struct Sequence*)arr.ptr)[idx] = elm; \
} while(0)

/* These can be put in the header file */
#define MALLOC(res, numElem, type) do { \
  res.len = numElem; \
  res.cap = numElem; \
  res.ptr = malloc(res.cap*sizeof(type)+sizeof(int)); \
  /*assert(res.ptr != 0);*/ \
  *REFCNT(res, type) = 0; \
} while(0)

#define FREE(res) do { \
  free(res.ptr); } while(0)

#define MAKE_SEQ_2(res, type, typeMacro, e1, e2) do { \
  MALLOC(res, 2, type); \
  SET_ELEM_ ## typeMacro(e1, res, 0); \
  SET_ELEM_ ## typeMacro(e2, res, 1); \
} while(0)

#define DECREF_SEQ_I(seq) do { \
  int _refcnt = atomicSub(REFCNT(seq, int), 1);\
  /*assert(_refcnt < 100);*/ \
  if(_refcnt == 1) { \
    FREE(seq); \
  }} while(0)

#define DECREF_SEQ_F(seq) do { \
  int _refcnt = atomicSub(REFCNT(seq, float), 1);\
  /*assert(_refcnt < 100); */\
  if(_refcnt == 1) { \
    FREE(seq); \
  }} while(0)

#define DECREF_SEQ_PAIR_F(seq) do { \
  int _refcnt = atomicSub(REFCNT(seq, struct Pair_F), 1);\
  /*assert(_refcnt < 100); */\
  if(_refcnt == 1) { \
    FREE(seq); \
  }} while(0)

#define DECREF_SEQ_SEQ_I(seq) do { \
  int _refcnt = atomicSub(REFCNT(seq, struct Sequence), 1); \
  /*assert(_refcnt < 100); */\
  if(_refcnt == 1) { \
    struct Sequence *_subseq = (struct Sequence*)seq.ptr; \
    int _i = 0; \
    while(_i < seq.len) { \
      DECREF_SEQ_I(_subseq[_i]); \
      _i++; \
    } \
    FREE(seq); \
  }} while(0)

#define DECREF_SEQ_SEQ_PAIR_F(seq) do { \
  int _refcnt = atomicSub(REFCNT(seq, struct Sequence), 1); \
  /*assert(_refcnt < 100); */\
  if(_refcnt == 1) { \
    struct Sequence *_subseq = (struct Sequence*)seq.ptr; \
    int _i = 0; \
    while(_i < seq.len) { \
      DECREF_SEQ_PAIR_F(_subseq[_i]); \
      _i++; \
    } \
    FREE(seq); \
  }} while(0)

#define CONCAT(s1, s2, res, type, typeMacro) { \
  int _i, _len; \
  MALLOC(res, s1.len+s2.len, type); \
  _len = res.len; _NEWLINE_  \
  _SHARP_ pragma pf parallel_mc _NEWLINE_  \
  for(_i=0; _i<_len; _i++) { \
    type _elem; \
    if(_i < s1.len) \
    GET_ELEM_ ## typeMacro(_elem, s1, _i); \
    else \
    GET_ELEM_ ## typeMacro(_elem, s2, _i-s1.len); \
    SET_ELEM_ ## typeMacro(_elem, res, _i); \
  }}

/* Used to prepend an element to a sequence */
#define PREPEND(elem, s, res, type, typeMacro) do { \
  int _i = 0; \
  MALLOC(res, s.len+1, type); \
  SET_ELEM_ ## typeMacro(elem, res, 0); \
  while(_i < s.len) { \
    type _e; \
    GET_ELEM_ ## typeMacro(_e, s, _i); \  SET_ELEM_ ## typeMacro(_e, res, _i+1); \
    _i++; \
  }} while(0)

#define FLATTEN(seq, res, type, typeMacro) do { \
  int _i=0, _numElem=0, _numCopied=0; \
  struct Sequence *_subseq = ((struct Sequence*)seq.ptr); \
  while(_i < seq.len) { \
    _numElem += _subseq[_i].len; \
    _i++; \
  } \
  MALLOC(res, _numElem, type); \
  _i = 0; \
  while(_i < seq.len) { \
    struct Sequence _s = _subseq[_i]; \
    int _len=_s.len, _j;  _NEWLINE_ \
    _SHARP_ pragma pf parallel_mc   _NEWLINE_\
    for(_j=0; _j<_len; _j++) { \
      type _elem; \
      GET_ELEM_ ## typeMacro(_elem, _s, _j); \
      SET_ELEM_ ## typeMacro(_elem, res, _j+_numCopied); \
    } \
    _numCopied += _s.len; \
    _i++; \
  }} while(0)

/* The filtering macros
 * Problem: This approach may distrub the original order of elements */
#define FILTER_1(res, resExpr, typer, typeMacror, s1, e1, type1, typeMacro1, predExpr) do { \
  int _filteredLen=0, _len=s1.len, _i; \
  MALLOC(res, _len, typer);  _NEWLINE_ \
  _SHARP_ pragma pf parallel_mc  _NEWLINE_ \
  for(_i=0; _i<_len; _i++) { \
    type1 e1; \
    uint32_t _mask, _offset; \
    bool _p; \
    GET_ELEM_ ## typeMacro1(e1, s1, _i); \
    _p = predExpr; \
    _mask = __ballot(_p); \
    if(laneID() == 0) { \
      _offset = atomicAdd(&_filteredLen, __popc(_mask)); \
    } \
    _offset = __shfl(_offset, 0); \
    if(_p) { \
      typer _r = resExpr; \
      _offset += __popc(_mask & __lanemask_lt()); \
      SET_ELEM_ ## typeMacror(_r, res, _offset); \
    } \
  } \
  res.len = _filteredLen; \
} while(0)

#define FILTER_2(res, resExpr, typer, typeMacror, s1, e1, type1, typeMacro1, s2, e2, type2, typeMacro2, predExpr) do { \
  int _filteredLen=0, _len=s1.len, _i; \
  MALLOC(res, _len, typer);   \
  _SHARP_ pragma pf parallel_mc  _NEWLINE_ \
  for(_i=0; _i<_len; _i++) { \
    type1 e1; \
    type2 e2; \
    uint32_t _mask, _offset; \
    bool _p; \
    GET_ELEM_ ## typeMacro1(e1, s1, _i); \
    GET_ELEM_ ## typeMacro2(e2, s2, _i); \
    _p = predExpr; \
    _mask = __ballot(_p); \
    if(laneID() == 1) { \
      _offset = atomicAdd(&_filteredLen, __popc(_mask)); \
    } \
    _offset = __shfl(_offset, 0); \
    if(_p) { \
      typer _r = resExpr; \
      _offset += __popc(_mask & __lanemask_lt()); \
      SET_ELEM_ ## typeMacror(_r, res, _offset); \
    } \
  } \
  res.len = _filteredLen; \
} while(0)

#define print_I(a) do{ \
  printf("%d ",a);   \
}while(0) \

#define print_F(a) do{ \
  printf("%f ",a);   \
}while(0) \

#define print_Tuple(res1, type1, type2) do{ \
  print_##type1(res1.a); \
  printf("\n"); \
  print_##type2(res1.b); \
  printf("\n"); \
}while(0) \

#define NESLDIST(res, p1, p2)  do{\
  int i =0;\
  MALLOC(res, p2, struct Sequence);\
  while(i<p2){\
    int elem = p1;\
    SET_ELEM_I(elem, res, i);\
    i++;\
  }\
}while(0)



#if __PF_COMPILER__ == 1 
#pragma pf device 
int RAND_I(int range);
void print_SEQ_I(struct Sequence src);
int isContiguousList(int start, int len, struct Sequence list);
struct Sequence genShuffledList(int start, int len);
#else
#include <curand_kernel.h>
#include <cuda.h>
__device__ int RAND_I(int range){
curandState state;
  int tmp;
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  //printf("hello ");
  curand_init(1337, idx, 0, &state);
  tmp =  (curand_uniform(&state)*10000);
  tmp %= range;
  //printf("idx:%d, tmp:%d\n",idx,tmp);
  return tmp;
}
__device__ void print_SEQ_I(struct Sequence src){
  int i,tmp;
  int len = src.len;
  //int idx = blockIdx.x * blockDim.x + threadIdx.x;
  //if(idx ==0){
  i=0;
  while(i<len){
    GET_ELEM_I(tmp,src,i);
    printf("%d, ",tmp);

    //printf("i:%d,elem:%d\n",i,((int*)src.ptr)[i]);
    i++;
  }
  printf("\n");
  //}
  //printf("idx:%d, tmp:%d\n",idx,tmp);
  return ;
}
__device__ int isContiguousList(int start, int len, struct Sequence list){
  int elm;
  int noerror=1;
  int i =0;
  while(i<len){// for(int i=0;i<len;i++){
    GET_ELEM_I(elm , list, i);
    if(elm!=start+i){
      printf("!!ERROR!! elm = %d, start:%d, i=%d\n",elm,start,i);
      noerror= 0;
      }
    i++;
  }
  if(noerror){
    printf("this is a Contiguous list.!\n");
  }
  return noerror;
}

__device__ struct Sequence genShuffledList(int start, int len){
  struct Sequence res;
  int i=0,idx=0;
  int rn1,rn2,tmp2,tmp1;
  MALLOC(res, len, struct Sequence);
  while(i<len){
    SET_ELEM_I(len+start-i-1,res,idx++);
    i++;
  }
  i=0;
  while(i<len){
    rn1 = RAND_I(start);
    GET_ELEM_I(tmp1,res,rn1); 
    rn2 = RAND_I(start);
    GET_ELEM_I(tmp2,res,rn2);
    SET_ELEM_I(tmp2,res,rn1);
    SET_ELEM_I(tmp1,res,rn2);
    i++;
  }
  return(res);
}


#endif

#define RAND_F(range) (((float)rand()/(float)(RAND_MAX)) * a)

