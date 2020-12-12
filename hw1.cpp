#include "tokens.hpp"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string>
int str_len(const char* str){
    int len=0;
    while(str[len]!=0){
        len++;
    }
    return len;
}
char hex2int(int i,int strLen){
    int tenth_digit, ones_digit;
    int sum =0;
    if(yytext[i]>='0' && yytext[i]<='7') tenth_digit=yytext[i]-'0';
    else  sum -1;
    if((sum!=-1)&&(tenth_digit<=7 && tenth_digit>=0)){
        tenth_digit=tenth_digit*16;
        if(yytext[i+1] >= 'A'&& yytext[i+1] <= 'F'){
          ones_digit = yytext[i+1] - 'A' + 10;
        }else if(yytext[i+1] >= '0' && yytext[i+1] <='9'){
            ones_digit= yytext[i+1] - '0';
        }else if(yytext[i+1] >= 'a' && yytext[i+1] <='f'){
            ones_digit= yytext[i+1] - 'a' + 10;
        }else{
            sum= -1;
        }
    }else{
        sum= -1;
    }
    if(sum==0) sum = tenth_digit+ones_digit;
    if(i+1 >= strLen || sum==-1){
        std::cout << "Error undefined escape sequence " << "x";
        int j=2;
        while(yytext[i]!='\"' && j>0){
            std::cout << yytext[i];
            i++;
            j--;
        }
        std::cout << std::endl;
         return -1;
     }
    return tenth_digit+ones_digit;
}
char SpecialCasesToString(char escape_seq){
    if(escape_seq=='n') return '\n';
    else if(escape_seq=='r') return '\r';
    else if(escape_seq=='t') return '\t';
    else if(escape_seq=='0') return '\0';
    else if(escape_seq=='\"') return '"';
    else if(escape_seq=='\\') return '\\';
    else if(escape_seq=='x') return 'x';
    else return -1;

}   

void print_error_msg(int index){
    std::cout << "Error undefined escape sequence" << " " << yytext[index] <<  " \n" << std::endl;
    exit(0);
}    
void dealWithString(const int token){
    std::string output_str="";
       int strLen = str_len(yytext);
        int i=0;
        for(i;i<strLen-1;i++){
            if(yytext[i]=='\\'){
                char str=SpecialCasesToString(yytext[i+1]);
                if(str=='\0'){
                   std::cout << yylineno << " " << "STRING " << output_str << std::endl;
                   return;
                }if(str=='x'){  
                    int fromAsciiToChar = hex2int(i+2,strLen); 
                    if( fromAsciiToChar==-1){
                        exit(0);
                    }
                    i+=3;
                    output_str.push_back(fromAsciiToChar); 
                    continue;
                 
                }else if(str==-1) print_error_msg(i+1);
                
                i+=1;
                output_str.push_back(str);
                
                //i++;//TODO: check this!! this might be wrong
            }else if (yytext[i] !='\\'){
                output_str.push_back(yytext[i]); //DONE
               
            
            }
         
        
        }
         std::cout << yylineno << " " << "STRING " << output_str << std::endl;
}
void dealWithWrongStr(const int token){
        int strLen = str_len(yytext);
        if((strLen>0 &&  yytext[strLen-1]!='\"') || strLen == 0){
          printf("Error unclosed string\n");
          exit(0);
        }
        for (size_t i = 0; i < strLen; i++) {
            if (yytext[i] == '\n' || yytext[i] == '\r' || yytext[i] == '\\' && (i == strLen - 1)) {
                printf("Error unclosed string\n");
                exit(0);
            }
            else if (yytext[i] == '\"' && i<strLen-1) {
                printf("Error %c\n", yytext[i]);
                exit(0);
            }
            else if ((yytext[i] == '\\')&& (yytext[i + 1] == 'x') ) { 
                    if( hex2int(i+2,strLen)==-1) exit(0);
            }
            else if(yytext[i]=='\\'&& (yytext[i+1]!='n' && yytext[i+1]!='r' && yytext[i+1]!='t' && yytext[i+1]!='0' && yytext[i+1]!='\"' && yytext[i+1]!='\\')){
                    char c = yytext[i+1];
                    printf("Error undefined escape sequence %c\n",c);
                    exit(0);
            }
        }
        printf("Error unclosed string\n");
        exit(0);
}


int main()
{
	int token;
	while(token = yylex()) {
        if(token==COMMENT){ 
            std::cout << yylineno <<  " COMMENT //" << std::endl; continue;
        }else if(token==WHITESPACE) {continue;}
        else if(token==STRING ){dealWithString(token);}
        else if(token==WRONGSTR) {dealWithWrongStr(token);}
        else{
            //TODO: deal with this!! we did not recieve a string or a comment or anything dealt with above. //DONE FOR NOW
            std::cout << yylineno << " " << tokenTypeArray[token-1] << " " << yytext << std::endl;
        }
     
	}
	return 0;
}