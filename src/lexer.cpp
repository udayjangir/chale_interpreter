#include <iostream>
#include "lexer.h"
#include <stdio.h>

using namespace std;

// declare a global variable to store tokens
vector<pair<string,string>> tokens;

// identify the tokens from input (lexical analysis)
void getTokens(string input) {
    
    string token = "";
    for (int i=0; i<input.size(); i++){

        // ignore white spaces and tabs
        if (input[i] == ' ' || input[i] == '\t'){
            if (token.size() != 0){
                tokens.push_back(make_pair("int", token));
                token = "";
            }
            else continue;
        }

        // save the <int> and <opr> tokens when '+' is encountered
        else if (input[i] == '+'){
            if (token.size() != 0){
                tokens.push_back(make_pair("int", token));
                tokens.push_back(make_pair("opr", "+"));
                token = "";
            }
            else tokens.push_back(make_pair("opr", "+"));
        }

        else {
            if (0 <= input[i] - '0' && input[i] - '0' < 9) token += input[i];

            // throw error if any character other than '+' or [0-9] found
            else {
                cout << "\033[31mError: Type(Lexical error): Unidentified character found at index = " << i << "\033[0m" <<  endl;
                exit(1);
            }
        }
    }

    // put the last remaining <int> (if any) in the tokens
    if (token.size() != 0) tokens.push_back(make_pair("int", token));
    return;
}

// shows the output of lexer i.e. tokens info
void showOutput(){
    cout << "\n---------------------------------------------\n" << endl;
    cout << "OUTPUT OF LEXER:\n\n" << "Token count: " << tokens.size() << endl;

    cout << "\nList of tokens:" << endl;
    for (int i=0; i<tokens.size(); i++){
        cout << tokens[i].first << " " << tokens[i].second << endl;
    }
    cout << "\n---------------------------------------------\n" << endl;
}

// function to take input characters and process on them
void lex(){

    cout << "\nEnter an expression: ";
    string input;
    getline(cin, input);

    getTokens(input);
    showOutput();
}