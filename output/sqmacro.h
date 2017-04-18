#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <math.h>
/* Reference count should stored with the array */
struct Sequence {
  int     len;    // Length
  int     cap;    // Capacity
  void   *ptr;
};


struct Pair_I {
  int   a;
  int   b;
};

struct Pair_F {
  float a;
  float b;
};

struct tupleFI {
  float a;
  int   b;
};

struct tupleIF {
  int   a;
  float b;
};

typedef struct {
  int     index;
  float   value;
} IFPair;

struct tupleSF {
  struct Sequence   a;
  float      b;
};
struct tupleFS {
  float      a;
  struct Sequence   b;
};
struct tupleSI {
  struct Sequence   a;
  int        b;
};
struct tupleIS {
  int        a;
  struct Sequence   b;
};
struct tuple{
  void *a;
  void *b;
};
// cpu speed. compute result in ms
#define CLOCKSPEED CLOCKS_PER_SEC*1000.0 
unsigned int seed = 123456789; 
int globalrefcount=0;
int globalmalloc=0;
int globalfree=0;

#define checkglobal() do{\
  printf("global:%d\n",globalrefcount);\
}while(0)

int atomicAdd(int * a, int b){
  int cnt = *a;
  printf("global:%d refcnt:%d Add:%d \t\t",globalrefcount,*a,b);
  globalrefcount++;
  *a +=b;
  printf("global:%d refcnt:%d\n",globalrefcount,*a);
  return cnt;
}
int atomicSub(int * a, int b){
  int cnt = *a;
  printf("global:%d refcnt:%d Sub:%d \t\t",globalrefcount,*a,b);
  globalrefcount--;
  *a -=b;
  printf("global:%d refcnt:%d\n",globalrefcount,*a);
  return cnt ;
}
//#define atomicAdd(a,n) (*a +n )
//#define atomicSub(a,n) (*a -n )

/* Macros to access sequences of various type
 * These should be generated automatically */
#define REFCNT(seq, type) ((int*)(((uint8_t*)seq.ptr)+seq.cap*sizeof(type)))

#define GET_ELEM_I(res, arr, idx) do { \
  res = ((int*)arr.ptr)[idx]; } while(0)

#define GET_ELEM_F(res, arr, idx) do { \
  res = ((float*)arr.ptr)[idx]; } while(0)

#define GET_ELEM_PAIR_IF(res, arr, idx) do { \
  res.a = ((int*)arr.ptr)[idx]; \
  res.b = ((float*)arr.ptr)[arr.cap+idx]; } while(0)

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
  assert(_refcnt > 0); \
  ((struct Sequence*)arr.ptr)[idx] = elm; \
} while(0)

/* We need the element type of the sub sequence so that we can increase the reference count */
#define SET_ELEM_SEQ_PAIR_F(elm, arr, idx) do { \
  int _refcnt = atomicAdd(REFCNT(elm, struct Pair_F), 1); \
  assert(_refcnt > 0); \
  ((struct Sequence*)arr.ptr)[idx] = elm; \
} while(0)

/* These can be put in the header file */
#define MALLOC(res, numElem, type) do { \
  res.len = numElem; \
  res.cap = numElem; \
  res.ptr = malloc(res.cap*sizeof(type)+sizeof(int)); \
  assert(res.ptr != 0); \
  *REFCNT(res, type) = 0; \
  globalmalloc++;\
} while(0)

#define FREE(res) do { \
  free(res.ptr); \
  globalfree++;\
} while(0)

#define MAKE_SEQ_2(res, type, typeMacro, e1, e2) do { \
  MALLOC(res, 2, type); \
  SET_ELEM_ ## typeMacro(e1, res, 0); \
  SET_ELEM_ ## typeMacro(e2, res, 1); \
} while(0)

#define DECREF_SEQ_I(seq) do { \
  int _refcnt = atomicSub(REFCNT(seq, int), 1);\
  assert(_refcnt < 100); \
  if(_refcnt == 1) { \
    FREE(seq); \
  }} while(0)

#define DECREF_SEQ_F(seq) do { \
  int _refcnt = atomicSub(REFCNT(seq, float), 1);\
  assert(_refcnt < 100); \
  if(_refcnt == 1) { \
    FREE(seq); \
  }} while(0)

#define DECREF_SEQ_PAIR_F(seq) do { \
  int _refcnt = atomicSub(REFCNT(seq, struct Pair_F), 1);\
  assert(_refcnt < 100); \
  if(_refcnt == 1) { \
    FREE(seq); \
  }} while(0)

#define DECREF_SEQ_SEQ_I(seq) do { \
  int _refcnt = atomicSub(REFCNT(seq, struct Sequence), 1); \
  assert(_refcnt < 100); \
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
  assert(_refcnt < 100); \
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
  _len = res.len; \
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
    GET_ELEM_ ## typeMacro(_e, s, _i); \
    SET_ELEM_ ## typeMacro(_e, res, _i+1); \
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
    int _len=_s.len, _j; \
    for(_j=0; _j<_len; _j++) { \
      type _elem; \
      GET_ELEM_ ## typeMacro(_elem, _s, _j); \
      SET_ELEM_ ## typeMacro(_elem, res, _j+_numCopied); \
    } \
    _numCopied += _s.len; \
    _i++; \
  }} while(0)

/* sequential version FILTER_1*/
#define FILTER_1(res, resExpr, typer, typeMacror, s1, e1, type1, typeMacro1, predExpr) do { \
  int  _len=s1.len, _i, _j; \
  MALLOC(res, _len, typer); \
  _j =0; \
  for(_i=0; _i<_len; _i++) { \
    type1 e1; \
    bool _p; \
    GET_ELEM_ ## typeMacro1(e1, s1, _i); \
    _p = predExpr; \
    if(_p) { \
      typer _r = resExpr; \
      SET_ELEM_ ## typeMacror(_r, res, _j); \
      _j++; \
    } \
  } \
  res.len = _j; \
} while(0)

#define FILTER_TUPLE_1( res,resExpr, child1, child2, ctype1, ctype2,\
                        typer, typeMacror, \
                        s1, e1, type1, typeMacro1,\
                        predExpr) do { \
  int  _len=s1.len, _i, _j; \
  MALLOC(res, _len, typer); \
  _j =0; \
  for(_i=0; _i<_len; _i++) { \
    type1 e1; \
    ctype1 child1;\
    ctype2 child2;\
    bool _p; \
    GET_ELEM_ ## typeMacro1(e1, s1, _i); \
    child1 = e1.a;\
    child2 = e1.b;\
    _p = predExpr; \
    if(_p) { \
      typer _r = resExpr; \
      SET_ELEM_ ## typeMacror(_r, res, _j); \
      _j++; \
    } \
  } \
  res.len = _j; \
} while(0)
#define FILTER_2(res, resExpr, typer, typeMacror, s1, e1, type1, typeMacro1, s2, e2, type2, typeMacro2, predExpr) do { \
  int _filteredLen=0, _len=s1.len, _i,_j; \
  MALLOC(res, _len, typer);   \
  _j=0;\
  for(_i=0; _i<_len; _i++) { \
    type1 e1; \
    type2 e2; \
    bool _p; \
    GET_ELEM_ ## typeMacro1(e1, s1, _i); \
    GET_ELEM_ ## typeMacro2(e2, s2, _i); \
    _p = predExpr; \
    if(_p) { \
      typer _r = resExpr; \
      SET_ELEM_ ## typeMacror(_r, res, _j); \
      _j++;\
    } \
  } \
  res.len = _j; \
} while(0)

#define print_I(a) do{ \
  printf("%d",a);   \
}while(0) \

#define print_F(a) do{ \
  printf("%f ",a);   \
}while(0) \


#define print_PAIR_F(src) do{\
  print_F(src.a);\
  printf(", ");\
  print_F(src.b);\
}while(0)

#define print_SEQ_PAIR_F(src)do{\
  int _i,_len;\
  struct Pair_F e;\
  _len = src.len;\
  for(_i=0; _i<_len; _i++) { \
    GET_ELEM_PAIR_F(e, src, _i); \
    printf("[ ");\
    print_PAIR_F(e);\
    printf("], ");\
  }\
  printf("\n"); \
}while(0)

#define print_SEQ_I(src) do{ \
  int i,e,_len; \
  printf("printSEQ %s, len=%d: \n",#src,src.len); \
  _len = src.len;\
  for(i=0; i<_len; i++) { \
    GET_ELEM_I(e, src, i); \
    print_I(e);\
    printf(", ");\
  }\
  printf("\n"); \
}while(0)

#define print_Tuple(res1, type1, type2) do{ \
  print_##type1(res1.a); \
  printf("\n"); \
  print_##type2(res1.b); \
  printf("\n"); \
}while(0) 

#define NESLDIST_F(res, p1, p2)  do{\
  int i;\
  MALLOC(res, p2, float);\
  for(i=0; i<p2; i++) { \
    float elem = p1;\
    SET_ELEM_F(elem, res, i);\
  }\
}while(0)


#define NESLDIST(res, p1, p2)  do{\
  int i;\
  MALLOC(res, p2, int);\
  for(i=0; i<p2; i++) { \
    int elem = p1;\
    SET_ELEM_I(elem, res, i);\
  }\
}while(0)

#define DECT1T2 clock_t _t1, _t2;
#define CLOCK() (clock())
//#define RAND_F(range) (((float)rand()/(float)(RAND_MAX)) * range)


#define NESLRAND_SEQ(res, len, src, p1, typer) do{\
  MALLOC(res, len, struct Sequence);\
  srand(time(0));\
  RAND_##typer(res, src);\
}while(0)

#define MALLOC_HEAP_SIZE 1000000
#define SET_HEAP_SIZE(size) do { \
}while(0)
#define CUDA_ERROR_CHECK() do{\
}while(0)

bool plusp(float x) { return x>0;}

IFPair max(IFPair a, IFPair b) {
  return a.value>b.value ? a : b;
}

IFPair min(IFPair a, IFPair b) {
  return a.value<b.value ? a : b;
}


int max_index_f(struct Sequence s) {
  int i, len=s.len, res;
  IFPair maxIFP = {0, -INFINITY};

  for(i=0; i<len; i++) {
    float val;
    IFPair currentIFP;

    GET_ELEM_F(val, s, i);

    currentIFP.index = i;
    currentIFP.value = val;

    maxIFP = max(maxIFP, currentIFP);
  }

  res = maxIFP.index;

  return res;
}

int min_index_f(struct Sequence s) {
  int i, len=s.len, res;
  IFPair minIFP = {0, INFINITY};

  for(i=0; i<len; i++) {
    float val;
    IFPair currentIFP;

    GET_ELEM_F(val, s, i);

    currentIFP.index = i;
    currentIFP.value = val;

    minIFP = min(minIFP, currentIFP);
  }

  res = minIFP.index;

  return res;
}

unsigned int myrand(){
  seed=(1103515245 *seed+12345)%4294967296;
  return seed;
}

float RAND_F(float range) {
  float x = ((float)myrand()/(float)4294967296.0)*range ;
  printf("rnf:%f\n",x);
  return x; 
}
unsigned int RAND_I(int range) {
  return myrand()%range; 
}

int isContiguousList(int start, int len, struct Sequence list){
  int elm;
  int noerror=1;
  for(int i=0;i<len;i++){
    GET_ELEM_I(elm , list, i);
    if(elm!=start+i){
      //printf("!!ERROR!! elm = %d, start:%d, i=%d\n",elm,start,i);
      noerror= 0;
      }
  }
  if(noerror){
    printf("this is a Contiguous list.!\n");
  }
  return noerror;
}

struct Sequence genReverseList(int len){
  struct Sequence res;
  int i=0;
  MALLOC(res, len, int); 
  while(i<len){
   SET_ELEM_I(len-i-1,res,i);
    i++;
  }
  return res;
}
struct Sequence genRandomArray(int len){
  struct Sequence res;
  int i,idx=0;
  int tmp1,tmp2,elm,base;
  int rn1,rn2;
  base = myrand()%1000;
  MALLOC(res, len, int);
  for (i =0 ;i<len;i++){
    elm = len-i-1+base;
    SET_ELEM_I(elm,res,idx++);
  }
  for (i =0 ;i<len;i++){
    rn1 = myrand()%len;
    rn2 = myrand()%len;
    GET_ELEM_I(tmp1,res,rn1); 
    GET_ELEM_I(tmp2,res,rn2);
    SET_ELEM_I(tmp2,res,rn1);
    SET_ELEM_I(tmp1,res,rn2);
  }
  return(res);
}
struct Sequence genShuffledList(int start, int len){
  struct Sequence res;
  int i,idx=0;
  int tmp1,tmp2;
  int rn1,rn2;
  MALLOC(res, len, int);
  for (i =0 ;i<len;i++){
    SET_ELEM_I(len+start-i-1,res,idx++);
  }
  seed = 123456789;
  for (i =0 ;i<len;i++){
    rn1 = myrand()%len;
    rn2 = myrand()%len;
    GET_ELEM_I(tmp1,res,rn1); 
    GET_ELEM_I(tmp2,res,rn2);
    SET_ELEM_I(tmp2,res,rn1);
    SET_ELEM_I(tmp1,res,rn2);
  }
  return(res);
}


/* below for testing usage. */
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

/* below for checking if parallel loop is parallelizable. */ 
int getSeed(int *ptr) {
  unsigned long long seed = (unsigned long long)ptr;
  srand(seed);

  return rand();
}

int loopStart(int lb, int ub, int *idx) {
  int seed = getSeed(idx);

  if(seed%2 == 0) {
    return lb;
  }
  else {
    return ub-1;
  }
}

int loopEnd(int lb, int ub, int *idx) {
  int seed = getSeed(idx);

  if(seed%2 == 0) {
    return ub;
  }
  else {
    return lb-1;
  }
}

int loopNext(int lb, int ub, int *idx) {
  int seed = getSeed(idx);

  if(seed%2 == 0) {
    return *idx+1;
  }
  else {
    return *idx-1;
  }
}

#define PARALLEL_LOOP(lb, ub, idx)  for(idx=loopStart(lb, ub, &idx); idx!=loopEnd(lb, ub, &idx); idx=loopNext(lb, ub, &idx))
