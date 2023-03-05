#include <iostream>
#include "lexer.h"
using namespace std;

// function to work upon the tokens received from lexer and add the integers
int addition(vector<pair<string,string>> tokens){
    int totalSum = 0;
    bool lastTokenInt = false;
    for (pair<string,string> token : tokens){

        // throw error when consecutive <opr><opr> or <int><int> found 
        if (lastTokenInt && token.first == "int" || !lastTokenInt && token.first == "opr"){
            cout << "\033[31mError: Type(Evaluation error): Not a valid expression\033[0m" << endl;
            exit(1);
        }

        if (token.first == "int"){
            totalSum += stoi(token.second);
            lastTokenInt = true;
        }
        else lastTokenInt = false;
    }

    // throw error if <opr> is found at the end with no further operand token
    if (!lastTokenInt){
        cout << "\033[31mError: Type(Evaluation error): Operand for '+' is missing at the end\033[0m" << endl;
        exit(1);
    }

    return totalSum;
}

int main(){

    extern vector<pair<string,string>> tokens;

    lex();

    cout << "Result of the expression: " << addition(tokens) << endl;
    cout << endl;
}