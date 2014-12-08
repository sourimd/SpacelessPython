%{
#include <iostream>
#include <cstring>
#include "ast.h"

#include "parse.tab.h"

  int lineno=1;

%}
letter      [a-zA-Z]
ident       {letter}({letter}|{digit})*
digit       [0-9]
digits      [0-9]+
float       {digits}"."{digits}



%%

"**"       { return EXPON; }
","        { return COMMA; }
"="        { return ASSIGN; }
"+"        { return PLUS; }
"-"        { return MINUS; }
"*"        { return MULT; }
"/"        { return DIV; }
"%"        { return MOD; }
"("        { return LPAREN; }
")"        { return RPAREN; }
":"        { return COLON; }

"=="        {return EQUALS;}
">="        {return GREATEQ;}
"<="        {return LESSEQ;}
">"         {return GREAT;}
"<"         {return LESS;}

"call"      { return CALL;}
"expr"      { return EXPR;}
"if"        { return IF;}
"else"      { return ELSE;}
"def"       { return DEF; }
"print"    { return PRINT; }
"end"      { return END; }
"endif"     { return ENDIF;}
"return"   { return RETURN;}
"global"    {return GLOBAL;}

{ident}    { 
				yylval.str = new char[strlen(yytext)+1];
  				strcpy(yylval.str, yytext);
				return IDENT; 
			}


{float}    { yylval.d = atof(yytext);
             return FLOAT; 
           }
{digits}   { yylval.i = atoi(yytext);
             return NUMBER; 
           }
"#"[^\n]*  { ; }
"\n"       { 
			      ++lineno;
            return CR; 
           }
.          { ; }

%%
int yywrap() {
  yylex_destroy();
  return 1;
}
