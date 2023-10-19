#pragma once
#include "include.h"
#include "Share.h"

//オセロのゲーム本体に関するクラス


struct Coord {
public:
	int x;
	int y;

	Coord(int, int);
	bool check();
};


static const int BOARD_SIZE = 8;

class OthelloState {
private:

public:
	int turn;
	int board[BOARD_SIZE][BOARD_SIZE];

	OthelloState();
	std::vector<Coord> getLegalAction()const;
	std::vector<Coord> getLegalAction(bool*)const;
	std::vector<Coord> legalAction()const;
	bool legalAction(int)const;
	void advance(Coord);
	bool isDone() const;
	WinningStatus getWinningStatus()const;
	WinningStatus getWinningStatus(piece)const;
	int getScore()const;
	int getNum(int)const;
	bool isCanPut(Coord, int) const;
	int getPiece(Coord)const;
	WinningStatus getBlackPlayerStatus() const;


	bool checkVector(Coord, int, int, int)const;
	bool isCanPutAll(int) const;
	void turnover(Coord);
	void turnoverVec(Coord, int, int);
};