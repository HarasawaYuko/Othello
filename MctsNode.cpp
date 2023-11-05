#include "MctsNode.h"
#include "AiRandom.h"

static const int EXPAND_THRESHOLD = 10;//�m�[�h�W�J��臒l
static const double C = 1.0;//�m�[�h�I��p�萔

MctsNode::MctsNode(const State& state)
	:state(state), w(0), n(0)
{
	this->child_nodes.clear();
}

//�m�[�h��]������
double MctsNode::evaluate() {
	//�ŏI�ǖʂ��m�F
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
	//�q�m�[�h���Ȃ��Ƃ�
	if (this->child_nodes.empty()) {
		//�v���C�A�E�g����
		State state_tmp = this->state;
		double value = playout(&state_tmp);
		this->w += value;
		n++;

		//�K��񐔂Ȃ�W�J
		if (this->n == EXPAND_THRESHOLD) {
			this->expand();
		}
		return value;
	}
	//�q�m�[�h�����݂���ꍇ
	else {
		//���̃m�[�h��I�����A�]���l�����Z
		//�G�̃m�[�h�Ȃ̂Ŏ��_���]
		double value = 1. - this->nextChildNode().evaluate();
		this->w += value;
		this->n++;
		return value;
	}
}

//�m�[�h�̓W�J
void MctsNode::expand() {
	//���@����擾
	auto legalActions = this->state.legalAction();
	for (const auto action:legalActions) {
		//�q�m�[�h��ǉ�
		this->child_nodes.emplace_back(this->state);
		this->child_nodes.back().state.advance(action);
	}
	
}

//UCB1�ɂ���Ď��ɕ]������m�[�h������
MctsNode& MctsNode::nextChildNode() {
	//��x���I������Ă��Ȃ��m�[�h������Α���
	for (auto& child_node : this->child_nodes) {
		if (child_node.n == 0) {
			return child_node;
		}
	}
	double t = 0; //���T����
	for (const auto& child_node : this->child_nodes) {
		t += child_node.n;
	}

	//UCB�̌v�Z�ƑI��
	double best_value = -100000;
	int best_index = -1;
	for (int i = 0; i < this->child_nodes.size(); i++) {
		const auto& child_node = this->child_nodes[i];
		//�q�m�[�h�͑��莋�_�Ȃ̂ŏ����͋t�]
		double ucb1 = 1. - child_node.w / (double)child_node.n + (double)C * std::sqrt(2. * std::log(t) / (double)child_node.n);
		if (ucb1 > best_value) {
			best_index = i;
			best_value = ucb1;
		}
	}
	return this->child_nodes[best_index];
}