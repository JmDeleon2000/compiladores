#pragma once
#include <vector>

struct state
{
    long int id;
};

struct automata
{
    char* alphabet;
    state start;
    std::vector<state> states;
    std::vector<state> accept;
};

typedef automata (*definition) (char*, void*, void*);

struct node
{
    definition impl ;
    char* lexema    ;
    node* leftson   ;
    node* rightson  ;
};

