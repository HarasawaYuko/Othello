#pragma once
#include "include.h"
#include "AiRandom.h"
#include "State.h"

class MctsNode {
private:
	State state;//盤面状態
	
public:
	std::vector<MctsNode> child_nodes;//子ノード
	int n;//探索回数
	double w;//累計価値

	MctsNode(const State &);
	double evaluate();
	void expand();
	MctsNode& nextChildNode();
};