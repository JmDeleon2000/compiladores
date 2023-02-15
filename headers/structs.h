#pragma once
#include <vector>
#include <string>

typedef long state;

state getNewID();

struct transition
{
    state current;
    char character;
    state next;
};

struct automata
{
    state start;
    std::vector<transition> gamma;
    state accept;
};



typedef automata* (*definition) (char*, automata*, automata*);

struct node
{
    definition impl;
    std::string character;
    node* leftson;
    node* rightson;
};

