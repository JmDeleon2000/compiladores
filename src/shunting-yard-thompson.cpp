#include "shunting-yard.h"
#include "operators.h"
#include "errorReporting.h"
#include <vector>
#include <iostream>


using namespace std;

void init() ;


#define opcount 7

op_thompson ops[opcount];
bool initialized = false;

bool getOP_thompson(char* a, op_thompson* out)
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

inline node* doTree(op_thompson, vector<op_thompson>*, vector<node*>*, string);




node* djkstra_thompson(string regex)
{
    if (!initialized) init();


    vector<op_thompson>      opstack;
    vector<node*>   nodestack;

    op_thompson* currentOP = new op_thompson();
    op_thompson tempOP;
    bool foundOpen;
    char ch = '\0';
    for (int i = 0; i < regex.size(); i++)
    {
        ch = regex[i];
        if (getOP_thompson(&ch, currentOP))
        {
#if logYard
            cout << "Current OP:\t" << currentOP->literal 
                << "\t" << currentOP->priority << "\t" 
                << (bool)currentOP->unary << endl;
#endif
            switch (currentOP->priority)
            {
                case -1:
                    currentOP->regexPosition = i;
                    opstack.push_back(*currentOP);
                    currentOP->regexPosition = -1;
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
                            MissingOperand(tempOP, regex);
                        tempOP = opstack.back();
                        opstack.pop_back();
                        
                        node* a;
                        node* b;

                        if (nodestack.empty())
                            MissingOperand(tempOP, regex);
                        
                        b = nodestack.back();
                        nodestack.pop_back();

                        node* father; 

                        if (tempOP.priority == -1)
                        {
                            father = b;
                            foundOpen = true;
                        }
                        else if (tempOP.unary)
                        {
                            father = new node();

                            father->character = tempOP.literal;
                            father->impl = tempOP.impl;
                            father->leftson = b;
                            father->regexPosition = i;
                        }
                        else 
                        {
                            father = new node();
                            if (nodestack.empty())
                                MissingOperand(tempOP, regex);

                            a = nodestack.back();
                            nodestack.pop_back();

                            father->character = tempOP.literal;
                            father->impl = tempOP.impl;
                            father->leftson = a;
                            father->rightson = b;
                            father->regexPosition = i;
                        }
                        nodestack.push_back(father);
                        
                    }
                    if (!foundOpen)
                    {
                        currentOP->regexPosition = i;
                        UnbalancedBracket(*currentOP, regex);
                    }
                break;
                default:
                    while (!opstack.empty() && //no está vacío
                        opstack.back().priority > 0 && //no es un paréntesis que abre
                        opstack.back().priority >= currentOP->priority) //el actual tiene menor presedencia
                        {
#if logYard
                            cout <<"Poping priority:\t"
                            << currentOP->literal << "\t"
                            << currentOP->priority << "\tstack:\t";

                            for (auto& bruh: opstack)
                                cout << bruh.literal << "\t";
                            cout << endl;

#endif
                            if (opstack.empty())
                                MissingOperand(tempOP, regex);
                            tempOP = opstack.back();
                            opstack.pop_back();

                            node* father = doTree(tempOP, &opstack, &nodestack, regex);
                            nodestack.push_back(father);
                        }
                    currentOP->regexPosition = i;
                    opstack.push_back(*currentOP);
                    currentOP->regexPosition = -1;

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
            litnode->regexPosition = i;
            nodestack.push_back(litnode);
        }
    }

    while (!opstack.empty())
    {
#if logYard
        cout <<"Final pop:\t";
        for (auto& bruh: opstack)
            cout << bruh.literal << "\t";
        cout << endl;
#endif
        tempOP = opstack.back();
        opstack.pop_back();

        node* father = doTree(tempOP, &opstack, &nodestack, regex);
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

    ops[4].literal  = "+";
    ops[4].priority =  4;
    ops[4].impl = plusfunc;
    ops[4].unary = true;

    ops[5].literal  = "(";
    ops[5].priority = -1;
    ops[5].impl = 0;

    ops[6].literal  = ")";
    ops[6].priority = -2;
    ops[6].impl = 0;


    initialized = true;
}

inline node* doTree(op_thompson tempOP, vector<op_thompson>* opstack, vector<node*>* nodestack, string regex)
{
    if (tempOP.literal == "(")
        UnbalancedBracket(tempOP, regex);
    
    node* a = nullptr;
    node* b = nullptr;

    if (nodestack->empty())
        MissingOperand(tempOP, regex);

    b = nodestack->back();
    nodestack->pop_back();


    node *father;
    if (tempOP.unary)
    {
        father = new node();

        father->character = tempOP.literal;
        father->impl = tempOP.impl;
        father->leftson = b;
        father->regexPosition = tempOP.regexPosition;
    }
    else 
    {
        father = new node();
        if (nodestack->empty())
            MissingOperand(tempOP, regex);

        a = nodestack->back();
        nodestack->pop_back();

        father->character = tempOP.literal;
        father->impl = tempOP.impl;
        father->leftson = a;
        father->rightson = b;
        father->regexPosition = tempOP.regexPosition;
    }
    return father;
}

