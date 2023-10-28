#pragma once
#include "include.h"
#include "AiRandom.h"
#include "State.h"

class MctsNode {
private:
	State state;//�Ֆʏ��
	
public:
	std::vector<MctsNode> child_nodes;//�q�m�[�h
	int n;//�T����
	double w;//�݌v���l

	MctsNode(const State &);
	double evaluate();
	void expand();
	MctsNode& nextChildNode();
};