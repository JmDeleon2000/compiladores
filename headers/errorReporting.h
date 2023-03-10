#pragma once
#include "structs.h"
#include <string>

void MissingOperand     (op_thompson tempOP, std::string regex);
void UnbalancedBracket  (op_thompson tempOP, std::string regex);