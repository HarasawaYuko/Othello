#pragma once
#include "include.h"
#include "OthelloState.h"
#include "AiRandom.h"
#include "State.h"

class MctsNode {
private:
	State state;
	
public:
	std::vector<MctsNode> child_nodes;
	int n;
	double w;

	MctsNode(const State &);
	double evaluate();
	void expand();
	MctsNode& nextChildNode();
};