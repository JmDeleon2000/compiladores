#include "shunting-yard.h"
#include "operators.h"
#include <vector>

using namespace std;


struct op
{
    definition impl;
    int priority = INT_MAX;
    const char* literal;
    bool unary = false;
};

#define opcount 6

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
            if (ops[i].literal[j] == 0 && a[j] == 0) //si tienen el mismo largo
                {
                    out->impl = ops[i].impl;
                    out->literal = ops[i].literal;
                    out->priority = ops[i].priority;
                    return true;
                }
        }
    }
    return false;
}


//WIP unarios y brackets
node djkstra(string regex)
{
    if (!initialized) init();
    node head;
    vector<op>      opstack;
    vector<node*>   nodestack;

    op* currentOP = new op();
    op tempOP;
    bool foundOpen;
    for (auto& ch:regex)
    {
        if (getOP(&ch, currentOP))
        {
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
                        if (opstack.empty())
                            throw exception("Error léxico");
                        tempOP = opstack.back();
                        opstack.pop_back();
                        
                        node* a;
                        node* b;

                        if (nodestack.empty())
                            throw exception("Error léxico");
                        b = nodestack.back();
                        nodestack.pop_back();

                        node* father = new node();
                        if (tempOP.priority == -1 && tempOP.impl)
                        {
                            foundOpen = true;
                            father->character = tempOP.literal;
                            father->impl = tempOP.impl;
                            father->leftson = b;
                        }
                        else
                        {
                            if (nodestack.empty())
                                throw exception("Error léxico");
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
                            if (opstack.empty())
                                throw exception("Error léxico");
                            tempOP = opstack.back();
                            opstack.pop_back();

                            node* a = nullptr;
                            node* b = nullptr;


                            if (nodestack.empty())
                                throw exception("Error léxico");
                            b = nodestack.back();
                            nodestack.pop_back();
                            if (nodestack.empty())
                                throw exception("Error léxico");
                            a = nodestack.back();
                            nodestack.pop_back();

                            node* father = new node();
                            father->character = tempOP.literal;
                            father->impl = tempOP.impl;
                            father->leftson = a;
                            father->rightson = b;

                            nodestack.push_back(father);
                        }
                    opstack.push_back(*currentOP);
                break;
            }
        }
        else
        {
            node* litnode = new node();
            litnode->character = ch;
            litnode->impl = charfunc;
            nodestack.push_back(litnode);
        }
    }
    
    return head;
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
    ops[1].priority = 2;
    ops[1].impl = concat;

    ops[2].literal  = "?";
    ops[2].priority = 4;
    ops[2].impl = questionMark;
    ops[3].unary = true;

    ops[3].literal  = "*";
    ops[3].priority = 5;
    ops[3].impl = kleene;
    ops[3].unary = true;

    ops[4].literal  = "(";
    ops[4].priority = -1;
    ops[4].impl = 0;

    ops[5].literal  = ")";
    ops[5].priority = -2;
    ops[5].impl = 0;
}