/* A Bison parser, made by GNU Bison 3.5.0.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_AST_TAB_H_INCLUDED
# define YY_YY_AST_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 21 "ast.y"

#include "gen.h"
#include "ast.h"

#line 53 "ast.tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT_NUM = 258,
    IOTA = 259,
    FLOAT_NUM = 260,
    ADDOP = 261,
    MULOP = 262,
    RELOP = 263,
    XOR = 264,
    ID = 265,
    READ = 266,
    IF = 267,
    ELSE = 268,
    WHILE = 269,
    FOR = 270,
    INT = 271,
    FLOAT = 272,
    REPEAT = 273,
    OR = 274,
    AND = 275,
    NOT = 276,
    NAND = 277,
    SWITCH = 278,
    CASE = 279,
    DEFAULT = 280,
    BREAK = 281,
    CONTINUE = 282
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 27 "ast.y"

   int ival;
   // float numbers in the source program are stored as double
   double fval;
   enum op op;
   char name[100];

   myType _type;

   //  pointers to AST nodes:
   Stmt *stmt;
   Block *block;
   Stmt *stmtlist; // points to first Stmt in the list (NULL if list is empty)
   ReadStmt *read_stmt;
   AssignStmt *assign_stmt;
   IfStmt *if_stmt;
   WhileStmt *while_stmt;
   SwitchStmt *switch_stmt;
   RepeatStmt *repeat_stmt;
   Case *caselist; //points to first Case in the list
   Case *mycase;
   BreakStmt *break_stmt;
   ContinueStmt *continue_stmt;
   Exp  *exp;
   BoolExp *boolexp;
   bool hasBreak;

#line 120 "ast.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_AST_TAB_H_INCLUDED  */
