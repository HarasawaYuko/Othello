#include "OthelloState.h"

/****�\���̓��̒�`*****/

static const int vector[8][2] = { {1,1},
							{1,0},
							{1,-1},
							{0,1},
							{0,-1},
							{-1,1},
							{-1,0},
							{-1,-1} };

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

bool Coord::check() {
	if (0 <= x && x < BOARD_SIZE && 0 <= y && y < BOARD_SIZE) {
		return true;
	}
	return false;
}

Coord::Coord(int x, int y) :x(x), y(y) {}

/***********private****************/

/*
* �x�N�g�����Ƃ̊m�F
* enemy �ׂ̍��W�@vec �x�N�g���̎��
* return �u���� true  �u���Ȃ� false
*/
bool OthelloState::checkVector(Coord enemy, const int vec, const int enemyColor, const int putColor) const {
	struct Coord adjacent = Coord(enemy.x + vector[vec][0], enemy.y + vector[vec][1]);
	if (!adjacent.check()) {
		//�ՖʊO��������false
		return false;
	}
	//�u���ĂȂ����
	if (board[adjacent.x][adjacent.y] == UNPUT || board[adjacent.x][adjacent.y] == CANPUT) {
		return false;
	}
	//�����̐F�Ȃ�΋��߂�
	if (board[adjacent.x][adjacent.y] == putColor) {
		return true;
	}
	//����̐F�Ȃ�X�ɉ���T��
	if (board[adjacent.x][adjacent.y] == enemyColor) {
		return checkVector(adjacent, vec, enemyColor, putColor);
	}
}

bool OthelloState::isCanPutAll(int turn) const {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (isCanPut(Coord(i, j), turn)) return true;
		}
	}
	return false;
}

void OthelloState::turnover(Coord coord) {
	int enemy = turn ^ 1;//����̐F

	for (int i = 0; i < 8; i++) {
		//�ׂɑ���̐F�̎��̂݌��؂���
		Coord adjacent{ coord.x + vector[i][0], coord.y + vector[i][1] };
		//���W���Ֆʏ�Ɏ��܂��Ă��Ȃ���΃X�L�b�v
		if (!adjacent.check()) continue;
		//���Ԃ���x�N�g���ł����
		if ((board[adjacent.x][adjacent.y] == enemy) && (checkVector(adjacent, i, enemy, turn))) {
			turnoverVec(adjacent, enemy, i);
		}
	}
}

void OthelloState::turnoverVec(Coord coord, int enemy, int vec) {
	if (board[coord.x][coord.y] == turn) {
		return;
	}
	else if (board[coord.x][coord.y] == enemy) {
		board[coord.x][coord.y] = turn;
		Coord adjacent = { coord.x + vector[vec][0] , coord.y + vector[vec][1] };
		turnoverVec(adjacent, enemy, vec);
	}
}


/***********public****************/

OthelloState::OthelloState() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = UNPUT;
		}
	}
	board[3][3] = BLACK;
	board[4][4] = BLACK;
	board[3][4] = WHITE;
	board[4][3] = WHITE;

	turn = BLACK;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (isCanPut(Coord(i, j), turn)) {
				board[i][j] = CANPUT;
			}
		}
	}
}

std::vector<Coord> OthelloState::getLegalAction()const {
	std::vector<Coord> legalActions ;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Coord coord = Coord(i, j);
			if (isCanPut(coord, turn)) {
				legalActions.push_back(coord);
			}
		}
	}
	if (legalActions.empty()) {
		Coord coord = Coord(-1, -1);
		legalActions.push_back(coord);
	}

	return legalActions;
}

std::vector<Coord> OthelloState::getLegalAction(bool* canPut)const {
	std::vector<Coord> legalActions;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Coord coord = Coord(i, j);
			if (isCanPut(coord, turn)) {
				legalActions.push_back(coord);
			}
		}
	}
	if (legalActions.empty()) {
		Coord coord = Coord(-1, -1);
		legalActions.push_back(coord);
		*canPut = false;
	}
	else {
		*canPut = true;
	}

	return legalActions;
}



std::vector<Coord> OthelloState::legalAction()const {
	std::vector<Coord> legalActions;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == CANPUT) {
				legalActions.push_back(Coord(i, j));
			}
		}
	}
	if (legalActions.empty()) {
		Coord coord = Coord(-1, -1);
		legalActions.push_back(coord);
	}

	return legalActions;
}

bool OthelloState::legalAction(int Turn)const {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Coord coord = Coord(i, j);
			if (isCanPut(coord, Turn)) {
				return true;
			}
		}
	}
	return false;
}

//�Ֆʂ�i�߂�
void OthelloState::advance(Coord action) {
	//���@��ł����
	//���u��
	if (isCanPut(action, turn)) {
		board[action.x][action.y] = turn;
		turnover(action);
	}
	//�^�[����ς���
	turn ^= 1;

	//���@����擾
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == CANPUT) board[i][j] = UNPUT;
		}
	}
	bool canPut;
	auto legalActions = getLegalAction(&canPut);
	if (canPut) {
		for (auto coord : legalActions) {
			board[coord.x][coord.y] = CANPUT;
		}
	}
}

//�Q�[���I������
bool OthelloState::isDone() const {
	bool result = true;
	if (isCanPutAll(WHITE)) {
		result = false;
	}
	if (isCanPutAll(BLACK)) {
		result = false;
	}
	return result;
}

WinningStatus OthelloState::getWinningStatus() const {
	int enemy = turn ^ 1;
	if (isDone()) {
		if (getNum(turn) > getNum(enemy)) {
			return WIN;
		}
		else if (getNum(turn) < getNum(enemy)) {
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

//�I�[�o�[���[�h 
//�w�肳�ꂽ�v���C���[�̏󋵂�Ԃ�
WinningStatus OthelloState::getWinningStatus(piece color) const {
	int enemy = color ^ 1;
	if (isDone()) {
		if (getNum(turn) > getNum(enemy)) {
			return WIN;
		}
		else if (getNum(turn) < getNum(enemy)) {
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

int OthelloState::getScore()const {
	return 0;
}

int OthelloState::getNum(int turn) const {
	int count = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == turn) count++;
		}
	}
	return count;
}

bool OthelloState::isCanPut(Coord coord, int turn) const {
	if (!coord.check()) {
		return false;
	}
	//���ɒu���Ă���}�X�ł���Βu���Ȃ�
	if (board[coord.x][coord.y] == BLACK || board[coord.x][coord.y] == WHITE) {
		return false;
	}

	int putColor;//�u���F
	int enemyColor;//����̐F
	//�G�̐F�Ǝ����̐F��ݒ�
	if (turn == BLACK) {
		putColor = BLACK;
		enemyColor = WHITE;
	}
	else {
		putColor = WHITE;
		enemyColor = BLACK;
	}

	for (int i = 0; i < 8; i++) {
		struct Coord adjacent = { coord.x + vector[i][0], coord.y + vector[i][1] };
		//���W���Ֆʏ�Ɏ��܂��Ă��Ȃ���΃X�L�b�v
		if (!adjacent.check()) continue;
		////�ׂɑ���̐F�̎��̂݌��؂���
		if ((board[adjacent.x][adjacent.y] == enemyColor) && checkVector(adjacent, i, enemyColor, putColor)) {
			return true;
		}
	}
	return false;
}

int OthelloState::getPiece(Coord coord) const {
	return board[coord.x][coord.y];
}

WinningStatus OthelloState::getBlackPlayerStatus() const {
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

int OthelloState::getEvaluation()const {
	int enemy = turn ^ 1;

	int myScore = 0;
	int enemyScore = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == turn) {
				myScore += BOARD_EVAL[i][j];
			}
			else if (board[i][j] == enemy) {
				enemyScore += BOARD_EVAL[i][j];
			}
		}
	}

	return myScore - enemyScore;
}