#include "Share.h"

int Share::level;
piece Share::playerColor;
int Share::loadPic;
int Share::playerStatus;
int Share::blackNum;
int Share::whiteNum;
Ai Share::ai = AI_MCTS;
const std::string Share::AI[2] = {"MCTS" , "AlphaBeta"};

// ?_mouse が 長方形の範囲にあるか判定する
bool Share::isIn(const int x, const int y, const int width, const int height, const int x_mouse, const int y_mouse) {
	if (   x < x_mouse && x_mouse < x + width 
		&& y < y_mouse && y_mouse < y + height) {
		return true;
	}
	else {
		return false;
	}
}

//オーバーロード Mouseクラスから?_mouseは自動で取得
bool Share::isIn(const int x, const int y, const int width, const int height) {
	if (   x < Mouse::instance()->getX() && Mouse::instance()->getX() < x + width 
		&& y < Mouse::instance()->getY() && Mouse::instance()->getY() < y + height) {
		return true;
	}
	else {
		return false;
	}
}

void Share::Share_Initialize() {
	loadPic = LoadGraph("pic/Othello/load.png");
}