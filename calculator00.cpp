#include "./std_lib_facilities.h"
using namespace std;
const char number='8';
const char print=';';
const char quit='q';
const string prompt = "> ";
const string result = "= ";

class Variable {    //made a class to store variables name with values
public:
 string name;       
 double value;
 Variable(string var,double val):name{var},value{val} { }   //constructor
}; 

class Token {
public:
 char kind;
 double value;
 string name;
 Token() { }
 Token(char ch) :kind{ch} { } // initialize kind with ch
 Token(char ch, double val) :kind{ch}, value{val} { } // initialize kind
 // and value
 Token(char ch, string n) :kind{ch}, name{n} { } // initialize kind
 // and name
};


class Token_stream {
public:
 Token get(); // get a Token 
 void putback(Token t); // put a Token back
 void ignore(char c); // discard characters up to and including a c
private:
 bool full {false}; // is there a Token in the buffer?
 Token buffer; // here is where we keep a Token put back using putback()
} ts;


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


const char name = 'a'; // name token
const char let = 'L'; // declaration token
const string declkey = "let"; // declaration keyword


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
  case '=':
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
 if (isalpha(ch)) {
 string s;
 s += ch;
 while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s+=ch;
 cin.putback(ch);
 if (s == declkey) return Token{let}; // declaration keyword
 return Token{name,s};
 }
 error("Bad token");
}
}


int factorial (double num){ //to find factorial of a number
    if(num!=(int)num) error("integer expected as factorial argument");
     if(num==0||num==1)
    return 1;
    else{
    return num*factorial(num-1);  //recursive call of factorial
    } 
}


vector<Variable> var_table;
double get_value(string s)
 // return the value of the Variable named s
{
 for (const Variable& v : var_table)
 if (v.name == s) return v.value;
 error("get: undefined variable ", s);
}

bool is_declared(string var)
 // is var already in var_table?
{
 for (const Variable& v : var_table)
 if (v.name == var) return true;
 return false;
}

double define_name(string var, double val)
 // add (var,val) to var_table
{
 if (is_declared(var)) error(var," declared twice");
 var_table.push_back(Variable(var,val));
 return val;
}

void set_value(string s, double d)
 // set the Variable named s to d
{
 for (Variable& v : var_table)
 if (v.name == s) {
 v.value = d;
 return;
 }
 error("set: undefined variable ", s);
}

double expression();
double declaration()
 // assume we have seen "let”
 // handle: name = expression
 // declare a variable called "name” with the initial value "expression”
{
 Token t = ts.get();
 if (t.kind != name) error ("name expected in declaration");
 string var_name = t.name;
 Token t2 = ts.get();
 if (t2.kind != '=') error("= missing in declaration of ", var_name);
 double d = expression();
 define_name(var_name,d);
 return d;
}

double statement()
{
 Token t = ts.get();
 switch (t.kind) {
 case let:
 return declaration();
 default:
 ts.putback(t);
 return expression();
 }
}

void clean_up_mess();
void calculate();
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
 case name:
 { double num=get_value(t.name);
 t=ts.get();
 if(t.kind=='!')
 return factorial(num);
 else{
     ts.putback(t);
 return num; 
 }
 } // return the variable’s value
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
     cout <<result << statement()<< '\n';
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