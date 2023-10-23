#include "BitState.h"

/****構造体等の定義*****/

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

//ビットマスク
static const uint64_t ALL_T   = 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111;
static const uint64_t ALL_F   = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
static const uint64_t LOWEST  = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001;
static const uint64_t HIGHEST  = 0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
static const uint64_t TOP_F   = 0b00000000'11111111'11111111'11111111'11111111'11111111'11111111'11111111;
static const uint64_t UNDER_F = 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'00000000;
static const uint64_t RIGHT_F = 0b11111110'11111110'11111110'11111110'11111110'11111110'11111110'11111110;
static const uint64_t LEFT_F  = 0b01111111'01111111'01111111'01111111'01111111'01111111'01111111'01111111;

//ベクトル
static const int SHIFT[] = { 1 , 7 , 8 , 9};
//シフトごと
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
	uint64_t unput = ~all_board;//置いてないマス

	uint64_t mayPut = ALL_F;//置ける可能性があるマス
	//右シフトのループ(ベクトルごと)
	for (int i = 0; i < 4; i++) {
		uint64_t my_copy = my_board;
		uint64_t mayTurn = ALL_F;//返る可能性があるマス
		int count = 1;
		//自分の隣に敵の駒があるか
		shift(&my_copy, true, i, count);
		mayTurn = my_copy & enemy_board;
		//返る可能性があるマスがあれば
		while (mayTurn != 0) {
			//シフト
			shift(&mayTurn, true, i);
			result = result | (mayTurn & unput);
			//置けるマスがあればその奥には置けない
			mayTurn = ~result & mayTurn;
			//自分のマスがあればその奥には行けない
			mayTurn = ~my_board & mayTurn;
			count++;
		}
	}
	for (int i = 0; i < 4; i++) {
		uint64_t mayTurn = ALL_F;//返る可能性があるマス
		uint64_t my_copy = my_board;
		int count = 1;
		//自分の隣に敵の駒があるか
		shift(&my_copy, false, i, count);
		mayTurn = my_copy & enemy_board;
		//返る可能性があるマスがあれば
		while (mayTurn != 0) {
			//シフト
			shift(&mayTurn, false, i);
			result = result | (mayTurn & unput);
			//置けるマスがあればその奥には置けない
			mayTurn = ~result & mayTurn;
			//自分のマスがあればその奥には行けない
			mayTurn = ~my_board & mayTurn;
			count++;
		}
	}
	return result;
}

//オーバーロード　プレイヤーを指定できる
uint64_t BitState::getLegalBoard(piece color) const{
	uint64_t my = my_board;
	uint64_t enemy = all_board xor my;
	uint64_t result = ALL_F;
	uint64_t unput = ~all_board;//置いてないマス

	if (color != turn) {
		uint64_t tmp = my;
		my = enemy;
		enemy = tmp;
	}

	uint64_t mayPut = ALL_F;//置ける可能性があるマス
	//右シフトのループ(ベクトルごと)
	for (int i = 0; i < 4; i++) {
		uint64_t my_copy = my;
		uint64_t mayTurn = ALL_F;//返る可能性があるマス
		int count = 1;
		//自分の隣に敵の駒があるか
		shift(&my_copy, true, i, count);
		mayTurn = my_copy & enemy;
		//返る可能性があるマスがあれば
		while (mayTurn != 0) {
			//シフト
			shift(&mayTurn, true, i);
			result = result | (mayTurn & unput);
			//置けるマスがあればその奥には置けない
			mayTurn = ~result & mayTurn;
			//自分のマスがあればその奥には行けない
			mayTurn = ~my & mayTurn;
			count++;
		}
	}
	for (int i = 0; i < 4; i++) {
		uint64_t mayTurn = ALL_F;//返る可能性があるマス
		uint64_t my_copy = my;
		int count = 1;
		//自分の隣に敵の駒があるか
		shift(&my_copy, false, i, count);
		mayTurn = my_copy & enemy;
		//返る可能性があるマスがあれば
		while (mayTurn != 0) {
			//シフト
			shift(&mayTurn, false, i);
			result = result | (mayTurn & unput);
			//置けるマスがあればその奥には置けない
			mayTurn = ~result & mayTurn;
			//自分のマスがあればその奥には行けない
			mayTurn = ~my & mayTurn;
			count++;
		}
	}
	return result;
}

//action置いたところ
uint64_t BitState::getTurnBoard(uint64_t action) const{
	uint64_t result = ALL_F;
	uint64_t unput = ~all_board;//置いてないマス
	uint64_t ene_board = all_board ^ my_board;
	for (int i = 0; i < 4; i++) {
		uint64_t mayPut = action;
		uint64_t mayTurn = ALL_F;
		shift(&mayPut , true, i );
		mayPut = mayPut & ene_board;
		//隣に敵の駒があればループへ
		while (mayPut != 0) {
			//敵のマスなら返る可能性あり
			mayTurn = mayTurn | mayPut;
			//更に隣を調べる
			shift(&mayPut ,true , i );
			//置ける場所発見したら
			if ((my_board & mayPut) != 0) {
				//現在のmayTurn
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
		//隣に敵の駒があればループへ
		while (mayPut != 0) {
			//敵のマスなら返る可能性あり
			mayTurn = mayTurn | mayPut;
			//更に隣を調べる
			shift(&mayPut, false, i);
			//置ける場所発見したら
			if ((my_board & mayPut) != 0) {
				//現在のmayTurn
				result = result | mayTurn;
				break;
			}
			mayPut = ~unput & mayPut;
		}
	}
	return result;
}

void BitState::advance(uint64_t action) {
	//合法手であれば
	if ((canput_board & action) != 0) {
		//駒を置く
		all_board = all_board | action;
		my_board = my_board | action;
		//裏返す
		my_board = my_board ^ getTurnBoard(action);
	}
	

	//ターンを変える(ボードの交換)
	turn ^= 1;
	my_board = all_board ^ my_board;

	//合法手を取得
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

//盤面をシフトする関数
//右シフトならright == true
void shift(uint64_t* board, bool right, int vec, int num) {
	if (right) {
		*board = (*board >> SHIFT[vec]) & MASK_RIGHT[vec] * num;
	}
	else {
		*board = (*board << SHIFT[vec]) & MASK_LEFT[vec] * num;
	}
}

