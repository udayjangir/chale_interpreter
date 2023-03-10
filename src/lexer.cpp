#include <iostream>
#include <unordered_map>
#include <deque>
#include <stdio.h>
#include "lexer.h"

// define some expressions
#define IS_DIGIT(x) (0 <= x-'0' && x-'0' <= 9)
#define IS_LETTER(x) ((0 <= x-'a' && x-'a' < 26) || (0 <= x-'A' && x-'A' < 26))
#define IS_UNDERSCORE(x) (x=='_')

using namespace std;

// declare global variables to store tokens, and symbol table
deque<pair<string,string>> tokens;
unordered_map<string,pair<string,int>> sym_table;

// identify the tokens from input (lexical analysis)
bool getTokens(string input) {
    
    string token = "";
    bool isTokenInt = false;
    for (int i=0; i<input.size(); i++){
        char currChar = input[i];

        // ignore white spaces and tabs
        if (currChar == ' ' || currChar == '\t' || currChar == '\n'){
            if (token.size() != 0){
                if (isTokenInt) tokens.push_back(make_pair(INT, token));
                else {
                    if (token == INT) tokens.push_back(make_pair(TYPE, INT));
                    else if (token == COUT) tokens.push_back(make_pair(KEY, COUT));
                    else tokens.push_back(make_pair(ID, token));
                }
                token = "";
            }
            else continue;
        }

        // add the token when any punctuation is encountered
        else if (currChar == '+' || currChar == ';' || currChar == ',' || currChar == '=' || currChar == '(' || currChar == ')' || currChar == '<' || currChar == '-'){
            if (token.size() != 0){
                if (isTokenInt) tokens.push_back(make_pair(INT, token));
                else {
                    if (token == INT) tokens.push_back(make_pair(TYPE, INT));
                    else if (token == COUT) tokens.push_back(make_pair(KEY, COUT));
                    else tokens.push_back(make_pair(ID, token));
                }
                token = "";
            }
            switch (currChar){
                case '=':
                    tokens.push_back(make_pair(OPR, ASSIGN));
                    break;
                case '+':
                    tokens.push_back(make_pair(OPR, PLUS));
                    break;
                case ';':
                    tokens.push_back(make_pair(PUNC, SCOLON));
                    break;
                case ',':
                    tokens.push_back(make_pair(PUNC, COMMA));
                    break;
                case '(':
                    tokens.push_back(make_pair(PUNC, LPAREN));
                    break;
                case ')':
                    tokens.push_back(make_pair(PUNC, RPAREN));
                    break;
                
                // check '<<' token to print the output with 'cout'
                case '<':
                    if (i+1 < input.size() && input[i+1] == '<'){
                        tokens.push_back(make_pair(OPR, DLT));
                        i++;
                    }
                    else {
                        cout << "\033[31mError: Type(Lexical error): Expected '<<' but found only '<'\033[0m" << endl;
                        tokens.clear();
                        return false;
                    }
                    break;
                
                // if negative integer is found
                // must be without space, because then lexer treats '-' as separate token and gives error
                case '-':
                    if (i+1 < input.size() && IS_DIGIT(input[i+1])){
                        isTokenInt = true;
                        token = "-";
                    }
                    else {
                        cout << "\033[31mError: Type(Lexical error): Expected negative integer but found only  '-'\033[0m" << endl;
                        tokens.clear();
                        return false;
                    }
            }
        }

        else {

            // mark the type of token (int or id)
            if (token.size() == 0 && (IS_DIGIT(currChar) || IS_LETTER(currChar) || IS_UNDERSCORE(currChar))){
                if (IS_DIGIT(currChar)) isTokenInt = true;
                else if (IS_LETTER(currChar) || IS_UNDERSCORE(currChar)) isTokenInt = false;

                token += currChar;
            }

            // if identifier starts with a number, throw error
            else if (isTokenInt && (IS_LETTER(currChar) || IS_UNDERSCORE(currChar))){
                cout << "\033[31mError: Type(Lexical error): Unexpected identifier name found " << "'" + token + currChar + "'" << "\033[0m" <<  endl;
                tokens.clear();
                return false;
            }

            // else keep adding characters to token
            else if (IS_DIGIT(currChar) || IS_LETTER(currChar) || IS_UNDERSCORE(currChar)){
                token += currChar;
            }

            // throw error if any other unexpected character found
            else if (currChar != '_'){
                cout << "\033[31mError: Type(Lexical error): Unidentified character '" <<  currChar <<  "' found at index = " << i << "\033[0m" <<  endl;
                tokens.clear();
                return false;
            }
        }
    }

    // put the last remaining token (if any) in the tokens
    if (token.size() != 0){

        if (isTokenInt) tokens.push_back(make_pair(INT, token));
        else {
            if (token == INT) tokens.push_back(make_pair(TYPE, INT));
            else if (token == COUT) tokens.push_back(make_pair(KEY, COUT));
            else tokens.push_back(make_pair(ID, token));
        }

        token = "";
    }

    return true;
}

// shows the output of lexer i.e. tokens info
void showOutput(){
    cout << "\n---------------------------------------------\n" << endl;
    cout << "OUTPUT OF LEXER:\n\n" << "Token count: " << tokens.size() << endl;

    cout << "\nList of tokens:\n" << endl;
    for (auto token: tokens){
        cout << token.first << "\t" << token.second << endl;
    }
    cout << "\n---------------------------------------------\n" << endl;
}

// function to take input characters and process on them
bool lex(){

    // cout << "\nEnter an expression: ";
    string input;
    getline(cin, input);

    return getTokens(input);
    // showOutput();
}

// int main(){
//     lex();
// }