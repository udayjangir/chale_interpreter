#include <iostream>
#include <deque>
#include <unordered_map>
#include <limits.h>
#include "lexer.h"
using namespace std;

extern deque<pair<string,string>> tokens;
extern unordered_map<string /*id*/ ,pair<string,int> /*(type,value)*/> sym_table;
pair<string,string> curr_token;
bool runningState = true;

// fucntion to show errors
void error(int code) {
    if (!runningState) return;

    switch (code) {

        // if an unexpected token is encountered
        case 11:
            cout << "\033[31mError: Type(Syntax error): Unexpected token " << curr_token.second << "\033[0m" << endl;
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

    // cout << token + " Token matched" << endl;
    if (!runningState) return;
    curr_token = getToken();

    if (curr_token.second == token) return;
    else error(21, token);
    // cout << "returning from match()" << endl;
}

bool checkID(string id){
    // cout << "ID checked" << endl;
    if (runningState && sym_table.find(id) != sym_table.end()) return true;
    else return false;
}

string matchID(){
    // cout << "ID matched" << endl;
    if (!runningState) return "";
    curr_token = getToken();
    // cout << curr_token.second << endl;
    if (curr_token.first == ID) {
        string id_name = curr_token.second;
        if (!checkID(id_name)) return id_name;
        else error(22,id_name); return id_name;
    }
    
    else error(11); return "";
}

int expression(){
    // to avoid evaluating expression if we have an error
    if (!runningState) return INT_MIN;
    int result = 0;
    curr_token = getToken();
    
    if (curr_token.first == ID) {
        string id_name = curr_token.second;
        if (checkID(id_name)){
            if (sym_table[id_name].second == INT_MIN) error(25, id_name);
            result += sym_table[id_name].second;
            int temp = expression();
            if (temp == INT_MIN) return INT_MIN;
            else return result + temp;
        }
        else error(23,id_name); return INT_MIN;
    }
    else if (curr_token.first == INT) {
        result += stoi(curr_token.second);
        int temp = expression();
        if (temp == INT_MIN) return INT_MIN;
        else return result + temp;
    }
    else if (curr_token.second == PLUS) {
        int temp = expression();
        if (temp == INT_MIN) return INT_MIN;
        else return result + temp;
    }
    else {
        tokens.push_front(curr_token);
        return result;
    }

}

void updateSymTable(string id, string type, int value){
    if (!runningState) return;
    // cout << "SymTable updated" << endl;
    sym_table[id] = make_pair(type,value);
    return;
}

bool declaration(string id_name){
    // cout << "Declared" << endl;
    if (runningState && curr_token.second == SCOLON){
        updateSymTable(id_name,INT,INT_MIN);
        return true;
    }
    else return false;
}

bool initialisation(string id_name){
    // cout << "Initialised" << endl;
    if (runningState && curr_token.second == ASSIGN){
        int id_value = expression();
        match(SCOLON);
        updateSymTable(id_name,INT,id_value);
        return true;
    }
    else return false;
}

bool assignment(){
    if (!runningState) return false;

    if (curr_token.second == INT){
        string id_name = matchID();
        curr_token = getToken();
        if (declaration(id_name) || initialisation(id_name)) return true;
        else error(24, id_name); return false;
    }
    else if (curr_token.first == ID){
        string id_name = curr_token.second;
        if (checkID(id_name)) {
            match(ASSIGN);
            int id_value = expression();
            if (id_value == INT_MIN) return false;
            match(SCOLON);
            updateSymTable(id_name,INT,id_value);
        }
        else {error(23, id_name);}
        return true;
    }
    else return false;
}

bool print(){
    if (runningState && curr_token.second == COUT){
        match(DLT);
        int value = expression();
        if (value == INT_MIN) return false;
        match(SCOLON);
        if (runningState) cout << value << endl;
        return true;
    }
    else return false;
}

bool exitProgram(){
    if (runningState && curr_token.second == EXIT) {
        match(SCOLON);
        exit(0);
    }
    else return false;
}

bool line(){
    // cout << "Line executed" << endl;
    runningState = true;
    curr_token = getToken();
    if (exitProgram() || print() || assignment()) return true;
    else return false;
}

void program(){
    while (!tokens.empty()) {
        if (line()) continue;
    } 
}

int main(){

    while (true){
        if (lex()) program();
        else continue;
    }

    cout << endl;
}