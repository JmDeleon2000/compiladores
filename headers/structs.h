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

constexpr bool operator==(const transition& a, const transition& b);


struct automata
{
    state start;
    std::vector<transition> gamma;
    state accept;
};

typedef automata* (*definition) (std::string, automata*, automata*);

struct node
{
    int regexPosition;
    definition impl;
    std::string character;
    node* leftson;
    node* rightson;
};

struct op_thompson
{
    definition impl;
    int priority = 100000;
    const char* literal;
    bool unary = false;
    int regexPosition = -1;
};