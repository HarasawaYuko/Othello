#include "AiAlphaBeta.h"

int alphaBetaScore(const State&, int, const int, const int);

uint64_t alphaBetaAction(State state, const int depth) {
	//合法手が一つ以下なら即決
	auto legalActions = state.legalAction();
	if (legalActions.size() <= 1) {
		return legalActions[0];
	}
	//アルファベータ法による探索
	int bestIndex = -1;//評価値の良い着手のインデックス
	int alpha = -1000000; //最善の評価値
	int beta = 1000000;
	for (int i = 0; i < legalActions.size(); i++) {
		//一手進めた状態を作成
		State next_state = state;
		next_state.advance(legalActions[i]);
		//相手のスコアなので反転
		int score = -alphaBetaScore(next_state, -beta, -alpha, depth);
		if (score > alpha) {
			bestIndex = i;
			alpha = score;
		}
	}
	return legalActions[bestIndex];
}

int alphaBetaScore(const State& state, int alpha, const int beta, const int depth) {
	//ゲーム終了か探索終了深度ではそのまま評価値を返す
	if (state.isDone() || depth == 0) {
		return state.getEvaluation();
	}
	//次の手の評価値を得る
	auto legalactions = state.legalAction();
	//全ての合法手について
	for (const auto action : legalactions) {
		//いって進めた状態を作成
		State next_state = state;
		next_state.advance(action);
		//次の状態のスコアを取得(相手だから視点を反転)
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