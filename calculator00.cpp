#include "./std_lib_facilities.h"
using namespace std;
const char number='8';
const char print=';';
const char quit='q';
const string prompt = "> ";
const string result = "= "; 
class Token { // a very simple user-defined type
public:
 char kind;
 double value;
};
class Token_stream {
public:
 Token get(); // get a Token 
 void putback(Token t); // put a Token back
 void ignore(char c); // discard characters up to and including a c
private:
 bool full {false}; // is there a Token in the buffer?
 Token buffer; // here is where we keep a Token put back using putback()
};
void Token_stream::ignore(char c)
 // c represents the kind of Token
{
 // first look in buffer:
 if (full && c==buffer.kind) {
 full = false;
 return;
 }
 full = false;
 // now search input:
 char ch = 0;
 while (cin>>ch)
 if (ch==c) return;
}
void Token_stream::putback(Token t)
{
 if (full) error("putback() into a full buffer"); 
 buffer = t; // copy t to buffer
 full = true; // buffer is now full
}
Token Token_stream::get()
{
 if (full) { // do we already have a Token ready?
 full = false; // remove Token from buffer
 return buffer;
 }
 char ch;
 cin >> ch; 
 switch (ch) {
 case print:
 case quit:
  case '{': 
  case '}': 
  case '(': 
  case ')': 
  case '+': 
  case '-': 
  case '*': 
  case '/': 
  case '%': 
  case '!':
 return Token{ch}; //let each character represent itself
 case '.':
 case '0': case '1': case '2': case '3': case '4':
 case '5': case '6': case '7': case '8': case '9':
 { cin.putback(ch); // put digit back into the input stream
 double val;
 cin >> val; // read a floating-point number
 return Token{number,val}; //“a number”
 }
 default:
 error("Bad token");
 }
}
Token_stream ts;
int factorial (double num){ //to find factorial of a number
    if(num!=(int)num) error("integer expected as factorial argument");
     if(num==0||num==1)
    return 1;
    else{
    return num*factorial(num-1);  //recursive call of factorial
    } 
} 
void clean_up_mess();
void calculate();
double expression();
double primary();
double term();
double expression();
int main()
try {
    calculate();
    return 0;
}
catch (...) {
 cerr << "exception \n";
 return 1;
}
double primary()
{
 Token t = ts.get();
 switch (t.kind) {
     case '{': // handle ‘{’ expression ‘}’
 {
 double d = expression();
 t = ts.get();
 if (t.kind != '}') error("'}' expected");
 t=ts.get();
 if(t.kind=='!') //factorial '1' will be either after number or braces
 return factorial(d);
 else
 {ts.putback(t);
 return d;
 }
 }
 case '(': // handle ‘(’ expression ‘)’
 {
 double d = expression();
 t = ts.get();
 if (t.kind != ')') error("')' expected");
 t=ts.get();
 if(t.kind=='!')
 return factorial(d);
 else
 {ts.putback(t);
 return d;}
 }
 case number:
{ double num=t.value;
 t=ts.get();
 if(t.kind=='!')
 return factorial(num);
 else{
     ts.putback(t);
 return num; 
 }
 } // return the number’s value
 case '-':
 return  -primary();
 case '+':
 return primary();
default:
 error("primary expected");
 }
}
double term()
{
 double left = primary();
 Token t = ts.get(); // get the next Token from the Token stream
 while (true) {
 switch (t.kind) {
 case '*':
 left *= primary();
 t = ts.get();
 break;
 case '/':
 { double d = primary();
 if (d == 0) error("divide by zero");
 left /= d;
 t = ts.get();
 break;
 }
 case '%':                      //modulus:only defined for integers
{ int i1 = narrow_cast<int>(left);
 int i2 = narrow_cast<int>(primary());
 if (i2 == 0) error("%: divide by zero");
 left = i1%i2;
 t = ts.get();
 break;
}
 default:
 ts.putback(t); // put t back into the Token stream
 return left;
 }
 }
}
double expression()
{
 double left = term(); // read and evaluate a Term
 Token t = ts.get(); // get the next Token from the Token stream
 while (true) {
 switch (t.kind) {
 case '+':
 left += term(); // evaluate Term and add
 t = ts.get();
 break;
 case '-':
 left -= term(); // evaluate Term and subtract
 t = ts.get();
 break;
 default:
 ts.putback(t); // put t back into the token stream
 return left; // finally: no more + or –; return the answer
 }
 }
}
void calculate(){
 while (cin)
 try{ 
      cout<<prompt;
 Token t=ts.get();
 while(t.kind==print)t=ts.get();
     if(t.kind==quit)
     return;
 ts.putback(t);
     cout <<result << expression()<< '\n';
 t=ts.get();
}
catch (exception& e) {
 cerr << e.what() << '\n'; 
 clean_up_mess();
}
}
void clean_up_mess()
{
 ts.ignore(print);
}