#include "OthelloAIMcts.h"

namespace {
	const int serchTime = 10000;
}

Coord mctsActionOthello(const OthelloState &state , const int playoutNum) {
	//	合法手が一つだけの時はそのまま
	if (state.legalAction().size() == 1) {
		return state.legalAction()[0];
	}
	
	othelloNode root = othelloNode(state);
	//選択対象の合法手を列挙
	root.expand();
	//評価
	for (int i = 0; i < playoutNum; i++) {
		root.evaluate();
	}
	auto legal_actions = state.legalAction();
	//最も選択回数が多いノードを選択
	int best_num = -1;
	int best_index = -1;
	for (int i = 0; i < legal_actions.size(); i++) {
		int n = root.child_nodes[i].n;
		if (n>best_num) {
			best_num = n;
			best_index = i;
		}
	}
	return legal_actions[best_index];
}