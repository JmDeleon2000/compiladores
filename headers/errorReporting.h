#pragma once
#include "structs.h"
#include <string>

void MissingOperand     (op_thompson tempOP, std::string regex);
void UnbalancedBracket  (op_thompson tempOP, std::string regex);
void MissingOperand     (op_direct tempOP, std::string regex);
void UnbalancedBracket  (op_direct tempOP, std::string regex);