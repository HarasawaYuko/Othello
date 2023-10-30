#pragma once
#include "include.h"
#include "Share.h"
//オセロのゲーム本体に関するクラス

const uint64_t ALL_T = 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111;
const uint64_t ALL_F = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
const uint64_t LOWEST = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001;
const uint64_t HIGHEST = 0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;




class State {
private:
public:
	int turn;
	uint64_t my_board;
	uint64_t all_board;
	uint64_t canput_board;

	State();
	uint64_t getLegalBoard()const;
	uint64_t getLegalBoard(piece)const;
	uint64_t getTurnBoard(uint64_t)const;
	void advance(uint64_t);
	bool isDone()const;
	bool isCanPut(uint64_t) const;
	bool isCanPutAll() const;
	int getNum(piece)const;
	std::vector<uint64_t> legalAction()const;
	WinningStatus getWinningStatus()const;
	WinningStatus getWinningStatus(piece)const;
	int getEvaluation()const;
	piece getPiece(uint64_t)const;

	WinningStatus  getBlackPlayerStatus()const;
};