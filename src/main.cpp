#include <iostream>
#include <deque>
#include <unordered_map>
#include <limits.h>
#include <vector>
#include "lexer.h"
using namespace std;

// declare some global variables
extern deque<pair<string,string>> tokens;
extern unordered_map<string /*id*/ ,pair<string,int> /*(type,value)*/> sym_table;
pair<string,string> curr_token;
bool running_state;

// function to show errors
void error(int code) {
    if (!running_state) return;

    switch (code) {

        // if an unexpected token is encountered
        case 11:
            cout << "\033[31mError: Type(Syntax error): Unexpected token " << curr_token.second << "\033[0m" << endl;
            break;
        
        // if not a valid statement line
        case 12:
            cout << "\033[31mError: Type(Syntax error): Invalid statement line\033[0m" << endl;
            break;
        
        // if empty expression found
        case 13:
            cout << "\033[31mError: Type(Syntax error): Invalid expression found\033[0m" << endl;
            break;
        
        // variable declaration error
        case 14:
            cout << "\033[31mError: Type(Syntax error): Variable is not declared properly\033[0m" << endl;
            break;
    }

    // perform a reset
    running_state = false;
    curr_token = make_pair(KEY,NONE);
    tokens.clear();
    return;
}

void error(int code, string extra_info){
    if (!running_state) return;

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
        
        // if variable used without initialisation
        case 24:
            cout << "\033[31mError: Type(Syntax error): Variable '" << extra_info << "' is not initialised\033[0m" << endl;
            break;
    }

    running_state = false;
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
    if (!running_state) return;

    curr_token = getToken();
    if (curr_token.second == token) return;
    else error(21, token);
}

// to check if an ID is in symbol table
bool checkID(string id){
    if (running_state && sym_table.find(id) != sym_table.end()) return true;
    else return false;
}

// matches ID in the grammar rule and handles errors {11,22}
string matchID(){
    if (!running_state) return "";

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

// match rule expr -> id | int
int expression(){
    
    // match for expr -> ID rule
    if (curr_token.first == ID) {
        string id_name = curr_token.second;

        if (checkID(id_name)){

            // if variable is declared but not initialised
            if (sym_table[id_name].second == INT_MIN) error(24, id_name);

            else return sym_table[id_name].second;
        }

        // if no such name exists in symbol table
        else error(23,id_name);
        return INT_MIN;
    }

    // match for expr -> INT rule
    else if (curr_token.first == INT) return stoi(curr_token.second);

    // if valid expression not found (i.e. expr -> int | id not matched)
    else {
        tokens.push_front(curr_token);
        error(13);
        return INT_MIN;
    }

}

// expression sequence, performs addition
int exprSeq(){
    if (!running_state) return INT_MIN;

    curr_token = getToken();
    int result = 0;
    result += expression();
    if (result == INT_MIN) return INT_MIN;

    pair<string,string> next_token = getToken();
    tokens.push_front(next_token);

    // check if sequence ended with ';' else match for '+' instead
    if (next_token.second == SCOLON || next_token.second == COMMA) return result;
    else if (next_token.second == PLUS){
        match(PLUS);
        return result + exprSeq();
    }
    else return INT_MIN;
}

// update the symbol table
void updateSymTable(string id, string type, int value){
    if (!running_state) return;
    sym_table[id] = make_pair(type,value);
    return;
}

bool declaration(vector<pair<string,int>> &ids);
bool initialisation(vector<pair<string,int>> &ids);

// matches declaration rule (assignment -> declaration)
bool declaration(vector<pair<string,int>> &ids){

    // if ';' found, declaration ends
    if (running_state && curr_token.second == SCOLON){
        while (!ids.empty()){

            // update symbol table for all the ids
            pair<string,int> id = ids.back();
            ids.pop_back();
            updateSymTable(id.first,INT,id.second);
        }
        return true;
    }
    
    // if multiple declarations using comma, process them
    else if (running_state && curr_token.second == COMMA){

        // add the declared id the the list of ids
        string other_id = matchID();
        ids.push_back(make_pair(other_id,INT_MIN));

        curr_token = getToken();
        return initialisation(ids) || declaration(ids);
    }
    else return false;
}

// matches initialisation rule (assignment -> initialisation)
bool initialisation(vector<pair<string,int>> &ids){
    if (running_state && curr_token.second == ASSIGN){
        int id_value = exprSeq();

        // add the id with its initialisation value to list of ids
        pair<string,int> id = ids.back();
        ids.pop_back();
        id.second = id_value;
        ids.push_back(id);

        // handle multiple initialisations by checking comma
        curr_token = getToken();
        if (curr_token.second == COMMA){

            // update ids list with new id
            string other_id = matchID();
            ids.push_back(make_pair(other_id,INT_MIN));
            curr_token = getToken();

            return initialisation(ids) || declaration(ids);
        }
        else tokens.push_front(curr_token);
        
        // if semicolon found, then assign values to all ids in list
        match(SCOLON);
        while (!ids.empty()){
            pair<string,int> id = ids.back();
            ids.pop_back();
            updateSymTable(id.first,INT,id.second);
        }
        return true;
    }
    else return false;
}

// to handle assignment -> declaration | initialisation | reassign
bool assignment(){
    if (!running_state) return false;

    // if 'int' keyword found, we should declare or initialise new variable
    if (curr_token.second == INT){

        // match ID tokens and handle errors
        string id_name = matchID();
        vector<pair<string,int>> ids ;
        ids.push_back(make_pair(id_name,INT_MIN));

        curr_token = getToken();
        if (declaration(ids) || initialisation(ids)) return true;

        // if not a proper assignment syntax show error
        else{
            error(14);
            return false;
        }
    }

    // otherwise reassign some new value to the variable
    else if (curr_token.first == ID){
        string id_name = curr_token.second;
        if (checkID(id_name)) {
            match(ASSIGN);
            int id_value = exprSeq();
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
    if (running_state && curr_token.second == COUT){
        match(DLT);
        int value = exprSeq();
        match(SCOLON);
        if (value == INT_MIN) return false;

        // prints only if we have not encountered any error
        if (running_state) cout << value << endl;
        return true;
    }
    else return false;
}

// exit the interpreter using 'exit' keyword
bool exitProgram(){
    if (running_state && curr_token.second == EXIT) {
        match(SCOLON);
        if (running_state) exit(0);
        else return false;
    }
    else return false;
}

// execute a line and check for rule line -> exit | print | assignment
bool line(){
    running_state = true;
    curr_token = getToken();
    if (exitProgram() || print() || assignment()) return true;
    else return false;
}

// main function
int main(){
    cout << "\nChale Interpreter 1.0.1\nType 'exit;' to terminate the program\n" << endl;

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