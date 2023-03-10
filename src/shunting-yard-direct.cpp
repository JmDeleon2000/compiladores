#include "shunting-yard.h"
#include "direct_funcs.h"
#include "errorReporting.h"
#include <vector>
#include <iostream>


using namespace std;

void init_dir();


#define opcount 7

op_direct ops_direct[opcount];
bool initialized_dir = false;

bool getOP_direct(char* a, op_direct* out)
{
    int j;
    for (int i = 0; i < opcount; i++)
    {
        j = 0;
        while (ops_direct[i].literal[j] == a[j]) //mientras sean iguales
        {
            j++;

            out->nullable = ops_direct[i].nullable;
            out->first_pos = ops_direct[i].first_pos;
            out->last_pos = ops_direct[i].last_pos;
            out->literal = ops_direct[i].literal;
            out->priority = ops_direct[i].priority;
            out->unary = ops_direct[i].unary;
            return true;
        }
    }
    return false;
}

inline node_direct* doTree(op_direct, vector<op_direct>*, vector<node_direct*>*, string);




node_direct* djkstra_direct(string regex)
{
    if (!initialized_dir) init_dir();


    vector<op_direct>      opstack;
    vector<node_direct*>   nodestack;

    op_direct* currentOP = new op_direct();
    op_direct tempOP;
    bool foundOpen;
    char ch = '\0';
    for (int i = 0; i < regex.size(); i++)
    {
        ch = regex[i];
        if (getOP_direct(&ch, currentOP))
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
                        
                        node_direct* a;
                        node_direct* b;

                        if (nodestack.empty())
                            MissingOperand(tempOP, regex);
                        
                        b = nodestack.back();
                        nodestack.pop_back();

                        node_direct* father; 

                        if (tempOP.priority == -1)
                        {
                            father = b;
                            foundOpen = true;
                        }
                        else if (tempOP.unary)
                        {
                            father = new node_direct();

                            father->character = tempOP.literal;
                            father->leftson = b;
                            father->regexPosition = i;
                            father->nullable = tempOP.nullable(father, father->leftson, father->rightson);
                            father->first_pos = tempOP.first_pos(father, father->leftson, father->rightson);
                            father->last_pos = tempOP.last_pos(father, father->leftson, father->rightson);
                        }
                        else 
                        {
                            father = new node_direct();
                            if (nodestack.empty())
                                MissingOperand(tempOP, regex);

                            a = nodestack.back();
                            nodestack.pop_back();

                            father->character = tempOP.literal;
                            father->leftson = a;
                            father->rightson = b;
                            father->regexPosition = i;
                            father->nullable = tempOP.nullable(father, father->leftson, father->rightson);
                            father->first_pos = tempOP.first_pos(father, father->leftson, father->rightson);
                            father->last_pos = tempOP.last_pos(father, father->leftson, father->rightson);
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
                    while (!opstack.empty() && //no est?? vac??o
                        opstack.back().priority > 0 && //no es un par??ntesis que abre
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

                            node_direct* father = doTree(tempOP, &opstack, &nodestack, regex);
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
            cout << "leaf_node:\t" << ch << endl;
#endif
            node_direct* leaf_node = new node_direct();
            leaf_node->character = ch;
            leaf_node->nullable = tempOP.nullable(leaf_node, leaf_node->leftson, leaf_node->rightson);
            leaf_node->first_pos = tempOP.first_pos(leaf_node, leaf_node->leftson, leaf_node->rightson);
            leaf_node->last_pos = tempOP.last_pos(leaf_node, leaf_node->leftson, leaf_node->rightson);
            leaf_node->regexPosition = i;
            nodestack.push_back(leaf_node);
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

        node_direct* father = doTree(tempOP, &opstack, &nodestack, regex);
        nodestack.push_back(father);
    }
    
    return nodestack.back();
}

// operators        = "*?.|";
// openBrackets     = "(";
// ClosingBrackets  = ")";
void init_dir() 
{
    ops_direct[0].literal  = "|";
    ops_direct[0].priority = 1;
    ops_direct[0].nullable = or_null;
    ops_direct[0].first_pos = or_first;
    ops_direct[0].last_pos = or_last;

    ops_direct[1].literal  = ".";
    ops_direct[1].priority = 3;
    ops_direct[1].nullable = concat_null;
    ops_direct[1].first_pos = concat_first;
    ops_direct[1].last_pos = concat_last;

    ops_direct[2].literal  = "?";
    ops_direct[2].priority = 4;
    ops_direct[2].nullable = qmark_null;
    ops_direct[2].first_pos = qmark_first;
    ops_direct[2].last_pos = qmark_last;
    ops_direct[2].unary = true;

    ops_direct[3].literal  = "*";
    ops_direct[3].priority = 4;    
    ops_direct[3].nullable = kleene_null;
    ops_direct[3].first_pos = kleene_first;
    ops_direct[3].last_pos = kleene_last;
    ops_direct[3].unary = true;

    ops_direct[4].literal  = "+";
    ops_direct[4].priority =  4;
    ops_direct[4].nullable = positive_null;
    ops_direct[4].first_pos = positive_first;
    ops_direct[4].last_pos = positive_last;
    ops_direct[4].unary = true;

    ops_direct[5].literal  = "(";
    ops_direct[5].priority = -1;
    ops_direct[5].nullable = 0;
    ops_direct[5].first_pos = 0;
    ops_direct[5].last_pos = 0;

    ops_direct[6].literal  = ")";
    ops_direct[6].priority = -2;
    ops_direct[6].nullable = 0;
    ops_direct[6].first_pos = 0;
    ops_direct[6].last_pos = 0;


    initialized_dir = true;
}

inline node_direct* doTree(op_direct tempOP, vector<op_direct>* opstack, vector<node_direct*>* nodestack, string regex)
{
    if (tempOP.literal == "(")
        UnbalancedBracket(tempOP, regex);
    
    node_direct* a = nullptr;
    node_direct* b = nullptr;

    if (nodestack->empty())
        MissingOperand(tempOP, regex);

    b = nodestack->back();
    nodestack->pop_back();


    node_direct* father;
    if (tempOP.unary)
    {
        father = new node_direct();

        father->character = tempOP.literal;
        father->leftson = b;
        father->regexPosition = tempOP.regexPosition;
        father->nullable = tempOP.nullable(father, father->leftson, father->rightson);
        father->first_pos = tempOP.first_pos(father, father->leftson, father->rightson);
        father->last_pos = tempOP.last_pos(father, father->leftson, father->rightson);
    }
    else 
    {
        father = new node_direct();
        if (nodestack->empty())
            MissingOperand(tempOP, regex);

        a = nodestack->back();
        nodestack->pop_back();

        father->character = tempOP.literal;
        father->leftson = a;
        father->rightson = b;
        father->regexPosition = tempOP.regexPosition;
        father->nullable = tempOP.nullable(father, father->leftson, father->rightson);
        father->first_pos = tempOP.first_pos(father, father->leftson, father->rightson);
        father->last_pos = tempOP.last_pos(father, father->leftson, father->rightson);
    }
    return father;
}

