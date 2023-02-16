
#include "operators.h"

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
    out->start = a->start;
    
    out->gamma = a->gamma;

    out->accept = b->accept;

    for (auto& trans:b->gamma)
    {
        transition insertion = trans;
        if (trans.current == b->start)
            insertion.current = a->accept;
        if (trans.next == b->start)
            insertion.next = a->accept;
        out->gamma.push_back(insertion);
    }

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
    //TODO
}