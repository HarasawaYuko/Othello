#include "BitState.h"

/****�\���̓��̒�`*****/

namespace {
	const int vector[8][2] = { {1,1},
								{1,0},
								{1,-1},
								{0,1},
								{0,-1},
								{-1,1},
								{-1,0},
								{-1,-1} };
}

static const int BOARD_EVAL[8][8] =
{
	{40 , -1  , 10 , 5 , 5 , 10 ,  -1 , 40},
	{-1 , -20 ,  1 , 1 , 1 ,  1 , -20 , -1},
	{10 ,   1 ,  1 , 1 , 1 ,  1 ,   1 , 10},
	{ 5 ,   1 ,  1 , 1 , 1 ,  1 ,   1 ,  5},
	{ 5 ,   1 ,  1 , 1 , 1 ,  1 ,   1 ,  5},
	{10 ,   1 ,  1 , 1 , 1 ,  1 ,   1 , 10},
	{-1 , -20 ,  1 , 1 , 1 ,  1 , -20 , -1},
	{40 , -1  , 10 , 5 , 5 , 10 ,  -1 , 40}
};

static const int EVAL[64] =
{
	40 , -1  , 10 , 5 , 5 , 10 ,  -1 , 40,
	-1 , -20 ,  1 , 1 , 1 ,  1 , -20 , -1,
	10 ,   1 ,  1 , 1 , 1 ,  1 ,   1 , 10,
	 5 ,   1 ,  1 , 1 , 1 ,  1 ,   1 ,  5,
	 5 ,   1 ,  1 , 1 , 1 ,  1 ,   1 ,  5,
	10 ,   1 ,  1 , 1 , 1 ,  1 ,   1 , 10,
	-1 , -20 ,  1 , 1 , 1 ,  1 , -20 , -1,
	40 , -1  , 10 , 5 , 5 , 10 ,  -1 , 40
};

//�r�b�g�}�X�N
static const uint64_t ALL_T   = 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111;
static const uint64_t ALL_F   = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
static const uint64_t LOWEST  = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001;
static const uint64_t HIGHEST  = 0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
static const uint64_t TOP_F   = 0b00000000'11111111'11111111'11111111'11111111'11111111'11111111'11111111;
static const uint64_t UNDER_F = 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'00000000;
static const uint64_t RIGHT_F = 0b11111110'11111110'11111110'11111110'11111110'11111110'11111110'11111110;
static const uint64_t LEFT_F  = 0b01111111'01111111'01111111'01111111'01111111'01111111'01111111'01111111;

//�x�N�g��
static const int SHIFT[] = { 1 , 7 , 8 , 9};
//�V�t�g����
static const uint64_t MASK_RIGHT[] = {LEFT_F , RIGHT_F , TOP_F , LEFT_F};
static const uint64_t MASK_LEFT[] = {RIGHT_F , LEFT_F , UNDER_F , RIGHT_F };

void shift(uint64_t* , bool , int , int num = 1);


BitState::BitState() {
	my_board  = 0b00000000'00000000'00000000'00010000'00001000'00000000'00000000'00000000;
	all_board = 0b00000000'00000000'00000000'00011000'00011000'00000000'00000000'00000000;
	canput_board = getLegalBoard();
	turn = BLACK;
}

uint64_t BitState::getLegalBoard() const{
	uint64_t enemy_board = all_board xor my_board;
	uint64_t result = ALL_F;
	uint64_t unput = ~all_board;//�u���ĂȂ��}�X

	uint64_t mayPut = ALL_F;//�u����\��������}�X
	//�E�V�t�g�̃��[�v(�x�N�g������)
	for (int i = 0; i < 4; i++) {
		uint64_t my_copy = my_board;
		uint64_t mayTurn = ALL_F;//�Ԃ�\��������}�X
		int count = 1;
		//�����ׂ̗ɓG�̋���邩
		shift(&my_copy, true, i, count);
		mayTurn = my_copy & enemy_board;
		//�Ԃ�\��������}�X�������
		while (mayTurn != 0) {
			//�V�t�g
			shift(&mayTurn, true, i);
			result = result | (mayTurn & unput);
			//�u����}�X������΂��̉��ɂ͒u���Ȃ�
			mayTurn = ~result & mayTurn;
			//�����̃}�X������΂��̉��ɂ͍s���Ȃ�
			mayTurn = ~my_board & mayTurn;
			count++;
		}
	}
	for (int i = 0; i < 4; i++) {
		uint64_t mayTurn = ALL_F;//�Ԃ�\��������}�X
		uint64_t my_copy = my_board;
		int count = 1;
		//�����ׂ̗ɓG�̋���邩
		shift(&my_copy, false, i, count);
		mayTurn = my_copy & enemy_board;
		//�Ԃ�\��������}�X�������
		while (mayTurn != 0) {
			//�V�t�g
			shift(&mayTurn, false, i);
			result = result | (mayTurn & unput);
			//�u����}�X������΂��̉��ɂ͒u���Ȃ�
			mayTurn = ~result & mayTurn;
			//�����̃}�X������΂��̉��ɂ͍s���Ȃ�
			mayTurn = ~my_board & mayTurn;
			count++;
		}
	}
	return result;
}

//�I�[�o�[���[�h�@�v���C���[���w��ł���
uint64_t BitState::getLegalBoard(piece color) const{
	uint64_t my = my_board;
	uint64_t enemy = all_board xor my;
	uint64_t result = ALL_F;
	uint64_t unput = ~all_board;//�u���ĂȂ��}�X

	if (color != turn) {
		uint64_t tmp = my;
		my = enemy;
		enemy = tmp;
	}

	uint64_t mayPut = ALL_F;//�u����\��������}�X
	//�E�V�t�g�̃��[�v(�x�N�g������)
	for (int i = 0; i < 4; i++) {
		uint64_t my_copy = my;
		uint64_t mayTurn = ALL_F;//�Ԃ�\��������}�X
		int count = 1;
		//�����ׂ̗ɓG�̋���邩
		shift(&my_copy, true, i, count);
		mayTurn = my_copy & enemy;
		//�Ԃ�\��������}�X�������
		while (mayTurn != 0) {
			//�V�t�g
			shift(&mayTurn, true, i);
			result = result | (mayTurn & unput);
			//�u����}�X������΂��̉��ɂ͒u���Ȃ�
			mayTurn = ~result & mayTurn;
			//�����̃}�X������΂��̉��ɂ͍s���Ȃ�
			mayTurn = ~my & mayTurn;
			count++;
		}
	}
	for (int i = 0; i < 4; i++) {
		uint64_t mayTurn = ALL_F;//�Ԃ�\��������}�X
		uint64_t my_copy = my;
		int count = 1;
		//�����ׂ̗ɓG�̋���邩
		shift(&my_copy, false, i, count);
		mayTurn = my_copy & enemy;
		//�Ԃ�\��������}�X�������
		while (mayTurn != 0) {
			//�V�t�g
			shift(&mayTurn, false, i);
			result = result | (mayTurn & unput);
			//�u����}�X������΂��̉��ɂ͒u���Ȃ�
			mayTurn = ~result & mayTurn;
			//�����̃}�X������΂��̉��ɂ͍s���Ȃ�
			mayTurn = ~my & mayTurn;
			count++;
		}
	}
	return result;
}

//action�u�����Ƃ���
uint64_t BitState::getTurnBoard(uint64_t action) const{
	uint64_t result = ALL_F;
	uint64_t unput = ~all_board;//�u���ĂȂ��}�X
	uint64_t ene_board = all_board ^ my_board;
	for (int i = 0; i < 4; i++) {
		uint64_t mayPut = action;
		uint64_t mayTurn = ALL_F;
		shift(&mayPut , true, i );
		mayPut = mayPut & ene_board;
		//�ׂɓG�̋����΃��[�v��
		while (mayPut != 0) {
			//�G�̃}�X�Ȃ�Ԃ�\������
			mayTurn = mayTurn | mayPut;
			//�X�ɗׂ𒲂ׂ�
			shift(&mayPut ,true , i );
			//�u����ꏊ����������
			if ((my_board & mayPut) != 0) {
				//���݂�mayTurn
				result = result | mayTurn;
				break;
			}
			mayPut = ~unput & mayPut;
		}
	}
	for (int i = 0; i < 4; i++) {
		uint64_t mayPut = action;
		uint64_t mayTurn = ALL_F;
		shift(&mayPut, false, i);
		mayPut = mayPut & ene_board;
		//�ׂɓG�̋����΃��[�v��
		while (mayPut != 0) {
			//�G�̃}�X�Ȃ�Ԃ�\������
			mayTurn = mayTurn | mayPut;
			//�X�ɗׂ𒲂ׂ�
			shift(&mayPut, false, i);
			//�u����ꏊ����������
			if ((my_board & mayPut) != 0) {
				//���݂�mayTurn
				result = result | mayTurn;
				break;
			}
			mayPut = ~unput & mayPut;
		}
	}
	return result;
}

void BitState::advance(uint64_t action) {
	//���@��ł����
	if ((canput_board & action) != 0) {
		//���u��
		all_board = all_board | action;
		my_board = my_board | action;
		//���Ԃ�
		my_board = my_board ^ getTurnBoard(action);
	}
	

	//�^�[����ς���(�{�[�h�̌���)
	turn ^= 1;
	my_board = all_board ^ my_board;

	//���@����擾
	canput_board = getLegalBoard();
}

bool BitState::isDone() const{
	return (getLegalBoard(WHITE) == 0 ) && (getLegalBoard(BLACK) == 0);
}

int BitState::getNum(piece color) const{
	uint64_t my;
	int count = 0;
	if (turn == color) {
		my = my_board;
	}
	else {
		my = my_board ^ all_board;
	}
	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
		if ((my & LOWEST) == 1) count++;
		my = my >> 1;
	}
	return count;
}

std::vector<uint64_t> BitState::legalAction() const{
	std::vector<uint64_t> legalActions;
	legalActions.clear();
	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
		if (((canput_board >> i) & LOWEST) == 1) {
			legalActions.push_back(LOWEST << i);
		}
	}
	if (legalActions.empty()) {
		legalActions.push_back(0);
	}
	return legalActions;
}

WinningStatus BitState::getWinningStatus() const{
	piece ene_color = (piece)(turn ^ 1);
	if (isDone()) {
		if (getNum((piece)turn) > getNum((piece) ene_color)) {
			return WIN;
		}
		else if (getNum((piece)turn) < getNum((piece)ene_color)) {
			return LOSE;
		}
		else {
			return DRAW;
		}
	}
	else {
		return NONE;
	}
}

int BitState::getEvaluation() const{
	int my_eval = 0;
	int ene_eval = 0;
	uint64_t ene_board = my_board ^ all_board;
	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
		if ((my_board >> i & LOWEST) == 1) {
			my_eval += EVAL[i];
		}
		else if ((ene_board >> i & LOWEST) == 1) {
			ene_eval += EVAL[i];
		}
	}
	return my_eval - ene_eval;
}

WinningStatus BitState::getBlackPlayerStatus() const{
	if (!isDone()) {
		return NONE;
	}
	else if (getNum(BLACK) < getNum(WHITE)) {
		return LOSE;
	}
	else if (getNum(BLACK) > getNum(WHITE)) {
		return WIN;
	}
	else {
		return DRAW;
	}
}

//�Ֆʂ��V�t�g����֐�
//�E�V�t�g�Ȃ�right == true
void shift(uint64_t* board, bool right, int vec, int num) {
	if (right) {
		*board = (*board >> SHIFT[vec]) & MASK_RIGHT[vec] * num;
	}
	else {
		*board = (*board << SHIFT[vec]) & MASK_LEFT[vec] * num;
	}
}

