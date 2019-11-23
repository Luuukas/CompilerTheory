#include <string>
#include <iostream>
#include <assert.h>
std::string str;
bool isright = true;
int p=0;
char SYM = '\0';
void advance(){
   assert(p<str.length());
   SYM = str[p++];
}
void error(){
   isright = false;
   std::cout << "ERROR" << std::endl;
}
void Procedure_E();
void Procedure_F();
void Procedure_S();
void Procedure_T();
void Procedure_U();
void Procedure_E(){
   if(!isright) return;
   switch(SYM){
       case '+':
           if(SYM=='+') advance();
           else error();
           Procedure_T();
           Procedure_E();
           break;
       default:
           if(SYM!='#'&&SYM!='#'&&SYM!=')') error();
           break;
   }
}
void Procedure_F(){
   if(!isright) return;
   switch(SYM){
       case '(':
           if(SYM=='(') advance();
           else error();
           Procedure_S();
           if(SYM==')') advance();
           else error();
           break;
       case 'i':
           if(SYM=='i') advance();
           else error();
           break;
       default:
           error();
           break;
   }
}
void Procedure_S(){
   if(!isright) return;
   switch(SYM){
       case '(':
           Procedure_T();
           Procedure_E();
           break;
       case 'i':
           Procedure_T();
           Procedure_E();
           break;
       default:
           error();
           break;
   }
}
void Procedure_T(){
   if(!isright) return;
   switch(SYM){
       case '(':
           Procedure_F();
           Procedure_U();
           break;
       case 'i':
           Procedure_F();
           Procedure_U();
           break;
       default:
           error();
           break;
   }
}
void Procedure_U(){
   if(!isright) return;
   switch(SYM){
       case '*':
           if(SYM=='*') advance();
           else error();
           Procedure_F();
           Procedure_U();
           break;
       default:
           if(SYM!='#'&&SYM!='#'&&SYM!=')'&&SYM!='+') error();
           break;
   }
}
int main(int argc, const char * argv[]) {
   while(std::cin >> str){
       p = 0;
       advance();
       Procedure_S();
       std::cout << isright << std::endl;
   }
   return 0;
}
