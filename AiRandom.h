#pragma once
#include "State.h"

//ランダムに着手を決定するAI
uint64_t randomActionOthello(const State &);

//モンテカルロ木探索用　ランダムプレイアウト
double playout(State*);