#include <iostream>
#include "shunting-yard.h"
#include "structs.h"
#include "operators.h"
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;


string header = "digraph finite_state_machine {\nfontname=%f\nnode [fontname=%f]\nedge [fontname=%f]\nrankdir=LR;\nnode [shape = doublecircle]; %a;\nnode [shape = point ]; qi\nnode [shape = circle];\n";

string preprocess(string);

void postorderprint(node*);
void postorderdirectprint(node_direct*);
automata* createAutomata(node*);


int main(int argc, char* argv[])
{
    string filePath = "regex/foo";
    if (argc > 1)
        filePath = argv[0];

    ifstream file = ifstream(filePath);
    string regex;
    getline(file, regex);

    for (int i = 0; i < 33; i++)
        regex.erase(std::remove(regex.begin(), regex.end(), (char)i), regex.end());

    if (regex.size() == 0)
        {
            cerr << "No se ingresó ninguna expresión regular" << endl;
            return -1;
        }
#if logYard
    cout << "Input:\t\t" << regex.c_str() << "\n";
    cout << "Preprocessor:\t" << preprocess(regex) << endl;
#endif

    try
    {
    node* finalTree  = djkstra_thompson(preprocess(regex));
#if logYard
    postorderprint(finalTree);
#endif
    automata *finalAutomata = createAutomata(finalTree);
    
    size_t pos;
    while ((pos = header.find("%f")) != std::string::npos) 
        header.replace(pos, 2, "\"Helvetica,Arial,sans-serif\"");

    
    while ((pos = header.find("%a")) != std::string::npos) 
        header.replace(pos, 2, to_string(finalAutomata->accept));

    

    string out = header;
    string nodeformat = "%d-> %d [label = \"%c\"];\n";
    string insertion;

    for (auto& trans:finalAutomata->gamma)
    {
        insertion = nodeformat;
        pos = insertion.find("%d");
        insertion.replace(pos, 2, to_string(trans.current));
        pos = insertion.find("%d");
        insertion.replace(pos, 2, to_string(trans.next));
        pos = insertion.find("%c");
        if (trans.character)
            insertion.replace(pos, 2, &trans.character);
        else
            insertion.replace(pos, 2, "ε");
        out+= insertion;
    }

    string startNodeMarker = "qi -> %s;";
    while ((pos = startNodeMarker.find("%s")) != std::string::npos) 
        startNodeMarker.replace(pos, 2, to_string(finalAutomata->start));
    
    out += startNodeMarker;

    out+="}";

#if logYard

#else
    cout << out << endl;
#endif
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
        if (i+1 < regex.size() &&
            regex[i] == '(' && 
            regex[i+1] == ')')
                out+= '\0';
            

        
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
    cout << root->character;
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


void postorderdirectprint(node_direct* root)
{
    if (root->leftson)
        postorderdirectprint(root->leftson);
    if (root->rightson)
        postorderdirectprint(root->rightson);
    cout << "{";
    for (auto& ele:root->first_pos)
        cout << ele << " ";
    cout << "} \t";
    cout << root->character << " \t{";
    for (auto& ele:root->last_pos)
        cout << ele << " ";
    cout << "} \n";
}