#pragma once
#include "include.h"

//オセロのゲーム本体に関するクラス


enum piece {
	BLACK = 0,
	WHITE = 1,
	UNPUT,
	CANPUT
};

struct Coord {
public:
	int x;
	int y;

	Coord(int, int);
	bool check();
};

enum WinningStatus {
	WIN,
	DRAW,
	LOSE,
	NONE
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