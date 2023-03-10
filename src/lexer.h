#ifndef HELPER_HPP
#define HELPER_HPP
#include <string>

// define macros for token strings
#define INT "int"
#define COUT "cout"
#define EXIT "exit"
#define NONE "none"

#define ID "id"
#define OPR "opr"
#define PUNC "punc"
#define KEY "key"
#define TYPE "type"


// define macros for symbols
#define PLUS "+"
#define MINUS "-"
#define ASSIGN "="
#define SCOLON ";"
#define COMMA ","
#define LPAREN "("
#define RPAREN ")"
#define DLT "<<"

using namespace std;

bool getTokens(string input);
bool lex();

#endif