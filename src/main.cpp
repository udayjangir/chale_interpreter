#include <iostream>
#include <deque>
#include <unordered_map>
#include <limits.h>
#include "lexer.h"
using namespace std;

// declare some global variables
extern deque<pair<string,string>> tokens;
extern unordered_map<string /*id*/ ,pair<string,int> /*(type,value)*/> sym_table;
pair<string,string> curr_token;
bool runningState;

// fucntion to show errors
void error(int code) {
    if (!runningState) return;

    switch (code) {

        // if an unexpected token is encountered
        case 11:
            cout << "\033[31mError: Type(Syntax error): Unexpected token " << curr_token.second << "\033[0m" << endl;
            break;
        
        // if not a valid statement line
        case 12:
            cout << "\033[31mError: Type(Syntax error): Invalid statement line\033[0m" << endl;
            break;
    }
    runningState = false;
    curr_token = make_pair(KEY,NONE);
    tokens.clear();
    return;
}

void error(int code, string extra_info){
    if (!runningState) return;

    switch (code){

        // matching error
        case 21:
            cout << "\033[31mError: Type(Syntax error): Expected token '" << extra_info << "' but found '" << curr_token.second << "'\033[0m" << endl;
            break;

        // variable declaration errors

        // if already used an identifier name
        case 22:
            cout << "\033[31mError: Type(Syntax error): Identifier with name '" << extra_info << "' is already declared\033[0m" << endl;
            break;
        
        // if a name is not matched with any key or id
        case 23:
            cout << "\033[31mError: Type(Syntax error): Name '" << extra_info << "' is not defined\033[0m" << endl;
            break;

        // variable declaration error
        case 24:
            cout << "\033[31mError: Type(Syntax error): Variable '" << extra_info << "' is not declared properly\033[0m" << endl;
            break;
        
        // if variable used without initialisation
        case 25:
            cout << "\033[31mError: Type(Syntax error): Variable '" << extra_info << "' is not initialised\033[0m" << endl;
            break;
    }

    runningState = false;
    curr_token = make_pair(KEY,NONE);
    tokens.clear();
    return;
}

// function to get the token
pair<string,string> getToken(){
    if (tokens.empty()){
        curr_token = make_pair(KEY,NONE);
        return curr_token;
    }
    
    pair<string,string> token = tokens.front();
    tokens.pop_front();

    return token;
}

// function to match for the terminal
void match(string token){
    if (!runningState) return;

    curr_token = getToken();
    if (curr_token.second == token) return;
    else error(21, token);
}

// to check if an ID is in symbol table
bool checkID(string id){
    if (runningState && sym_table.find(id) != sym_table.end()) return true;
    else return false;
}

// matches ID in the grammar rule and handles errors {11,22}
string matchID(){
    if (!runningState) return "";

    curr_token = getToken();
    if (curr_token.first == ID) {
        string id_name = curr_token.second;
        if (!checkID(id_name)) return id_name;

        // if id_name already in symbol table, handle error
        else {
            error(22,id_name);
            return id_name;
        }
    }
    
    // if some other token found instead of ID
    else error(11); return "";
}

int expression(){
    // to avoid evaluating expression if we have an error already
    if (!runningState) return INT_MIN;

    int result = 0;
    curr_token = getToken();
    
    // match for expr -> ID rule
    if (curr_token.first == ID) {
        string id_name = curr_token.second;
        if (checkID(id_name)){

            // if variable is declared but not initialised
            if (sym_table[id_name].second == INT_MIN) error(25, id_name);

            // else add the value of id to result
            result += sym_table[id_name].second;

            // evaluate remaining expression
            int temp = expression();
            if (temp == INT_MIN) return INT_MIN;
            else return result + temp;
        }

        // if no such name exists in symbol table
        else error(23,id_name); return INT_MIN;
    }

    // match for expr -> INT rule
    else if (curr_token.first == INT) {
        result += stoi(curr_token.second);
        int temp = expression();
        if (temp == INT_MIN) return INT_MIN;
        else return result + temp;
    }

    // match for expr -> "+" expr rule
    else if (curr_token.second == PLUS) {
        int temp = expression();
        if (temp == INT_MIN) return INT_MIN;
        else return result + temp;
    }

    // otherwise
    else {
        tokens.push_front(curr_token);
        return result;
    }

}

// update the symbol table
void updateSymTable(string id, string type, int value){
    if (!runningState) return;
    sym_table[id] = make_pair(type,value);
    return;
}

// matches declaration rule (assignment -> declaration)
bool declaration(string id_name){
    if (runningState && curr_token.second == SCOLON){
        updateSymTable(id_name,INT,INT_MIN);
        return true;
    }
    else return false;
}

// matches initialisation rule (assignment -> initialisation)
bool initialisation(string id_name){
    if (runningState && curr_token.second == ASSIGN){
        int id_value = expression();
        match(SCOLON);
        updateSymTable(id_name,INT,id_value);
        return true;
    }
    else return false;
}

// to handle assignment -> declaration | initialisation | reassign
bool assignment(){
    if (!runningState) return false;

    // if 'int' keyword found, we should declare or initialise new variable
    if (curr_token.second == INT){

        // match ID tokens and handle errors
        string id_name = matchID();

        curr_token = getToken();
        if (declaration(id_name) || initialisation(id_name)) return true;

        // if not a proper assignment syntax
        else{
            error(24, id_name);
            return false;
        }
    }

    // otherwise reassign some new value to the variable
    else if (curr_token.first == ID){
        string id_name = curr_token.second;
        if (checkID(id_name)) {
            match(ASSIGN);
            int id_value = expression();
            if (id_value == INT_MIN) return false;
            match(SCOLON);
            updateSymTable(id_name,INT,id_value);
        }
        else {
            error(23, id_name);
        }
        return true;
    }
    else return false;
}

// prints using 'cout' keyword
bool print(){
    if (runningState && curr_token.second == COUT){
        match(DLT);
        int value = expression();
        if (value == INT_MIN) return false;
        match(SCOLON);

        // prints only if we have not encountered any error
        if (runningState) cout << value << endl;
        return true;
    }
    else return false;
}

// exit the interpreter using 'exit' keyword
bool exitProgram(){
    if (runningState && curr_token.second == EXIT) {
        match(SCOLON);
        if (runningState) exit(0);
        else return false;
    }
    else return false;
}

// execute a line and check for rule line -> exit | print | assignment
bool line(){
    runningState = true;
    curr_token = getToken();
    if (exitProgram() || print() || assignment()) return true;
    else return false;
}

// main function
int main(){

    // continuosly take input statement lines
    while (true){

        // first analyse lexically, if all fine, execute the line
        if (lex()){

            // to handle multiple statements in one line
            while (!tokens.empty()){
                if (line()) continue;

                // if not a valid statement line
                else error(12);
            }
        }
    }
    cout << endl;
}