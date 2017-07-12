/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     FUNCTION = 258,
     DATATYPE = 259,
     NUMBER = 260,
     ORDINAL = 261,
     LOGICAL = 262,
     ANY = 263,
     IN = 264,
     IF = 265,
     ELSE = 266,
     THEN = 267,
     LET = 268,
     AND = 269,
     NAND = 270,
     NOR = 271,
     XOR = 272,
     OR = 273,
     PI = 274,
     PP = 275,
     RARROW = 276,
     LARROW = 277,
     EQ = 278,
     NE = 279,
     LE = 280,
     GE = 281,
     INT = 282,
     BOOL = 283,
     FLOAT = 284,
     CHAR = 285,
     TRUE_CONST = 286,
     FALSE_CONST = 287,
     INT_CONST = 288,
     FLOAT_CONST = 289,
     STRING_CONST = 290,
     ID = 291,
     UMINUS = 292
   };
#endif
/* Tokens.  */
#define FUNCTION 258
#define DATATYPE 259
#define NUMBER 260
#define ORDINAL 261
#define LOGICAL 262
#define ANY 263
#define IN 264
#define IF 265
#define ELSE 266
#define THEN 267
#define LET 268
#define AND 269
#define NAND 270
#define NOR 271
#define XOR 272
#define OR 273
#define PI 274
#define PP 275
#define RARROW 276
#define LARROW 277
#define EQ 278
#define NE 279
#define LE 280
#define GE 281
#define INT 282
#define BOOL 283
#define FLOAT 284
#define CHAR 285
#define TRUE_CONST 286
#define FALSE_CONST 287
#define INT_CONST 288
#define FLOAT_CONST 289
#define STRING_CONST 290
#define ID 291
#define UMINUS 292




/* Copy the first part of user declarations.  */
#line 1 "nesl.y"


#include "OpCode.h"

#include "Node.h"
// Nulary Node
#include "ConstantBoolean.h"
#include "ConstantInteger.h"
#include "ConstantFloat.h"
#include "ConstantString.h"
// Binary Node
#include "ArithmeticOpFactory.h"
#include "RelationalOpFactory.h"

using namespace nesl2c;

void yyerror(const char *s);
Node* yyheader;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern int yylineno;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 84 "nesl.y"
{
  int tokenval;
  char *tokenstr;
  nesl2c::OpCode opcode;
  Node* node;
}
/* Line 193 of yacc.c.  */
#line 203 "nesl.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 216 "nesl.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  57
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   337

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  100
/* YYNRULES -- Number of states.  */
#define YYNSTATES  173

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    45,    53,     2,     2,     2,
      50,    51,    42,    40,    37,    41,     2,    43,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    48,    52,
      38,    49,    39,     2,    46,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    54,     2,    55,    44,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    56,    58,    57,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    47
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    19,    26,    33,    38,
      41,    43,    45,    47,    49,    53,    55,    60,    64,    68,
      72,    74,    78,    80,    82,    84,    91,    97,   102,   104,
     108,   112,   114,   118,   120,   122,   126,   128,   130,   132,
     134,   136,   140,   142,   144,   146,   150,   152,   154,   156,
     158,   160,   162,   164,   168,   170,   172,   174,   176,   178,
     182,   184,   186,   188,   190,   194,   196,   199,   201,   203,
     205,   207,   212,   214,   219,   223,   229,   233,   238,   242,
     244,   249,   251,   255,   257,   261,   263,   265,   269,   271,
     273,   275,   277,   279,   282,   287,   288,   290,   292,   294,
     296
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      60,     0,    -1,    61,    -1,    62,    -1,    61,    62,    -1,
       3,    63,    69,    48,    65,    49,    69,    64,    -1,     3,
      63,    69,    49,    69,    64,    -1,     4,    36,    50,    68,
      51,    64,    -1,    69,    49,    69,    64,    -1,    69,    64,
      -1,    36,    -1,    90,    -1,    52,    -1,    53,    -1,    66,
      21,    66,    -1,    94,    -1,    36,    50,    68,    51,    -1,
      54,    66,    55,    -1,    50,    67,    51,    -1,    66,    37,
      67,    -1,    66,    -1,    68,    37,    66,    -1,    66,    -1,
      70,    -1,    73,    -1,    10,    69,    12,    69,    11,    69,
      -1,    13,    71,    52,     9,    69,    -1,    13,    71,     9,
      69,    -1,    72,    -1,    71,    52,    72,    -1,    69,    49,
      69,    -1,    75,    -1,    75,    37,    74,    -1,    73,    -1,
      70,    -1,    75,    76,    77,    -1,    77,    -1,    18,    -1,
      16,    -1,    17,    -1,    79,    -1,    77,    78,    79,    -1,
      14,    -1,    15,    -1,    81,    -1,    79,    80,    81,    -1,
      23,    -1,    24,    -1,    38,    -1,    39,    -1,    25,    -1,
      26,    -1,    83,    -1,    81,    82,    83,    -1,    40,    -1,
      41,    -1,    20,    -1,    22,    -1,    85,    -1,    83,    84,
      85,    -1,    42,    -1,    43,    -1,    21,    -1,    86,    -1,
      85,    44,    86,    -1,    88,    -1,    87,    86,    -1,    45,
      -1,    46,    -1,    41,    -1,    89,    -1,    89,    54,    69,
      55,    -1,    96,    -1,    90,    50,    69,    51,    -1,    56,
      91,    57,    -1,    56,    91,    58,    69,    57,    -1,    54,
      55,    66,    -1,    54,    69,    95,    55,    -1,    50,    69,
      51,    -1,    36,    -1,    94,    50,    69,    51,    -1,     8,
      -1,    69,    48,    92,    -1,    92,    -1,    92,    52,    93,
      -1,    93,    -1,    36,    -1,    69,     9,    69,    -1,    36,
      -1,    27,    -1,    29,    -1,    28,    -1,    30,    -1,    48,
      69,    -1,    48,    69,    48,    69,    -1,    -1,    33,    -1,
      34,    -1,    31,    -1,    32,    -1,    35,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    96,    96,   103,   107,   116,   120,   124,   128,   134,
     141,   146,   153,   154,   158,   167,   171,   175,   179,   186,
     190,   197,   201,   208,   212,   219,   226,   232,   241,   245,
     254,   263,   267,   277,   281,   288,   294,   301,   305,   309,
     316,   320,   329,   333,   340,   344,   351,   355,   359,   363,
     367,   371,   378,   382,   389,   393,   397,   401,   408,   412,
     419,   423,   427,   434,   438,   447,   451,   459,   463,   467,
     474,   478,   487,   491,   495,   499,   505,   510,   516,   520,
     525,   534,   541,   547,   554,   560,   567,   572,   581,   586,
     590,   594,   598,   605,   610,   616,   622,   626,   630,   634,
     638
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FUNCTION", "DATATYPE", "NUMBER",
  "ORDINAL", "LOGICAL", "ANY", "IN", "IF", "ELSE", "THEN", "LET", "AND",
  "NAND", "NOR", "XOR", "OR", "PI", "PP", "RARROW", "LARROW", "EQ", "NE",
  "LE", "GE", "INT", "BOOL", "FLOAT", "CHAR", "TRUE_CONST", "FALSE_CONST",
  "INT_CONST", "FLOAT_CONST", "STRING_CONST", "ID", "','", "'<'", "'>'",
  "'+'", "'-'", "'*'", "'/'", "'^'", "'#'", "'@'", "UMINUS", "':'", "'='",
  "'('", "')'", "';'", "'$'", "'['", "']'", "'{'", "'}'", "'|'", "$accept",
  "Goal", "TopLevels", "TopLevel", "FunId", "EndMark", "FunTypeDef",
  "TypeExp", "PairTypes", "TypeList", "Exp", "IfOrLetExp", "ExpBinds",
  "ExpBind", "TupleExp", "TupleRest", "OrExp", "OrOp", "AndExp", "AndOp",
  "RelExp", "RelOp", "AddExp", "AddOp", "MulExp", "MulOp", "ExpExp",
  "UnExp", "UnOp", "SubscriptExp", "AtomicExp", "SpecialId", "ApplyBody",
  "RBinds", "RBind", "id", "SequenceTail", "Const", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,    44,    60,    62,
      43,    45,    42,    47,    94,    35,    64,   292,    58,    61,
      40,    41,    59,    36,    91,    93,   123,   125,   124
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    59,    60,    61,    61,    62,    62,    62,    62,    62,
      63,    63,    64,    64,    65,    66,    66,    66,    66,    67,
      67,    68,    68,    69,    69,    70,    70,    70,    71,    71,
      72,    73,    73,    74,    74,    75,    75,    76,    76,    76,
      77,    77,    78,    78,    79,    79,    80,    80,    80,    80,
      80,    80,    81,    81,    82,    82,    82,    82,    83,    83,
      84,    84,    84,    85,    85,    86,    86,    87,    87,    87,
      88,    88,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    90,    91,    91,    92,    92,    93,    93,    94,    94,
      94,    94,    94,    95,    95,    95,    96,    96,    96,    96,
      96
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     8,     6,     6,     4,     2,
       1,     1,     1,     1,     3,     1,     4,     3,     3,     3,
       1,     3,     1,     1,     1,     6,     5,     4,     1,     3,
       3,     1,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     3,     1,     2,     1,     1,     1,
       1,     4,     1,     4,     3,     5,     3,     4,     3,     1,
       4,     1,     3,     1,     3,     1,     1,     3,     1,     1,
       1,     1,     1,     2,     4,     0,     1,     1,     1,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,    81,     0,     0,    89,    91,    90,    92,
      98,    99,    96,    97,   100,    79,    69,    67,    68,     0,
       0,     0,     0,     2,     3,     0,    23,    24,    31,    36,
      40,    44,    52,    58,    63,     0,    65,    70,     0,     0,
      72,    10,     0,    11,     0,     0,     0,     0,    28,     0,
       0,    95,    79,     0,     0,    83,    85,     1,     4,     0,
      12,    13,     9,    38,    39,    37,     0,     0,    42,    43,
       0,    46,    47,    50,    51,    48,    49,     0,    56,    57,
      54,    55,     0,    62,    60,    61,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    78,    88,
       0,     0,    76,    15,     0,     0,     0,     0,    74,     0,
       0,     0,    34,    33,    32,    35,    41,    45,    53,    59,
      64,     0,     0,     0,     0,     0,    22,     0,     0,    30,
      27,     0,    29,     0,    20,     0,     0,    93,    77,    87,
       0,    82,     0,    84,     8,    71,    73,    80,     0,     0,
       0,     0,     0,     0,    26,     0,     0,    18,    17,     0,
      75,     0,     0,     6,    21,     7,    25,    16,    19,    94,
       0,    14,     5
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    22,    23,    24,    42,    62,   148,   126,   135,   127,
      25,    26,    47,    48,    27,   114,    28,    67,    29,    70,
      30,    77,    31,    82,    32,    86,    33,    34,    35,    36,
      37,    38,    54,    55,    56,    39,   105,    40
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -110
static const yytype_int16 yypact[] =
{
     131,    -3,   -22,  -110,   221,   221,  -110,  -110,  -110,  -110,
    -110,  -110,  -110,  -110,  -110,   -27,  -110,  -110,  -110,   221,
     161,   251,    25,   131,  -110,    20,  -110,  -110,    -6,    21,
      40,     4,   -13,   -10,  -110,   281,  -110,   -17,     8,    24,
    -110,  -110,   221,  -110,    26,    63,     3,    -5,  -110,    30,
      -8,    34,    10,     0,   -18,    31,  -110,  -110,  -110,   221,
    -110,  -110,  -110,  -110,  -110,  -110,   221,   281,  -110,  -110,
     281,  -110,  -110,  -110,  -110,  -110,  -110,   281,  -110,  -110,
    -110,  -110,   281,  -110,  -110,  -110,   281,   281,  -110,   221,
     221,   221,     2,    -8,   221,   221,   221,   191,  -110,    38,
      -8,    -8,  -110,  -110,   221,    29,   221,   251,  -110,   221,
     251,    18,  -110,  -110,  -110,    21,    40,     4,   -13,   -10,
    -110,    39,    44,    45,    -8,   221,  -110,   -24,    86,  -110,
    -110,   221,  -110,    -8,    61,    48,    46,    60,  -110,  -110,
     101,    31,    54,  -110,  -110,  -110,  -110,  -110,    64,    93,
      18,    -8,    18,   221,  -110,   -19,    -8,  -110,  -110,   221,
    -110,   221,    -8,  -110,  -110,  -110,  -110,  -110,  -110,  -110,
      18,  -110,  -110
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -110,  -110,  -110,    94,  -110,  -109,  -110,   -47,   -40,   -14,
      -4,    56,  -110,    23,    57,  -110,  -110,  -110,    58,  -110,
      59,  -110,    47,  -110,    49,  -110,    42,   -28,  -110,  -110,
    -110,   125,  -110,    33,    22,   -44,  -110,  -110
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -89
static const yytype_int16 yytable[] =
{
      45,    46,   144,   102,    96,     3,   103,    88,    83,   106,
      63,    64,    65,   151,    44,    49,    51,    53,   151,     6,
       7,     8,     9,   -88,    78,    57,    79,   152,    99,    84,
      85,    66,   167,    41,    87,    68,    69,    89,    92,   108,
     109,   163,   100,   165,    80,    81,   101,    97,   107,   103,
     124,   125,    95,   134,   136,   111,   103,   103,    90,   120,
     -88,   172,   -86,    71,    72,    73,    74,   -86,   -86,    59,
      60,    61,    60,    61,    91,    94,    93,   149,    75,    76,
     103,    98,   104,   110,   138,   121,   122,   123,   133,   103,
     128,   129,   130,    46,   145,   146,   147,   153,   156,   157,
     137,   158,   139,   140,   164,   142,   140,   103,   159,   134,
     106,   160,   103,   161,   162,   171,   168,    58,   103,   155,
     132,   150,   112,   113,   117,   115,    43,   154,   119,   116,
       0,   118,   143,     0,     1,     2,     0,     0,     0,     3,
     141,     4,     0,     0,     5,     0,     0,     0,     0,   166,
       0,     0,     0,     0,     0,   169,     0,   170,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,     0,     3,
       0,     4,    16,     0,     5,     0,    17,    18,     0,     0,
       0,    19,     0,     0,     0,    20,     0,    21,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,     0,     3,
     131,     4,    16,     0,     5,     0,    17,    18,     0,     0,
       0,    19,     0,     0,     0,    20,    50,    21,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,     0,     3,
       0,     4,    16,     0,     5,     0,    17,    18,     0,     0,
       0,    19,     0,     0,     0,    20,     0,    21,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,     0,     3,
       0,     4,    16,     0,     5,     0,    17,    18,     0,     0,
       0,    19,     0,     0,     0,    20,     0,    21,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    52,     0,     3,
       0,     0,    16,     0,     0,     0,    17,    18,     0,     0,
       0,    19,     0,     0,     0,    20,     0,    21,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,     0,     0,
       0,     0,    16,     0,     0,     0,    17,    18,     0,     0,
       0,    19,     0,     0,     0,    20,     0,    21
};

static const yytype_int16 yycheck[] =
{
       4,     5,   111,    50,     9,     8,    50,    35,    21,     9,
      16,    17,    18,    37,    36,    19,    20,    21,    37,    27,
      28,    29,    30,    50,    20,     0,    22,    51,    36,    42,
      43,    37,    51,    36,    44,    14,    15,    54,    42,    57,
      58,   150,    50,   152,    40,    41,    54,    52,    48,    93,
      48,    49,    49,   100,   101,    59,   100,   101,    50,    87,
      50,   170,    52,    23,    24,    25,    26,    57,    58,    49,
      52,    53,    52,    53,    50,    12,    50,   124,    38,    39,
     124,    51,    48,    52,    55,    89,    90,    91,    50,   133,
      94,    95,    96,    97,    55,    51,    51,    11,    37,    51,
     104,    55,   106,   107,   151,   109,   110,   151,    48,   156,
       9,    57,   156,    49,    21,   162,   156,    23,   162,   133,
      97,   125,    66,    66,    77,    67,     1,   131,    86,    70,
      -1,    82,   110,    -1,     3,     4,    -1,    -1,    -1,     8,
     107,    10,    -1,    -1,    13,    -1,    -1,    -1,    -1,   153,
      -1,    -1,    -1,    -1,    -1,   159,    -1,   161,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,     8,
      -1,    10,    41,    -1,    13,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    56,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,     8,
       9,    10,    41,    -1,    13,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    56,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,     8,
      -1,    10,    41,    -1,    13,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    56,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,     8,
      -1,    10,    41,    -1,    13,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    56,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,     8,
      -1,    -1,    41,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    56,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    56
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     8,    10,    13,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    41,    45,    46,    50,
      54,    56,    60,    61,    62,    69,    70,    73,    75,    77,
      79,    81,    83,    85,    86,    87,    88,    89,    90,    94,
      96,    36,    63,    90,    36,    69,    69,    71,    72,    69,
      55,    69,    36,    69,    91,    92,    93,     0,    62,    49,
      52,    53,    64,    16,    17,    18,    37,    76,    14,    15,
      78,    23,    24,    25,    26,    38,    39,    80,    20,    22,
      40,    41,    82,    21,    42,    43,    84,    44,    86,    54,
      50,    50,    69,    50,    12,    49,     9,    52,    51,    36,
      50,    54,    66,    94,    48,    95,     9,    48,    57,    58,
      52,    69,    70,    73,    74,    77,    79,    81,    83,    85,
      86,    69,    69,    69,    48,    49,    66,    68,    69,    69,
      69,     9,    72,    50,    66,    67,    66,    69,    55,    69,
      69,    92,    69,    93,    64,    55,    51,    51,    65,    66,
      69,    37,    51,    11,    69,    68,    37,    51,    55,    48,
      57,    49,    21,    64,    66,    64,    69,    51,    67,    69,
      69,    66,    64
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 97 "nesl.y"
    {
            /* yyheader = $1; */
        ;}
    break;

  case 3:
#line 104 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 4:
#line 108 "nesl.y"
    {
            /* $$ = createNode(NODE_TOP_LEVELS); */
            /* $$->left = $1; */
            /* $$->right = $2; */
        ;}
    break;

  case 5:
#line 117 "nesl.y"
    {
            // TODO:
        ;}
    break;

  case 6:
#line 121 "nesl.y"
    {
            // TODO:
        ;}
    break;

  case 7:
#line 125 "nesl.y"
    {
            // TODO:
        ;}
    break;

  case 8:
#line 129 "nesl.y"
    {
            /* $$ = createNode(NODE_ASSIGN); */
            /* $$->left = $1; */
            /* $$->right = $3; */
        ;}
    break;

  case 9:
#line 135 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 10:
#line 142 "nesl.y"
    {
            /* $$ = createNode(NODE_ID); */
            /* $$->str_val = $1; */
        ;}
    break;

  case 11:
#line 147 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 14:
#line 159 "nesl.y"
    {    
            /* $$ = createNode(NODE_FUNC_TYPE_DEF); */
            /* $$->left = $1; */
            /* $$->right = $3; */
        ;}
    break;

  case 15:
#line 168 "nesl.y"
    {  
            /* $$ = $1; */
        ;}
    break;

  case 16:
#line 172 "nesl.y"
    {
            // TODO:
        ;}
    break;

  case 17:
#line 176 "nesl.y"
    {
            // TODO:
        ;}
    break;

  case 18:
#line 180 "nesl.y"
    {
            // TODO:
        ;}
    break;

  case 19:
#line 187 "nesl.y"
    {
            // TODO:
        ;}
    break;

  case 20:
#line 191 "nesl.y"
    {
            (yyval.node) = (yyvsp[(1) - (1)].node);   
        ;}
    break;

  case 21:
#line 198 "nesl.y"
    {
            // TODO:
        ;}
    break;

  case 22:
#line 202 "nesl.y"
    {
            /* $$ = $1;     */
        ;}
    break;

  case 23:
#line 209 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 24:
#line 213 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 25:
#line 220 "nesl.y"
    {
            /* $$ = createNode(NODE_IFELSE); */
            /* $$->sibling = $2; */
            /* $$->left = $4; */
            /* $$->right = $6; */
        ;}
    break;

  case 26:
#line 227 "nesl.y"
    {
            /* $$ = createNode(NODE_LET); */
            /* $$->left = $2; */
            /* $$->right = $5; */
        ;}
    break;

  case 27:
#line 233 "nesl.y"
    {
            /* $$ = createNode(NODE_LET); */
            /* $$->left = $2; */
            /* $$->right = $4; */
        ;}
    break;

  case 28:
#line 242 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 29:
#line 246 "nesl.y"
    { 
            /* $$ = createNode(NODE_EXP_BINDS); */
            /* $$->left = $1; */
            /* $$->right = $3; */
        ;}
    break;

  case 30:
#line 255 "nesl.y"
    {
            /* $$ = createNode(NODE_ASSIGN); */
            /* $$->left = $1; */
            /* $$->right = $3; */
        ;}
    break;

  case 31:
#line 264 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 32:
#line 268 "nesl.y"
    {
            /* $$ = createNode(NODE_TUPLE); */
            /* $$->left = $1; */
            /* $$->right = $3; */
        ;}
    break;

  case 33:
#line 278 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 34:
#line 282 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 35:
#line 289 "nesl.y"
    {
            /* $2->left = $1; */
            /* $2->right = $3; */
            /* $$ = $2; */
        ;}
    break;

  case 36:
#line 295 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 37:
#line 302 "nesl.y"
    { 
            /* $$ = createNode(NODE_OR);  */
        ;}
    break;

  case 38:
#line 306 "nesl.y"
    { 
            /* $$ = createNode(NODE_NOR);  */
        ;}
    break;

  case 39:
#line 310 "nesl.y"
    { 
            /* $$ = createNode(NODE_XOR);   */
        ;}
    break;

  case 40:
#line 317 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 41:
#line 321 "nesl.y"
    {
            /* $2->left = $1; */
            /* $2->right = $3; */
            /* $$ = $2; */
        ;}
    break;

  case 42:
#line 330 "nesl.y"
    {
            /* $$ = createNode(NODE_AND); */
        ;}
    break;

  case 43:
#line 334 "nesl.y"
    {
            /* $$ = createNode(NODE_NAND); */
        ;}
    break;

  case 44:
#line 341 "nesl.y"
    {
      (yyval.node) = (yyvsp[(1) - (1)].node);
    ;}
    break;

  case 45:
#line 345 "nesl.y"
    {
      (yyval.node) = RelationalOpFactory::CreateRelationalOpNode((yyvsp[(2) - (3)].opcode), (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
    ;}
    break;

  case 46:
#line 352 "nesl.y"
    {
      (yyval.opcode) = EQ_OP;
    ;}
    break;

  case 47:
#line 356 "nesl.y"
    {
      (yyval.opcode) = NE_OP;
    ;}
    break;

  case 48:
#line 360 "nesl.y"
    {
      (yyval.opcode) = LT_OP;
    ;}
    break;

  case 49:
#line 364 "nesl.y"
    {
      (yyval.opcode) = GT_OP;
    ;}
    break;

  case 50:
#line 368 "nesl.y"
    {
      (yyval.opcode) = LE_OP;
    ;}
    break;

  case 51:
#line 372 "nesl.y"
    {
      (yyval.opcode) = GE_OP;
    ;}
    break;

  case 52:
#line 379 "nesl.y"
    {
      (yyval.node) = (yyvsp[(1) - (1)].node);
    ;}
    break;

  case 53:
#line 383 "nesl.y"
    {
      (yyval.node) = ArithmeticOpFactory::CreateArithmeticOpNode((yyvsp[(2) - (3)].opcode), (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
    ;}
    break;

  case 54:
#line 390 "nesl.y"
    {
      (yyval.opcode) = ADD_OP;
    ;}
    break;

  case 55:
#line 394 "nesl.y"
    {
      (yyval.opcode) = SUBTRACT_OP;      
    ;}
    break;

  case 56:
#line 398 "nesl.y"
    {
      (yyval.opcode) = PP_OP;
    ;}
    break;

  case 57:
#line 402 "nesl.y"
    {
      (yyval.opcode) = LARROW_OP;
    ;}
    break;

  case 58:
#line 409 "nesl.y"
    {
      (yyval.node) = (yyvsp[(1) - (1)].node);
    ;}
    break;

  case 59:
#line 413 "nesl.y"
    {
      (yyval.node) = ArithmeticOpFactory::CreateArithmeticOpNode((yyvsp[(2) - (3)].opcode), (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
    ;}
    break;

  case 60:
#line 420 "nesl.y"
    {
      (yyval.opcode) = MUL_OP;
    ;}
    break;

  case 61:
#line 424 "nesl.y"
    {
      (yyval.opcode) = DIV_OP;
    ;}
    break;

  case 62:
#line 428 "nesl.y"
    {
      (yyval.opcode) = RARROW_OP
    ;}
    break;

  case 63:
#line 435 "nesl.y"
    { 
            /* $$ = $1; */
        ;}
    break;

  case 64:
#line 439 "nesl.y"
    {
            /* $$ = createNode(NODE_UPT); */
            /* $$->left = $1; */
            /* $$->right = $3; */
        ;}
    break;

  case 65:
#line 448 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 66:
#line 452 "nesl.y"
    {
            /* $1->left = $2; */
            /* $$ = $1; */
        ;}
    break;

  case 67:
#line 460 "nesl.y"
    {   
            /* $$ = createNode(NODE_SHARP); */
        ;}
    break;

  case 68:
#line 464 "nesl.y"
    {
            /* $$ = createNode(NODE_AT); */
        ;}
    break;

  case 69:
#line 468 "nesl.y"
    {
            /* $$ = createNode(NODE_UMINUS); */
        ;}
    break;

  case 70:
#line 475 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 71:
#line 479 "nesl.y"
    {
            /* $$ = createNode(NODE_SEQ_REF); */
            /* $$->left = $1; */
            /* $$->right = $3; */
        ;}
    break;

  case 72:
#line 488 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 73:
#line 492 "nesl.y"
    {
            // TODO:
        ;}
    break;

  case 74:
#line 496 "nesl.y"
    {
            /* $$ = $2; */
        ;}
    break;

  case 75:
#line 500 "nesl.y"
    {
            /* $$ = createNode(NODE_APPLY_TO_EACH); */
            /* $$->left = $2; */
            /* $$->right = $4; */
        ;}
    break;

  case 76:
#line 506 "nesl.y"
    {
            /* $$ = createNode(NODE_EMPTY_SEQ); */
            /* $$->left = $3; */
        ;}
    break;

  case 77:
#line 511 "nesl.y"
    {
            /* $$ = createNode(NODE_SEQ); */
            /* $$->left = $2; */
            /* $$->right = $3; */
        ;}
    break;

  case 78:
#line 517 "nesl.y"
    {
            /* $$ = $2; */
        ;}
    break;

  case 79:
#line 521 "nesl.y"
    {
            /* $$ = createNode(NODE_ID); */
            /* $$->str_val = $1; */
        ;}
    break;

  case 80:
#line 526 "nesl.y"
    {
            /* $$ = createNode(NODE_FUNC_CALL); */
            /* $$->left = $1; */
            /* $$->right = $3; */
        ;}
    break;

  case 81:
#line 535 "nesl.y"
    {
            // TODO:
        ;}
    break;

  case 82:
#line 542 "nesl.y"
    {
            /* $$ = createNode(NODE_APPLY_BODY); */
            /* $$->left = $1; */
            /* $$->right = $3; */
        ;}
    break;

  case 83:
#line 548 "nesl.y"
    {
            // TODO:
        ;}
    break;

  case 84:
#line 555 "nesl.y"
    {
            /* $$ = createNode(NODE_RBINDS); */
            /* $$->left = $1; */
            /* $$->right = $3; */
        ;}
    break;

  case 85:
#line 561 "nesl.y"
    {
            /* $$ = $1; */
        ;}
    break;

  case 86:
#line 568 "nesl.y"
    {
            /* $$ = createNode(NODE_ID); */
            /* $$->str_val = $1; */
        ;}
    break;

  case 87:
#line 573 "nesl.y"
    {
            /* $$ = createNode(NODE_IN); */
            /* $$->left = $1; */
            /* $$->right = $3; */
        ;}
    break;

  case 88:
#line 582 "nesl.y"
    {
            /* $$ = createNode(NODE_ID); */
            /* $$->str_val = $1; */
        ;}
    break;

  case 89:
#line 587 "nesl.y"
    {
            /* $$ = createNode(NODE_TYPE_INT); */
        ;}
    break;

  case 90:
#line 591 "nesl.y"
    {
            /* $$ = createNode(NODE_TYPE_FLOAT); */
        ;}
    break;

  case 91:
#line 595 "nesl.y"
    {
            /* $$ = createNode(NODE_TYPE_BOOL); */
        ;}
    break;

  case 92:
#line 599 "nesl.y"
    {
            /* $$ = createNode(NODE_TYPE_CHAR); */
        ;}
    break;

  case 93:
#line 606 "nesl.y"
    {
            /* $$ = createNode(NODE_SEQ_TAIL); */
            /* $$->left = $2; */
        ;}
    break;

  case 94:
#line 611 "nesl.y"
    {
            /* $$ = createNode(NODE_SEQ_TAIL); */
            /* $$->left = $2; */
            /* $$->right = $4; */
        ;}
    break;

  case 95:
#line 616 "nesl.y"
    {
            /* $$ = createNode(NODE_SEQ_TAIL); */
        ;}
    break;

  case 96:
#line 623 "nesl.y"
    {
      (yyval.node) = new ConstantInteger((yyvsp[(1) - (1)].tokenstr));
    ;}
    break;

  case 97:
#line 627 "nesl.y"
    {
      (yyval.node) = new ConstantFloat((yyvsp[(1) - (1)].tokenstr));
    ;}
    break;

  case 98:
#line 631 "nesl.y"
    {
      (yyval.node) = new ConstantBoolean(true);
    ;}
    break;

  case 99:
#line 635 "nesl.y"
    {
      (yyval.node) = new ConstantBoolean(false);
    ;}
    break;

  case 100:
#line 639 "nesl.y"
    {
      (yyval.node) = new ConstantString((yyvsp[(1) - (1)].tokenstr));
    ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2334 "nesl.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 644 "nesl.y"


void yyerror(char const *s) {
  
  fprintf(stderr, "line %d: %s\n", yylineno, s);
}

