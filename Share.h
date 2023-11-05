#pragma once
#include "include.h"

//共通変数を管理するstaticクラス

enum WinningStatus {
	WIN,
	DRAW,
	LOSE,
	NONE
};

enum piece {
	BLACK = 0,
	WHITE = 1,
	UNPUT,
	CANPUT
};

enum Ai{
	AI_MCTS = 0,
	AI_Alpha = 1
};

const int BOARD_SIZE = 8;

class Share {
private:
	Share();

public:
	static piece playerColor;
	static int level;
	static int loadPic;
	static int playerStatus;
	static int blackNum;
	static int whiteNum;
	static const std::string AI[2];
	static Ai ai;
	//プロトタイプ宣言
	static void Share_Initialize();
};