#include <algorithm>
#include <unordered_map>
#include "operators.h"
#include <iostream>

using namespace std;


state counter = 0;
state getNewID()
{
    return counter++;
}

automata* orop (string character, automata* a, automata* b)
{
    automata* out = new automata();
    out->start = a->start;
    
    out->gamma = a->gamma;

    out->accept = a->accept;

    for (auto& trans:b->gamma)
    {
        transition insertion = trans;
        if (trans.current == b->start)
            insertion.current = a->start;
        if (trans.next == b->accept)
            insertion.next = a->accept;
        out->gamma.push_back(insertion);
    }

    return out;
}


automata* kleene (string character, automata* a, automata* b)
{
    automata* out = new automata();
    
    out->accept = a->accept;
    out->start = a->accept;
    out->gamma = std::vector<transition>();

    for (auto& trans:a->gamma)
    {
        transition insertion = trans;
        if (trans.current == a->start)
            insertion.current = a->accept;
        if (trans.next == a->start)
            insertion.next = a->accept;
        out->gamma.push_back(insertion);
    }

    return out;
}


automata* concat (string character, automata* a, automata* b)
{
    automata* out = new automata();
    
    out->gamma = vector<transition>();


    unordered_map<long, long> replaceTable;
    unordered_map<long, long>::iterator it;

    replaceTable.insert({a->accept, getNewID()});
    replaceTable.insert({b->start, replaceTable[a->accept]});

    for (auto& trans: a->gamma)
    {
        transition new_trans = trans;

        it = replaceTable.find(trans.current);
        if (it == replaceTable.end())
            replaceTable.insert({trans.current, trans.current});

        it = replaceTable.find(trans.next);
        if (it == replaceTable.end())
            replaceTable.insert({trans.next, trans.next});

        new_trans.current = replaceTable[trans.current];
        new_trans.next = replaceTable[trans.next];
        out->gamma.push_back(new_trans);
    }

    for (auto& trans: b->gamma)
    {
        transition new_trans = trans;

        it = replaceTable.find(trans.current);
        if (it == replaceTable.end())
            replaceTable.insert({trans.current, trans.current});

        it = replaceTable.find(trans.next);
        if (it == replaceTable.end())
            replaceTable.insert({trans.next, trans.next});

        new_trans.current = replaceTable[trans.current];
        new_trans.next = replaceTable[trans.next];
        out->gamma.push_back(new_trans);
    }

    out->start = replaceTable[a->start];
    out->accept = replaceTable[b->accept];

    return out;
}

automata* charfunc (string character, automata* a, automata* b)
{
    automata* out = new automata();
    out->start = getNewID();
    out->accept = getNewID();
    out->gamma = std::vector<transition>();
    transition trans;
    trans.current = out->start;
    trans.next = out->accept;
    trans.character = character[0];
    
    out->gamma.push_back(trans);
    return out;
}

automata* questionMark (string character, automata* a, automata* b)
{
    automata* out = new automata();
    out->start = a->start;
    
    out->gamma = a->gamma;

    out->accept = a->accept;

    
    transition epsilon;
    epsilon.character = '\0';
    epsilon.current = out->start;
    epsilon.next = out->accept;

    out->gamma.push_back(epsilon);

    return out;
}


automata* plusfunc (std::string character, automata* a, automata* b)
{
    
    automata* a_copy = new automata();
    
    a_copy->gamma = vector<transition>();

    unordered_map<long, long> replaceTable;
    unordered_map<long, long>::iterator it;

    for (auto& trans: a->gamma)
    {
        transition new_trans = trans;

        it = replaceTable.find(trans.current);
        if (it == replaceTable.end())
            replaceTable.insert({trans.current, getNewID()});

        it = replaceTable.find(trans.next);
        if (it == replaceTable.end())
            replaceTable.insert({trans.next, getNewID()});

        new_trans.current = replaceTable[trans.current];
        new_trans.next = replaceTable[trans.next];
        a_copy->gamma.push_back(new_trans);
    }

    a_copy->start = replaceTable[a->start];
    a_copy->accept = replaceTable[a->accept];


    automata* k = kleene("*", a_copy, 0);
    automata* out = concat(".", a, k);

    delete k;
    delete a_copy;
    return out;
}
