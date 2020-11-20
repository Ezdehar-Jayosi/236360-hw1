%{
#include "tokens.hpp"
#include <stdio.h>
void print_error();
void error();
%}

%option noyywrap
%option yylineno
whitespace [\n\t\r 	]
whitespaceA [ \t\r\n\f]
letter [a-zA-Z]
digit [0-9]
binop [(\+|\-|\*|\/)]
%x STRING_S
%%

void return VOID;
int return INT;
byte return BYTE;
b return B;
bool return BOOL;
and return AND;
or return OR;
not return NOT;
true return TRUE;
false return FALSE;
return return RETURN;
if return IF;
else return ELSE;
while return WHILE;
break return BREAK;
continue return CONTINUE;
(\;) return SC;
(\,) return COMMA;
(\() return LPAREN;
(\)) return RPAREN;
(\{) return LBRACE;
(\}) return RBRACE;
= return ASSIGN;
(((!|<|>|=)=)|<|>) return RELOP;
{binop} return BINOP;
\/\/.* return COMMENT; 
{letter}({letter}|{digit})* return ID;
([1-9]{digit}*|0) return NUM;
(\") BEGIN(STRING_S);
<STRING_S><<EOF>> print_error();
<STRING_S>([\x00-\x09\x0b-\x0c\x0e-\x21\x23-\x5b\x5d-\x7f]|((\\)(\\))|((\\)(\"))|((\\)(n))|((\\)(r))|((\\)(t))|((\\)(0))|((\\)x))*(\") {BEGIN(INITIAL);return STRING;}
<STRING_S>([^(\")])*((\")?) return WRONGSTR;
<STRING_S>. error();
({whitespaceA})+  return WHITESPACE;  
. error();
%%

void print_error() {
    printf("Error unclosed string\n");
    exit(1);
}
void error() {
    printf("Error %s\n", yytext);
    exit(2);
}
