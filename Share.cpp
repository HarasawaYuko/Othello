#include "Share.h"

int Share::level;
piece Share::playerColor;
int Share::loadPic;
int Share::playerStatus;
int Share::blackNum;
int Share::whiteNum;
Ai Share::ai = AI_MCTS;

bool Share::isIn(const int x, const int y, const int width, const int height, const int x_mouse, const int y_mouse) {
	if (x < x_mouse && x_mouse < x + width && y < y_mouse && y_mouse < y + height) {
		return true;
	}
	else {
		return false;
	}
}

void Share::Share_Initialize() {
	loadPic = LoadGraph("pic/Othello/load.png");
}