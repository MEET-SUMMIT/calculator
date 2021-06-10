#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cmath>
#include<cstdlib>
#include<string>
#include<list>
#include <forward_list>
#include<vector>
#include<unordered_map>
#include<algorithm>
#include <array>
#include <regex>
#include<random>
#include<stdexcept>
#include<exception>
using namespace std;
class Token { // a very simple user-defined type
public:
 char kind;
 double value;
};
Token get_token(){// function to read a token from cin
    Token t;
   // cout<<"t.kind:";
    cin>>t.kind;
    if(t.kind=='8')
   { //cout<<"t.value";
   cin>>t.value;}
    return t;
} 
double primary();
double term();
double expression();
int main()
try {
 while (cin)
 cout << expression() << '\n';
}
catch (exception& e) {
 cerr << e.what() << '\n';
 return 1;
}
catch (...) {
 cerr << "exception \n";
 return 2;
}
double primary()
{
 Token t = get_token();
 switch (t.kind) {
 case '(': // handle ‘(‘ expression ‘)’
 { double d = expression();
 t = get_token();
 if (t.kind != ')') throw("')' expected");
 return d;
 }
 case '8': // we use ‘8’ to represent a number
 return t.value; // return the number’s value
 default:
 throw("primary expected");
 }
}
double term()
{
 double left;
 left = primary();
 Token t = get_token();
 
 while (true) {
 switch (t.kind) {
 case '*':
 left *= primary();
 t = get_token();
 break;
 case '/':
 { double d = primary();
 if (d == 0) throw("divide by zero");
 left /= d;
 t = get_token();
 break;
 }
 default:
 return left;
 }
 }
}
double expression()
{
 double left;
 left = term(); // read and evaluate a Term
 Token t = get_token(); // get the next token
 while (true) {
 switch (t.kind) {
 case '+':
 left += term(); // evaluate Term and add
 t = get_token();
 break;
 case '-':
 left -= term(); // evaluate Term and subtract
 t = get_token();
 break;
 default:
 return left; // finally: no more + or –; return the answer
 }
 }
}