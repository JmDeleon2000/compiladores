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


struct node_direct
{
    int regexPosition;
    bool nullable;
    std::vector<int> first_pos;
    std::vector<int> last_pos;
    std::string character;
    node_direct* leftson;
    node_direct* rightson;
};

typedef std::vector<int> (*dir_func) (node_direct*, node_direct*, node_direct*);
typedef bool (*null_func) (node_direct*, node_direct*, node_direct*);

struct op_direct
{
    null_func nullable;
    dir_func first_pos;
    dir_func last_pos;
    int priority = 100000;
    const char* literal;
    bool unary = false;
    int regexPosition = -1;
};










