#include "AiAlphaBeta.h"

int alphaBetaScore(const BitState&, int, const int, const int);

uint64_t alphaBetaAction(BitState state, const int depth) {
	//���@�肪��ȉ��Ȃ瑦��
	auto legalActions = state.legalAction();
	if (legalActions.size() <= 1) {
		return legalActions[0];
	}
	int bestIndex = -1;//�]���l�̗ǂ�����̃C���f�b�N�X
	int alpha = -1000000; //�őP�̕]���l
	int beta = 1000000;
	for (int i = 0; i < legalActions.size(); i++) {
		BitState next_state = state;
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

int alphaBetaScore(const BitState& state, int alpha, const int beta, const int depth) {
	//�Q�[���I�����T���I���[�x�ł͂��̂܂ܕԂ�
	if (state.isDone() || depth == 0) {
		return state.getEvaluation();
	}
	//���̎��T������
	auto legalactions = state.legalAction();
	for (const auto action : legalactions) {
		//�R�s�[�R���X�g���N�^
		BitState next_state = state;
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