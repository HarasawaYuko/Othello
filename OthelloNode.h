#pragma once
#include "include.h"
#include "OthelloState.h"
#include "OhtelloAIRandom.h"

class othelloNode {
private:
	OthelloState state;
	
public:
	std::vector<othelloNode> child_nodes;
	int n;
	double w;

	othelloNode(const OthelloState &);
	double evaluate();
	void expand();
	othelloNode& nextChildNode();
};