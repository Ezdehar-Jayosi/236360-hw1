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
char fromAscii(int i){
    int tenth_digit, ones_digit;
    if(yytext[i]>='0' && yytext[i]<='7') tenth_digit=yytext[i]-'0';
    else  return -1;
    if(tenth_digit<=7 && tenth_digit>=0){
        if(yytext[i+1] >= 'A'&& yytext[i+1] <= 'F'){
          ones_digit = yytext[i+1] - 'A' + 10;
        }else if(yytext[i+1] >= '0' && yytext[i+1] <='9'){
            ones_digit= yytext[i+1] - '0';
        }else if(yytext[i+1] >= 'a' && yytext[i+1] <='f'){
            ones_digit= yytext[i+1] - 'a' + 10;
        }else{
            return -1;
        }
    }else{
        return -1;
    }
    return tenth_digit*16+ones_digit;
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

void dealWithToken(const int token){
   std::string output_str="";
    if(token==WRONGSTR){ 
       int strLen = str_len(yytext);
        if(strLen == 0 || (strLen>0 &&  yytext[strLen-1]!='\"')){
          printf("Error unclosed string\n");
          exit(0);
        }
        for (size_t i = 0; i < strLen; i++) {
            if (yytext[i] == '\n' || yytext[i] == '\r' || yytext[i] == '\\' && (i == strLen - 1)) {
                printf("Error unclosed string\n");
                exit(0);
            }
            
            if (yytext[i] == '\\') {
                if (yytext[i + 1] == 'x') {
                    int fromAsciiToChar = fromAscii(i+2); 
                    if(i+3 >= strLen || fromAsciiToChar==-1){
                        if(yytext[i+2]=='\"')  std::cout << "Error undefined escape sequence " << "x" << std::endl;
                        if(yytext[i+3]=='\"')  std::cout << "Error undefined escape sequence " << "x" << yytext[i+2]  << std::endl; 
                        std::cout << "Error undefined escape sequence " << "x" << yytext[i+2] << yytext[i+3]  << std::endl; 
                        exit(0);
                         //printf("(2*)\n");
                        
                    }
                }else if(yytext[i]=='\\'&& (yytext[i+1]!='n' && yytext[i+1]!='r' && yytext[i+1]!='t' && yytext[i+1]!='0' && yytext[i+1]!='\"' && yytext[i+1]!='\\')){
                    char c = yytext[i+1];
                    printf("Error undefined escape sequence %c\n",c);
                    exit(0);
                }
               
            }
             
            if (yytext[i] == '\"' && i<strLen-1) {
                printf("Error %c\n", yytext[i]);
                exit(0);
            }
        }
        printf("Error unclosed string\n");
        exit(0);
        
    }
    else if(token==COMMENT){  //print the comment //DONE
        std::cout << yylineno <<  " COMMENT //" << std::endl;
    }else if(token==STRING){
        int strLen = str_len(yytext);
        int i=0;
        for(i;i<strLen-1;i++){
        //if(yytext[i]=='\"') { std::cout << yylineno << " " << "STRING " << output_str << std::endl; }
            if(yytext[i]=='\\'){
                char str=SpecialCasesToString(yytext[i+1]);
                if(str==-1) print_error_msg(i+1);
                else if(str=='x'){  //TODO: this might be worng the comparision
                    int fromAsciiToChar = fromAscii(i+2); //TODO: implement this function which tries to convert an ascii number to its equivelant string
                    //printf("(1*)\n");
                    //printf("fromAsciiToChar is %d, strlen is %d, i is %d\n, yytext[i] is %c",fromAsciiToChar,strLen,i,yytext[i]);
                    if(i+3 >= strLen || fromAsciiToChar==-1){
                        if(yytext[i+2]=='\"')  std::cout << "Error undefined escape sequence " << "x" << std::endl;
                        if(yytext[i+3]=='\"')  std::cout << "Error undefined escape sequence " << "x" << yytext[i+2]  << std::endl;
                        if(yytext[i+4]=='\"')  std::cout << "Error undefined escape sequence " << "x" << yytext[i+2] << yytext[i+3]  << std::endl;
                         //printf("(2*)\n");
                        exit(0);
                    }
                    // printf("(3*)\n");
                    i+=3;
                   // char str_e= fromAsciiToChar;
                  //  printf("str_e is %c\n",str_e);
                  
                    output_str.push_back(fromAsciiToChar);
                    //std::cout<<"fffff " << output_str << "    "<< std::endl;
                   // if(yytext[i+4]=='\"') std::cout << yylineno << " " << "STRING " << output_str << std::endl; 
                    continue;
                 
                }
                else if(str=='\0'){
                   // printf("here\n");
                   // output_str.push_back(str);
                   
                   std::cout << yylineno << " " << "STRING " << output_str << std::endl;
                   return;
                }
                //printf("(4*)\n");
                i+=1;
                output_str.push_back(str);
                
                //i++;//TODO: check this!! this might be wrong
            }else if (yytext[i] !='\\'){
                output_str.push_back(yytext[i]); //DONE
               
            
            }
         
        
        }
         std::cout << yylineno << " " << "STRING " << output_str << std::endl;
       //TODO: print the final output string;
      // std::cout << yylineno << " " << "STRING " << output_str << std::endl;
}else if(token==WHITESPACE) {return;}
    else{
        //TODO: deal with this!! we did not recieve a string or a comment or anything dealt with above. //DONE FOR NOW
        std::cout << yylineno << " " << tokenTypeArray[token-1] << " " << yytext << std::endl;
    
    
    
    }
    
}


int main()
{
	int token;
	while(token = yylex()) {
	// Your code here
  //  if(yytext == "\t" || yytext == "\n" || yytext=="\r") continue;
   // printf("yyyext is %s <<<<<<< \n", yytext);
     dealWithToken(token);
   //  printf("done with the first token \n");
	}
	return 0;
}
