#pragma once
#include "include.h"
#include "OthelloState.h"
#include "OhtelloAIRandom.h"
#include "BitState.h"

class othelloNode {
private:
	BitState state;
	
public:
	std::vector<othelloNode> child_nodes;
	int n;
	double w;

	othelloNode(const BitState &);
	double evaluate();
	void expand();
	othelloNode& nextChildNode();
};