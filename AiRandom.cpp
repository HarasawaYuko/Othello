#include "AiRandom.h"

static std::mt19937 random_for_action(1);

uint64_t randomActionOthello(const State& state) {
	auto legalActions = state.legalAction();
	return legalActions[random_for_action()%legalActions.size()];
}


//�����e�J�����ؒT���p�v���C�A�E�g
double playout(State *state) {
	switch (state->getWinningStatus()) {
	case WIN:
		return 1.0;
	case LOSE:
		return 0.0;
	case DRAW:
		return 0.5;
	default:
		state->advance(randomActionOthello(*state));
		//���_���t�Ȃ̂ŃX�R�A���t�]
		return 1 - playout(state);
	}
}