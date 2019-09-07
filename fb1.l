
%option nounput
%option noyywrap
%{
#include "fb1.tab.h"
#include<stdio.h>
#include<string.h>
%}


%%
[Oo][pP][eE][nN] {return OPEN;}
[Cc][Ll][Oo][Ss][eE] {return CLOSE;}
[aA][cC][cC][oO][uU][nN][tT] {return ACCOUNT;}
[Ss][Hh][Oo][Ww] {return SHOW;}
[Bb][Aa][Ll][Aa][Nn][Cc][Ee] {return BALANCE;}
[Dd][Ee][Pp][Oo][Ss][Ii][Tt] {return DEPOSIT;}
[Ww][Ii][Tt][Hh][Dd][Rr][Aa][Ww] {return WITHDRAW;}
[Mm][Ii][Nn][Ii] {return MINI;}
[Ss][Tt][Aa][Tt][Ee][Mm][Ee][Nn][Tt] {return STATEMENT;}
[Aa][Cc][Cc][Oo][Uu][Nn][Tt][Ss] {return ACCOUNTS;}
[Bb][Aa][Nn][Kk] {return BANK;}
[Aa][Cc][Tt][Ii][Vv][Ee] {return ACTIVE;}
[Cc][Oo][Nn][Nn][Ee][Cc][Tt][Ii][Oo][Nn][Ss] {return CONNECTIONS;}
[Ss][Hh][Uu][Tt][Dd][Oo][Ww][Nn] {return SHUTDOWN;}
[Aa][Ll][Ll] {return ALL;}
[A-Za-z]+ { strcpy(yylval.name,yytext); return NAME;  }
([0-9])+ { yylval.num = atoi(yytext); return NUMBER; }
[-+]?([0-9]*[.])?[0-9]+ {yylval.money=atof(yytext); return MONEY;}
\n {  }
[ \t] { /* ignore whitespace */ }
. {  return ERROR; }
%%

 void my_buf_func(char *buf)
{
yy_scan_string(buf);
}

void destroy()
{
yylex_destroy();
}
