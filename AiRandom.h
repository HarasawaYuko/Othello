#pragma once
#include "State.h"

//�����_���ɒ�������肷��AI
uint64_t randomActionOthello(const State &);

//�����e�J�����ؒT���p�@�����_���v���C�A�E�g
double playout(State*);