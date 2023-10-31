#include "Share.h"

int Share::level;
piece Share::playerColor;
int Share::loadPic;
int Share::playerStatus;
int Share::blackNum;
int Share::whiteNum;
Ai Share::ai = AI_MCTS;
const std::string Share::AI[2] = {"MCTS" , "AlphaBeta"};

void Share::Share_Initialize() {
	loadPic = LoadGraph("pic/load.png");
}