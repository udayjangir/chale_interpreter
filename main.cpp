#include <iostream>
#include <vector>
using namespace std;

int addition(vector<pair<string,string>> &tokens){
    int totalSum = 0;
    bool lastTokenInt = false;
    for (pair<string,string> token : tokens){
        if (lastTokenInt && token.first == "int" || !lastTokenInt && token.first == "opr"){
            cout << "Error: Not a valid expression";
            exit(1);
        }

        if (token.first == "int"){
            totalSum += stoi(token.second);
            lastTokenInt = true;
        }
        else lastTokenInt = false;
    }

    if (!lastTokenInt){
        cout << "Error: Operand for '+' is missing at the end";
        exit(1);
    }

    return totalSum;
}

int main(){
    vector<pair<string,string>> tokens;
    tokens.push_back(make_pair("int", "12"));
    tokens.push_back(make_pair("opr", "+"));
    tokens.push_back(make_pair("int", "16"));
    tokens.push_back(make_pair("opr", "+"));
    // tokens.push_back(make_pair("opr", "+"));

    cout << addition(tokens);
}