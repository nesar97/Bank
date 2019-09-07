
%{
#include <stdio.h>
#include<string.h>
#include"struct.h"
#include<string>
#include<iostream>
using namespace std;
extern int yyerror(const char *s);
extern int yylex();
details val;


%}


%token NUMBER NAME MONEY
%token OPEN CLOSE ACCOUNT SHOW BALANCE DEPOSIT WITHDRAW MINI STATEMENT ALL
%token BANK ACCOUNTS ACTIVE CONNECTIONS SHUTDOWN ERROR

%type <name> NAME
%type <num> NUMBER
%type <money> MONEY

%union{
char name[30];
int num;
float money;
}


%%

stmt: 
 | OPEN ACCOUNT NAME { val.option=1; val.username=$3; val.amount=-1; val.accno=-1; return 1;} 
 | CLOSE ACCOUNT NUMBER { val.option=2; val.username=""; val.amount=-1; val.accno=$3;    return 1;}
 | SHOW ACCOUNT BALANCE NUMBER { val.option=3; val.username=""; val.amount=-1; val.accno=$4;   return 1;}
 | DEPOSIT NUMBER MONEY { val.option=4; val.username=""; val.amount=$3; val.accno=$2;   return 1;}
 | WITHDRAW NUMBER MONEY { val.option=5; val.username=""; val.amount=$3; val.accno=$2;   return 1;}
 | SHUTDOWN { val.option=6; val.username=""; val.amount=-1; val.accno=-1;   return 1;}
 | SHOW MINI STATEMENT NUMBER { val.option=7; val.username=""; val.amount=-1; val.accno=$4;   return 1;}
 | SHOW ALL ACCOUNTS { val.option=8; val.username=""; val.amount=-1; val.accno=-1;   return 1;}
 | SHOW BANK BALANCE { val.option=9; val.username=""; val.amount=-1; val.accno=-1;   return 1;}
 | SHOW ACTIVE CONNECTIONS { val.option=10; val.username=""; val.amount=-1; val.accno=-1;   return 1;}
 | ERROR {val.option=-1; val.username=""; val.amount=-1; val.accno=-1; return 1; } 
 ;
%%


int yyerror(const char *s)
{
 val.option=-1; val.username=""; val.amount=-1; val.accno=-1;
 return 1;
}

