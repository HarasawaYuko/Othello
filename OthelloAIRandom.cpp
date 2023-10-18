#include "OhtelloAIRandom.h"

namespace {
	std::mt19937 random_for_action(1);
}

Coord randomActionOthello(const OthelloState& state) {
	auto legalActions = state.legalAction();
	return legalActions[random_for_action()%legalActions.size()];
}


//探索用プレイアウト
double playout(OthelloState *state) {
	switch (state->getWinningStatus()) {
	case WIN:
		return 1.0;
	case LOSE:
		return 0.0;
	case DRAW:
		return 0.5;
	default:
		state->advance(randomActionOthello(*state));
		return 1 - playout(state);
	}
}