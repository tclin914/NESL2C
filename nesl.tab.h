/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 85 "nesl.y"
{
  int tokenval;
  char *tokenstr;
  nesl2c::OpCode opcode;
  Node* node;
}
/* Line 1529 of yacc.c.  */
#line 130 "nesl.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

