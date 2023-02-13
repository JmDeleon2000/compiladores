#pragma once
#include "structs.h"

automata* kleene (char* character, automata* a, automata* b);
automata* orop (char* character, automata* a, automata* b);
automata* concat (char* character, automata* a, automata* b);
automata* charfunc (char* character, automata* a, automata* b);
