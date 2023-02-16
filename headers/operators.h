#pragma once
#include "structs.h"
#include <string>

automata* kleene        (std::string character, automata* a, automata* b);
automata* orop          (std::string character, automata* a, automata* b);
automata* questionMark  (std::string character, automata* a, automata* b);
automata* concat        (std::string character, automata* a, automata* b);
automata* charfunc      (std::string character, automata* a, automata* b);
