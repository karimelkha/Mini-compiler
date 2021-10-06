/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_VOID = 258,
    TOK_INT = 259,
    TOK_INTVAL = 260,
    TOK_BOOL = 261,
    TOK_TRUE = 262,
    TOK_FALSE = 263,
    TOK_IF = 264,
    TOK_ELSE = 265,
    TOK_WHILE = 266,
    TOK_FOR = 267,
    TOK_DO = 268,
    TOK_PRINT = 269,
    TOK_SEMICOL = 270,
    TOK_IDENT = 271,
    TOK_STRING = 272,
    TOK_AFFECT = 273,
    TOK_LPAR = 274,
    TOK_RPAR = 275,
    TOK_LACC = 276,
    TOK_RACC = 277,
    TOK_COMMA = 278,
    TOK_LT = 279,
    TOK_RT = 280,
    TOK_PLUS = 281,
    TOK_MINUS = 282,
    TOK_MULT = 283,
    TOK_DIV = 284,
    TOK_REST = 285,
    TOK_OPP = 286,
    TOK_LOGIC_AND = 287,
    TOK_LOGIC_OR = 288,
    TOK_LOGIC_XOR = 289,
    TOK_LDEC = 290,
    TOK_RDEC = 291,
    TOK_LTEQ = 292,
    TOK_RTEQ = 293,
    TOK_EQ = 294,
    TOK_DIF = 295,
    TOK_AND = 296,
    TOK_OR = 297,
    TOK_THEN = 298
  };
#endif
/* Tokens.  */
#define TOK_VOID 258
#define TOK_INT 259
#define TOK_INTVAL 260
#define TOK_BOOL 261
#define TOK_TRUE 262
#define TOK_FALSE 263
#define TOK_IF 264
#define TOK_ELSE 265
#define TOK_WHILE 266
#define TOK_FOR 267
#define TOK_DO 268
#define TOK_PRINT 269
#define TOK_SEMICOL 270
#define TOK_IDENT 271
#define TOK_STRING 272
#define TOK_AFFECT 273
#define TOK_LPAR 274
#define TOK_RPAR 275
#define TOK_LACC 276
#define TOK_RACC 277
#define TOK_COMMA 278
#define TOK_LT 279
#define TOK_RT 280
#define TOK_PLUS 281
#define TOK_MINUS 282
#define TOK_MULT 283
#define TOK_DIV 284
#define TOK_REST 285
#define TOK_OPP 286
#define TOK_LOGIC_AND 287
#define TOK_LOGIC_OR 288
#define TOK_LOGIC_XOR 289
#define TOK_LDEC 290
#define TOK_RDEC 291
#define TOK_LTEQ 292
#define TOK_RTEQ 293
#define TOK_EQ 294
#define TOK_DIF 295
#define TOK_AND 296
#define TOK_OR 297
#define TOK_THEN 298

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 37 "grammar.y" /* yacc.c:1909  */

    int32_t intval;
    char * strval;
    node_t ptr;

#line 146 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (node_t * program_root);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
