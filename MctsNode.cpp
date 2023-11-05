#include "MctsNode.h"
#include "AiRandom.h"

static const int EXPAND_THRESHOLD = 10;//ノード展開の閾値
static const double C = 1.0;//ノード選択用定数

MctsNode::MctsNode(const State& state)
	:state(state), w(0), n(0)
{
	this->child_nodes.clear();
}

//ノードを評価する
double MctsNode::evaluate() {
	//最終局面か確認
	if (state.isDone()) {
		double value = 0.5;
		switch (state.getWinningStatus()) {
		case WIN:
			value = 1.0;
			break;
		case LOSE:
			value = 0.0;
			break;
		default:
			break;
		}
		this->w += value;
		n++;
		return value;
	}
	//子ノードがないとき
	if (this->child_nodes.empty()) {
		//プレイアウトする
		State state_tmp = this->state;
		double value = playout(&state_tmp);
		this->w += value;
		n++;

		//規定回数なら展開
		if (this->n == EXPAND_THRESHOLD) {
			this->expand();
		}
		return value;
	}
	//子ノードが存在する場合
	else {
		//次のノードを選択し、評価値を加算
		//敵のノードなので視点反転
		double value = 1. - this->nextChildNode().evaluate();
		this->w += value;
		this->n++;
		return value;
	}
}

//ノードの展開
void MctsNode::expand() {
	//合法手を取得
	auto legalActions = this->state.legalAction();
	for (const auto action:legalActions) {
		//子ノードを追加
		this->child_nodes.emplace_back(this->state);
		this->child_nodes.back().state.advance(action);
	}
	
}

//UCB1によって次に評価するノードを決定
MctsNode& MctsNode::nextChildNode() {
	//一度も選択されていないノードがあれば即決
	for (auto& child_node : this->child_nodes) {
		if (child_node.n == 0) {
			return child_node;
		}
	}
	double t = 0; //総探索回数
	for (const auto& child_node : this->child_nodes) {
		t += child_node.n;
	}

	//UCBの計算と選択
	double best_value = -100000;
	int best_index = -1;
	for (int i = 0; i < this->child_nodes.size(); i++) {
		const auto& child_node = this->child_nodes[i];
		//子ノードは相手視点なので勝率は逆転
		double ucb1 = 1. - child_node.w / (double)child_node.n + (double)C * std::sqrt(2. * std::log(t) / (double)child_node.n);
		if (ucb1 > best_value) {
			best_index = i;
			best_value = ucb1;
		}
	}
	return this->child_nodes[best_index];
}