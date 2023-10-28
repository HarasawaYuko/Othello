#include "AiAlphaBeta.h"

int alphaBetaScore(const State&, int, const int, const int);

uint64_t alphaBetaAction(State state, const int depth) {
	//���@�肪��ȉ��Ȃ瑦��
	auto legalActions = state.legalAction();
	if (legalActions.size() <= 1) {
		return legalActions[0];
	}
	//�A���t�@�x�[�^�@�ɂ��T��
	int bestIndex = -1;//�]���l�̗ǂ�����̃C���f�b�N�X
	int alpha = -1000000; //�őP�̕]���l
	int beta = 1000000;
	for (int i = 0; i < legalActions.size(); i++) {
		//���i�߂���Ԃ��쐬
		State next_state = state;
		next_state.advance(legalActions[i]);
		//����̃X�R�A�Ȃ̂Ŕ��]
		int score = -alphaBetaScore(next_state, -beta, -alpha, depth);
		if (score > alpha) {
			bestIndex = i;
			alpha = score;
		}
	}
	return legalActions[bestIndex];
}

int alphaBetaScore(const State& state, int alpha, const int beta, const int depth) {
	//�Q�[���I�����T���I���[�x�ł͂��̂܂ܕ]���l��Ԃ�
	if (state.isDone() || depth == 0) {
		return state.getEvaluation();
	}
	//���̎�̕]���l�𓾂�
	auto legalactions = state.legalAction();
	//�S�Ă̍��@��ɂ���
	for (const auto action : legalactions) {
		//�����Đi�߂���Ԃ��쐬
		State next_state = state;
		next_state.advance(action);
		//���̏�Ԃ̃X�R�A���擾(���肾���王�_�𔽓])
		int score = -alphaBetaScore(next_state, -beta, -alpha, depth - 1);
		if (score > alpha)
		{
			alpha = score;
		}
		if (alpha >= beta) {
			return alpha;
		}
	}
	return alpha;
}