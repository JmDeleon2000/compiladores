#include <iostream>
#include "shunting-yard.h"
#include "structs.h"
#include "operators.h"
#include <string>
#include <fstream>
#include <cstring>

using namespace std;


string header = "digraph finite_state_machine {\nfontname=%f\nnode [fontname=%f]\nedge [fontname=%f]\nrankdir=LR;\nnode [shape = doublecircle]; %a;\nnode [shape = circle];\n";

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

    //cout << "Input:\t\t" << regex.c_str() << "\n";
    //cout << "Preprocessor:\t" << preprocess(regex) << endl;

    try{
    node* a  = djkstra(preprocess(regex));

    automata *bruh = createAutomata(a);
    

    size_t pos;
    while ((pos = header.find("%f")) != std::string::npos) 
        header.replace(pos, 2, "\"Helvetica,Arial,sans-serif\"");

    
    while ((pos = header.find("%a")) != std::string::npos) 
        header.replace(pos, 2, to_string(bruh->accept));

    

    string out = header;
    string nodeformat = "%d-> %d [label = \"%c\"];\n";
    string insertion;


    char buff[100];

    for (auto& trans:bruh->gamma)
    {
        memset(buff, 0, sizeof(buff));
        sprintf(buff, nodeformat.c_str(), trans.current, trans.next, trans.character);
        out+= buff;
    }

    out+="}";

    cout << out << endl;
    
    //cout <<"start\t"<< bruh->start
    //<<"\taccept\t" << bruh->accept << endl;
    //for (auto& trans:bruh->gamma)
    //    cout << trans.current << "\t==>\t" 
    //    << trans.character << "\t==>\t" 
    //    << trans.next << endl;

    } catch(const invalid_argument& e)
    {
        cerr << e.what() << endl;
    }
    return 0;
}



string nonconcatwith = ")|*?+";
string nonconcat = "(|";

string preprocess(string regex)
{
    string out = "";
    int i = 0;
    
    size_t pos;
    while ((pos = regex.find("ε")) != std::string::npos) 
        regex.replace(pos, 1, "\0");
    
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
