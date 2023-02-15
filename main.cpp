#include <iostream>
#include "shunting-yard.h"
#include "structs.h"
#include "operators.h"
#include <string>
#include <fstream>

using namespace std;


char* alphabet;


string preprocess(string);



int main(int argc, char* argv[])
{
    string filePath = "regex/foo";
    if (argc > 1)
        filePath = argv[0];

    ifstream file = ifstream(filePath);
    string regex;
    getline(file, regex);

    cout << "Input:\t\t" << regex << "\n";
    cout << "Preprocessor:\t" << preprocess(regex) << endl;

    
    
    return 0;
}


string nonconcatwith = ")|";
string nonconcat = "()|";

string preprocess(string regex)
{
    string out = "";
    int i = 0;
    while (i < regex.size())
    {
        out += regex[i];
        
        if(i+1 < regex.size() && 
            nonconcatwith.find(regex[i+1]) == -1  &&
            nonconcat.find(regex[i]) == -1)
            out += ".";
        i++;
    }
    return out;
}
