#include <iostream>
#include <vector>

using namespace std;

vector<pair<string,string>> getTokens(string input) {
    
    string token = "";
    vector<pair<string,string>> tokens;
    for (int i=0; i<input.size(); i++){
        if (input[i] == ' ' || input[i] == '\t'){
            if (token.size() != 0){
                tokens.push_back(make_pair("int", token));
                token = "";
            }
            else continue;
        }

        else if (input[i] == '+'){
            if (token.size() != 0){
                tokens.push_back(make_pair("int", token));
                tokens.push_back(make_pair("opr", "+"));
                token = "";
            }
            else tokens.push_back(make_pair("opr", "+"));
        }

        else {
            if (input[i] - '0' < 9) token += input[i];
            else {
                cout << "Error: Unidentified character found at index = " << i;
                exit(1);
            }
        }
    }

    if (token.size() != 0) tokens.push_back(make_pair("int", token));
    return tokens;
}

int main(){
    cout << "Enter the input: ";
    string input;
    getline(cin, input);

    vector<pair<string,string>> tokens;
    tokens = getTokens(input);

    for (int i=0; i<tokens.size(); i++){
        cout << tokens[i].first << " " << tokens[i].second << endl;
    }
}