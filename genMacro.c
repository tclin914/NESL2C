#include <stdio.h>
#include "genMacro.h"

void genMacro(FILE* fptr){
  fprintf(fptr, "#define REFCNT(seq, type) ((int*)(((uint8_t*)seq.ptr)+seq.cap*sizeof(type)))\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define GET_ELEM_I(res, arr, idx) do { \\\n");
  fprintf(fptr, "    res = ((int*)arr.ptr)[idx]; } while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define GET_ELEM_F(res, arr, idx) do { \\\n");
  fprintf(fptr, "    res = ((float*)arr.ptr)[idx]; } while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define GET_ELEM_PAIR_F(res, arr, idx) do { \\\n");
  fprintf(fptr, "    res.a = ((float*)arr.ptr)[idx]; \\\n");
  fprintf(fptr, "    res.b = ((float*)arr.ptr)[arr.cap+idx]; } while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define GET_ELEM_SEQ_I(res, arr, idx) do { \\\n");
  fprintf(fptr, "    res = ((struct Sequence*)arr.ptr)[idx]; \\\n");
  fprintf(fptr, "    } while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define GET_ELEM_SEQ_PAIR_F(res, arr, idx) do { \\\n");
  fprintf(fptr, "    res = ((struct Sequence*)arr.ptr)[idx]; \\\n");
  fprintf(fptr, "    } while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define SET_ELEM_I(elm, arr, idx) do { \\\n");
  fprintf(fptr, "    ((int*)arr.ptr)[idx] = elm; \\\n");
  fprintf(fptr, "    } while(0)\n");
  fprintf(fptr, "    \n");
  fprintf(fptr, "#define SET_ELEM_F(elm, arr, idx) do { \\\n");
  fprintf(fptr, "    ((float*)arr.ptr)[idx] = elm; \\\n");
  fprintf(fptr, "    } while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define SET_ELEM_PAIR_F(elm, arr, idx) do { \\\n");
  fprintf(fptr, "    ((float*)arr.ptr)[idx] = elm.a; \\\n");
  fprintf(fptr, "    ((float*)arr.ptr)[arr.cap+idx] = elm.b; } while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define SET_ELEM_SEQ_I(elm, arr, idx) do { \\\n");
  fprintf(fptr, "    int _refcnt = atomicAdd(REFCNT(elm, int), 1); \\\n");
  fprintf(fptr, "    assert(_refcnt > 0); \\\n");
  fprintf(fptr, "    ((struct Sequence*)arr.ptr)[idx] = elm; \\\n");
  fprintf(fptr, "} while(0)\n");
  fprintf(fptr, "    \n");
  fprintf(fptr, "/* We need the element type of the sub sequence so that we can increase the reference count */\n");
  fprintf(fptr, "#define SET_ELEM_SEQ_PAIR_F(elm, arr, idx) do { \\\n");
  fprintf(fptr, "    int _refcnt = atomicAdd(REFCNT(elm, struct Pair_F), 1); \\\n");
  fprintf(fptr, "    assert(_refcnt > 0); \\\n");
  fprintf(fptr, "    ((struct Sequence*)arr.ptr)[idx] = elm; \\\n");
  fprintf(fptr, "} while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "/* These can be put in the header file */\n");
  fprintf(fptr, "#define MALLOC(res, numElem, type) do { \\\n");
  fprintf(fptr, "    res.len = numElem; \\\n");
  fprintf(fptr, "    res.cap = numElem; \\\n");
  fprintf(fptr, "    res.ptr = malloc(res.cap*sizeof(type)+sizeof(int)); \\\n");
  fprintf(fptr, "    assert(res.ptr != 0); \\\n");
  fprintf(fptr, "    *REFCNT(res, type) = 0; \\\n");
  fprintf(fptr, "    } while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define FREE(res) do { \\\n");
  fprintf(fptr, "    free(res.ptr); } while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define MAKE_SEQ_2(res, type, typeMacro, e1, e2) do { \\\n");
  fprintf(fptr, "    MALLOC(res, 2, type); \\\n");
  fprintf(fptr, "    SET_ELEM_ ## typeMacro(e1, res, 0); \\\n");
  fprintf(fptr, "    SET_ELEM_ ## typeMacro(e2, res, 1); \\\n");
  fprintf(fptr, "    } while(0)\n");
  fprintf(fptr, "    \n");
  fprintf(fptr, "#define DECREF_SEQ_I(seq) do { \\\n");
  fprintf(fptr, "    int _refcnt = atomicSub(REFCNT(seq, int), 1);\\\n");
  fprintf(fptr, "    assert(_refcnt < 100); \\\n");
  fprintf(fptr, "    if(_refcnt == 1) { \\\n");
  fprintf(fptr, "        FREE(seq); \\\n");
  fprintf(fptr, "    }} while(0)\n");
  fprintf(fptr, "    \n");
  fprintf(fptr, "#define DECREF_SEQ_F(seq) do { \\\n");
  fprintf(fptr, "    int _refcnt = atomicSub(REFCNT(seq, float), 1);\\\n");
  fprintf(fptr, "    assert(_refcnt < 100); \\\n");
  fprintf(fptr, "    if(_refcnt == 1) { \\\n");
  fprintf(fptr, "        FREE(seq); \\\n");
  fprintf(fptr, "    }} while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define DECREF_SEQ_PAIR_F(seq) do { \\\n");
  fprintf(fptr, "    int _refcnt = atomicSub(REFCNT(seq, struct Pair_F), 1);\\\n");
  fprintf(fptr, "    assert(_refcnt < 100); \\\n");
  fprintf(fptr, "    if(_refcnt == 1) { \\\n");
  fprintf(fptr, "        FREE(seq); \\\n");
  fprintf(fptr, "    }} while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define DECREF_SEQ_SEQ_I(seq) do { \\\n");
  fprintf(fptr, "    int _refcnt = atomicSub(REFCNT(seq, struct Sequence), 1); \\\n");
  fprintf(fptr, "    assert(_refcnt < 100); \\\n");
  fprintf(fptr, "    if(_refcnt == 1) { \\\n");
  fprintf(fptr, "        struct Sequence *_subseq = (struct Sequence*)seq.ptr; \\\n");
  fprintf(fptr, "        int _i = 0; \\\n");
  fprintf(fptr, "        while(_i < seq.len) { \\\n");
  fprintf(fptr, "            DECREF_SEQ_I(_subseq[_i]); \\\n");
  fprintf(fptr, "            _i++; \\\n");
  fprintf(fptr, "        } \\\n");
  fprintf(fptr, "        FREE(seq); \\\n");
  fprintf(fptr, "    }} while(0)\n");
  fprintf(fptr, "    \n");
  fprintf(fptr, "#define DECREF_SEQ_SEQ_PAIR_F(seq) do { \\\n");
  fprintf(fptr, "    int _refcnt = atomicSub(REFCNT(seq, struct Sequence), 1); \\\n");
  fprintf(fptr, "    assert(_refcnt < 100); \\\n");
  fprintf(fptr, "    if(_refcnt == 1) { \\\n");
  fprintf(fptr, "        struct Sequence *_subseq = (struct Sequence*)seq.ptr; \\\n");
  fprintf(fptr, "        int _i = 0; \\\n");
  fprintf(fptr, "        while(_i < seq.len) { \\\n");
  fprintf(fptr, "            DECREF_SEQ_PAIR_F(_subseq[_i]); \\\n");
  fprintf(fptr, "            _i++; \\\n");
  fprintf(fptr, "        } \\\n");
  fprintf(fptr, "        FREE(seq); \\\n");
  fprintf(fptr, "    }} while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define CONCAT(s1, s2, res, type, typeMacro) { \\\n");
  fprintf(fptr, "    int _i, _len; \\\n");
  fprintf(fptr, "    MALLOC(res, s1.len+s2.len, type); \\\n");
  fprintf(fptr, "    _len = res.len; _NEWLINE_ \\\n");
  fprintf(fptr, "    _SHARP_ pragma pf parallel_mc _NEWLINE_ \\\n");
  fprintf(fptr, "    for(_i=0; _i<_len; _i++) { \\\n");
  fprintf(fptr, "        type _elem; \\\n");
  fprintf(fptr, "        if(_i < s1.len) \\\n");
  fprintf(fptr, "            GET_ELEM_ ## typeMacro(_elem, s1, _i); \\\n");
  fprintf(fptr, "        else \\\n");
  fprintf(fptr, "            GET_ELEM_ ## typeMacro(_elem, s2, _i-s1.len); \\\n");
  fprintf(fptr, "        SET_ELEM_ ## typeMacro(_elem, res, _i); \\\n");
  fprintf(fptr, "    }}\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "/* Used to prepend an element to a sequence */\n");
  fprintf(fptr, "#define PREPEND(elem, s, res, type, typeMacro) do { \\\n");
  fprintf(fptr, "    int _i = 0; \\\n");
  fprintf(fptr, "    MALLOC(res, s.len+1, type); \\\n");
  fprintf(fptr, "    SET_ELEM_ ## typeMacro(elem, res, 0); \\\n");
  fprintf(fptr, "    while(_i < s.len) { \\\n");
  fprintf(fptr, "        type _e; \\\n");
  fprintf(fptr, "        GET_ELEM_ ## typeMacro(_e, s, _i); \\\n");
  fprintf(fptr, "        SET_ELEM_ ## typeMacro(_e, res, _i+1); \\\n");
  fprintf(fptr, "        _i++; \\\n");
  fprintf(fptr, "    }} while(0)\n");
  fprintf(fptr, "\n");
  fprintf(fptr, "#define FLATTEN(seq, res, type, typeMacro) do { \\\n");
  fprintf(fptr, "    int _i=0, _numElem=0, _numCopied=0; \\\n");
  fprintf(fptr, "    struct Sequence *_subseq = ((struct Sequence*)seq.ptr); \\\n");
  fprintf(fptr, "    while(_i < seq.len) { \\\n");
  fprintf(fptr, "        _numElem += _subseq[_i].len; \\\n");
  fprintf(fptr, "        _i++; \\\n");
  fprintf(fptr, "    } \\\n");
  fprintf(fptr, "    MALLOC(res, _numElem, type); \\\n");
  fprintf(fptr, "    _i = 0; \\\n");
  fprintf(fptr, "    while(_i < seq.len) { \\\n");
  fprintf(fptr, "        struct Sequence _s = _subseq[_i]; \\\n");
  fprintf(fptr, "        int _len=_s.len, _j; _NEWLINE_ \\\n");
  fprintf(fptr, "        _SHARP_ pragma pf parallel_mc _NEWLINE_ \\\n");
  fprintf(fptr, "        for(_j=0; _j<_len; _j++) { \\\n");
  fprintf(fptr, "            type _elem; \\\n");
  fprintf(fptr, "            GET_ELEM_ ## typeMacro(_elem, _s, _j); \\\n");
  fprintf(fptr, "            SET_ELEM_ ## typeMacro(_elem, res, _j+_numCopied); \\\n");
  fprintf(fptr, "        } \\\n");
  fprintf(fptr, "        _numCopied += _s.len; \\\n");
  fprintf(fptr, "        _i++; \\\n");
  fprintf(fptr, "    }} while(0)\n");
  fprintf(fptr, "    \n\n");
  fprintf(fptr, "#define FILTER_1(res, resExpr, typer, typeMacror, s1, e1, type1, typeMacro1, predExpr) do { \\\n");
  fprintf(fptr, "    int _filteredLen=0, _len=s1.len, _i; \\\n");
  fprintf(fptr, "    MALLOC(res, _len, typer); _NEWLINE_ \\\n");
  fprintf(fptr, "    _SHARP_ pragma pf parallel_mc _NEWLINE_ \\\n");
  fprintf(fptr, "    for(_i=0; _i<_len; _i++) { \\\n");
  fprintf(fptr, "        type1 e1; \\\n");
  fprintf(fptr, "        uint32_t _mask, _offset; \\\n");
  fprintf(fptr, "        bool _p; \\\n");
  fprintf(fptr, "        GET_ELEM_ ## typeMacro1(e1, s1, _i); \\\n");
  fprintf(fptr, "        _p = predExpr; \\\n");
  fprintf(fptr, "        _mask = __ballot(_p); \\\n");
  fprintf(fptr, "        if(laneID() == 0) { \\\n");
  fprintf(fptr, "            _offset = atomicAdd(&_filteredLen, __popc(_mask)); \\\n");
  fprintf(fptr, "        } \\\n");
  fprintf(fptr, "        _offset = __shfl(_offset, 0); \\\n");
  fprintf(fptr, "        if(_p) { \\\n");
  fprintf(fptr, "            typer _r = resExpr; \\\n");
  fprintf(fptr, "            _offset += __popc(_mask & __lanemask_lt()); \\\n");
  fprintf(fptr, "            SET_ELEM_ ## typeMacror(_r, res, _offset); \\\n");
  fprintf(fptr, "        } \\\n");
  fprintf(fptr, "    } \\\n");
  fprintf(fptr, "    res.len = _filteredLen; \\\n");
  fprintf(fptr, "    } while(0)\n");
  fprintf(fptr, " \n");
  fprintf(fptr, "#define FILTER_2(res, resExpr, typer, typeMacror, s1, e1, type1, typeMacro1, s2, e2, type2, typeMacro2, predExpr) do { \\\n");
  fprintf(fptr, "    int _filteredLen=0, _len=s1.len, _i; \\\n");
  fprintf(fptr, "    MALLOC(res, _len, typer); _NEWLINE_ \\\n");
  fprintf(fptr, "    _SHARP_ pragma pf parallel_mc _NEWLINE_ \\\n");
  fprintf(fptr, "    for(_i=0; _i<_len; _i++) { \\\n");
  fprintf(fptr, "        type1 e1; \\\n");
  fprintf(fptr, "        type2 e2; \\\n");
  fprintf(fptr, "        uint32_t _mask, _offset; \\\n");
  fprintf(fptr, "        bool _p; \\\n");
  fprintf(fptr, "        GET_ELEM_ ## typeMacro1(e1, s1, _i); \\\n");
  fprintf(fptr, "        GET_ELEM_ ## typeMacro2(e2, s2, _i); \\\n");
  fprintf(fptr, "        _p = predExpr; \\\n");
  fprintf(fptr, "        _mask = __ballot(_p); \\\n");
  fprintf(fptr, "        if(laneID() == 0) { \\\n");
  fprintf(fptr, "            _offset = atomicAdd(&_filteredLen, __popc(_mask)); \\\n");
  fprintf(fptr, "        } \\\n");
  fprintf(fptr, "        _offset = __shfl(_offset, 0); \\\n");
  fprintf(fptr, "        if(_p) { \\\n");
  fprintf(fptr, "            typer _r = resExpr; \\\n");
  fprintf(fptr, "            _offset += __popc(_mask & __lanemask_lt()); \\\n");
  fprintf(fptr, "            SET_ELEM_ ## typeMacror(_r, res, _offset); \\\n");
  fprintf(fptr, "        } \\\n");
  fprintf(fptr, "    } \\\n");
  fprintf(fptr, "    res.len = _filteredLen; \\\n");
  fprintf(fptr, "    } while(0)\n");

}
