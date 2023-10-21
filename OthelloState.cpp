#include "OthelloState.h"

/****構造体等の定義*****/

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
* ベクトルごとの確認
* enemy 隣の座標　vec ベクトルの種類
* return 置ける true  置けない false
*/
bool OthelloState::checkVector(Coord enemy, const int vec, const int enemyColor, const int putColor) const {
	struct Coord adjacent = Coord(enemy.x + vector[vec][0], enemy.y + vector[vec][1]);
	if (!adjacent.check()) {
		//盤面外だったらfalse
		return false;
	}
	//置いてなければ
	if (board[adjacent.x][adjacent.y] == UNPUT || board[adjacent.x][adjacent.y] == CANPUT) {
		return false;
	}
	//自分の色ならば挟める
	if (board[adjacent.x][adjacent.y] == putColor) {
		return true;
	}
	//相手の色なら更に奥を探索
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
	int enemy = turn ^ 1;//相手の色

	for (int i = 0; i < 8; i++) {
		//隣に相手の色の時のみ検証する
		Coord adjacent{ coord.x + vector[i][0], coord.y + vector[i][1] };
		//座標が盤面上に収まっていなければスキップ
		if (!adjacent.check()) continue;
		//裏返せるベクトルであれば
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

//盤面を進める
void OthelloState::advance(Coord action) {
	//合法手であれば
	//駒を置く
	if (isCanPut(action, turn)) {
		board[action.x][action.y] = turn;
		turnover(action);
	}
	//ターンを変える
	turn ^= 1;

	//合法手を取得
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

//ゲーム終了判定
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

//オーバーロード 
//指定されたプレイヤーの状況を返す
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
	//既に置いてあるマスであれば置けない
	if (board[coord.x][coord.y] == BLACK || board[coord.x][coord.y] == WHITE) {
		return false;
	}

	int putColor;//置く色
	int enemyColor;//相手の色
	//敵の色と自分の色を設定
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
		//座標が盤面上に収まっていなければスキップ
		if (!adjacent.check()) continue;
		////隣に相手の色の時のみ検証する
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