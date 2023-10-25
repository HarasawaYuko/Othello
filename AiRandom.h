#pragma once
#include "OthelloState.h"
#include "State.h"

uint64_t randomActionOthello(const State &);
double playout(State*);