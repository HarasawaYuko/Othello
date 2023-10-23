#pragma once
#include "OthelloState.h"
#include "BitState.h"

uint64_t randomActionOthello(const BitState &);
double playout(BitState*);