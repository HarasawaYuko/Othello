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
	//プロトタイプ宣言
	static bool isIn(const int, const int, const int, const int, const int, const int);
	static void Share_Initialize();
};