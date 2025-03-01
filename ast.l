%{
/* ast.tab.h was generated by bison with the -d option */
#include "ast.tab.h"
extern int atoi (const char *);

/* columns  are not tracked here (yylloc.first_column and yylloc.last_column are not set) */
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
%}

%x COMMENT

%option noyywrap
/*  this will cause  flex to maintain the current input line number in
    the global variable int yylineno.
*/
%option yylineno
%option noinput nounput

%%

[0-9]+     { yylval.ival = atoi (yytext); return INT_NUM; }

[0-9]+\.[0-9]+ { yylval.fval = atof (yytext); return FLOAT_NUM; }

[\n\t ]+   /* skip white space */

"+"        { yylval.op = PLUS; return ADDOP;}
"-"        { yylval.op = MINUS; return ADDOP;}
"*"        { yylval.op = MUL; return MULOP; }
"/"        { yylval.op = DIV; return MULOP;}
"^"        { yylval.op = XOR_OP; return XOR;}

"<"        { yylval.op = LT; return RELOP; }
">"        { yylval.op = GT; return RELOP; }
"<="        { yylval.op = LE; return RELOP; }
">="        { yylval.op = GE; return RELOP; }
"=="        { yylval.op = EQ; return RELOP; }
"!="        { yylval.op = NE; return RELOP; }


[()=;:{}]      { return yytext[0]; }


"if"       { return IF; }
"else"     { return ELSE; }
"while"    { return WHILE; }
"int"      { return INT; }
"float"    { return FLOAT; }
"or"       { return OR; }
"and"      { return AND; }
"not"      { return NOT; }
"nand"     { return NAND; }
"switch"   { return SWITCH; }
"case"     { return CASE; }
"default"  { return DEFAULT; }
"break"    { return BREAK; }
"continue" { return CONTINUE;}
"read"     { return READ;}
"iota"     { yylval.ival = atoi (yytext); return IOTA; }


"repeat"   {return REPEAT;}


[a-zA-Z][A-Za-z_]*  {  strcpy (yylval.name, yytext); return ID; }

  /* C++ style comments: */
"//".*     /* skip comment */

  /* C style comments: */
"/*"       { BEGIN (COMMENT); }
<COMMENT>. /* skip comment */
<COMMENT>"*/" { BEGIN (0); }

.          { fprintf (stderr, "unrecognized token %c\n", yytext[0]); }

%%
