#pragma once
#include "include.h"

//共通変数を管理するstaticクラス

class Share {
private:
	Share();

public:
	static int playerColor;
	static int level;
	static int loadPic;
	static int winner;
	//プロトタイプ宣言
	static bool isIn(const int, const int, const int, const int, const int, const int);
	static void Share_Initialize();
};