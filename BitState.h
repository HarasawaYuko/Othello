#pragma once
#include "include.h"
#include "Share.h"
//オセロのゲーム本体に関するクラス






class BitState {
private:

public:
	int turn;
	uint64_t my_board;
	uint64_t all_board;
	uint64_t canput_board;

	BitState();
	uint64_t getLegalBoard()const;
	uint64_t getLegalBoard(piece)const;
	uint64_t getTurnBoard(uint64_t)const;
	void advance(uint64_t);
	bool isDone()const;
	int getNum(piece)const;
	std::vector<uint64_t> legalAction()const;
	WinningStatus getWinningStatus()const;
	int getEvaluation()const;

	WinningStatus  getBlackPlayerStatus()const;
};