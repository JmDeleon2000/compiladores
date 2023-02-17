#include <iostream>
#include "shunting-yard.h"
#include "structs.h"
#include "operators.h"
#include <string>
#include <fstream>

using namespace std;




string preprocess(string);

void postorderprint(node*);
automata* createAutomata(node*);


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

    try{
    node* a  = djkstra(preprocess(regex));

    automata *bruh = createAutomata(a);
    
#if img
    cout << "Compiled with img" << endl;
#endif

    cout <<"start\t"<< bruh->start
    <<"\taccept\t" << bruh->accept << endl;
    for (auto& trans:bruh->gamma)
        cout << trans.current << "\t==>\t" 
        << trans.character << "\t==>\t" 
        << trans.next << endl;

    } catch(const invalid_argument& e)
    {
        cerr << e.what() << endl;
    }
    return 0;
}


string nonconcatwith = ")|*?+";
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

void postorderprint(node* root)
{
    if (root->leftson)
        postorderprint(root->leftson);
    if (root->rightson)
        postorderprint(root->rightson);
    cout << root->character << endl;
}

automata* createAutomata(node* root)
{
    automata* left   = 0;
    automata* right  = 0;
    if (root->leftson)
        left = createAutomata(root->leftson);
    if (root->rightson)
        right = createAutomata(root->rightson);
    automata *out = root->impl(root->character, left, right);

    if (left)
        delete left;
    
    if (right)
        delete right;

    return out;
}
