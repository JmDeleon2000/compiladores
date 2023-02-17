#include "shunting-yard.h"
#include "operators.h"
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

void init() ;
struct op
{
    definition impl;
    int priority = 100000;
    const char* literal;
    bool unary = false;
};

#define opcount 7

op ops[opcount];
bool initialized = false;

bool getOP(char* a, op* out)
{
    int j;
    for (int i = 0; i < opcount; i++)
    {
        j = 0;
        while (ops[i].literal[j] == a[j]) //mientras sean iguales
        {
            j++;

            out->impl = ops[i].impl;
            out->literal = ops[i].literal;
            out->priority = ops[i].priority;
            out->unary = ops[i].unary;
            return true;
        }
    }
    return false;
}

inline node* doTree(op, vector<op>*, vector<node*>*);


//WIP unarios y brackets
node* djkstra(string regex)
{
    if (!initialized) init();
    
    vector<op>      opstack;
    vector<node*>   nodestack;

    op* currentOP = new op();
    op tempOP;
    bool foundOpen;
    for (auto& ch:regex)
    {
        if (getOP(&ch, currentOP))
        {
#if logYard
            cout << "Current OP:\t" << currentOP->literal 
                << "\t" << currentOP->priority << "\t" 
                << (bool)currentOP->unary << endl;
#endif
            switch (currentOP->priority)
            {
                case -1:
                    opstack.push_back(*currentOP);
                break;
                case -2:
                    foundOpen = false; 
                while (!foundOpen &&
                    !opstack.empty() && 
                    opstack.back().priority >= currentOP->priority)
                    {
#if logYard
                        cout <<"Poping bracket:\t";
                        for (auto& bruh:opstack)
                            cout << bruh.literal << "\t";
                        cout << endl;
#endif
                        if (opstack.empty())
                            throw invalid_argument("Error léxico");
                        tempOP = opstack.back();
                        opstack.pop_back();
                        
                        node* a;
                        node* b;

                        if (nodestack.empty())
                            throw invalid_argument("Error léxico");
                        b = nodestack.back();
                        nodestack.pop_back();

                        node* father; 

                        if (tempOP.priority == -1)
                            father = b;
                        else if (tempOP.unary)
                        {
                            father = new node();

                            father->character = tempOP.literal;
                            father->impl = tempOP.impl;
                            father->leftson = b;
                        }
                        else 
                        {
                            father = new node();
                            if (nodestack.empty())
                                throw invalid_argument("Error léxico");
                            a = nodestack.back();
                            nodestack.pop_back();

                            father->character = tempOP.literal;
                            father->impl = tempOP.impl;
                            father->leftson = a;
                            father->rightson = b;
                        }
                        nodestack.push_back(father);
                    }
                break;
                default:
                    while (!opstack.empty() && //no está vacío
                        opstack.back().priority > 0 && //no es un paréntesis que abre
                        opstack.back().priority >= currentOP->priority) //el actual tiene menor presedencia
                        {
#if logYard
                            cout <<"Poping priority:\t";
                            for (auto& bruh:opstack)
                                cout << bruh.literal << "\t";
                            cout << endl;
#endif
                            if (opstack.empty())
                                throw invalid_argument("Error léxico");
                            tempOP = opstack.back();
                            opstack.pop_back();

                            node* father = doTree(tempOP, &opstack, &nodestack);
                            nodestack.push_back(father);
                        }
                    opstack.push_back(*currentOP);
                break;
            }
        }
        else
        {
#if logYard
            cout << "Litnode:\t" << ch << endl;
#endif
            node* litnode = new node();
            litnode->character = ch;
            litnode->impl = charfunc;
            nodestack.push_back(litnode);
        }
    }

    while (!opstack.empty())
    {
        tempOP = opstack.back();
        opstack.pop_back();

        node* father = doTree(tempOP, &opstack, &nodestack);
        nodestack.push_back(father);
    }
    
    return nodestack.back();
}

// operators        = "*?.|";
// openBrackets     = "(";
// ClosingBrackets  = ")";
void init() 
{
    ops[0].literal  = "|";
    ops[0].priority = 1;
    ops[0].impl = orop;

    ops[1].literal  = ".";
    ops[1].priority = 3;
    ops[1].impl = concat;

    ops[2].literal  = "?";
    ops[2].priority = 4;
    ops[2].impl = questionMark;
    ops[2].unary = true;

    ops[3].literal  = "*";
    ops[3].priority = 4;
    ops[3].impl = kleene;
    ops[3].unary = true;

    ops[4].literal  = "(";
    ops[4].priority = -1;
    ops[4].impl = 0;

    ops[5].literal  = ")";
    ops[5].priority = -2;
    ops[5].impl = 0;

    ops[6].literal  = "+";
    ops[6].priority =  4;
    ops[6].impl = plusfunc;
    ops[6].unary = true;

    initialized = true;
}

inline node* doTree(op tempOP, vector<op>* opstack, vector<node*>* nodestack)
{
    node* a = nullptr;
    node* b = nullptr;

    if (nodestack->empty())
        throw invalid_argument("Error léxico");
    b = nodestack->back();
    nodestack->pop_back();

    node *father;
    if (tempOP.unary)
    {
        father = new node();
    
        father->character = tempOP.literal;
        father->impl = tempOP.impl;
        father->leftson = b;
    }
    else 
    {
        father = new node();
        if (nodestack->empty())
            throw invalid_argument("Error léxico");
        a = nodestack->back();
        nodestack->pop_back();

        father->character = tempOP.literal;
        father->impl = tempOP.impl;
        father->leftson = a;
        father->rightson = b;
    }
    return father;
}