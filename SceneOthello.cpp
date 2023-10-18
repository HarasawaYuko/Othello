#include "SceneOthello.h"
#include "OthelloAIMcts.h"


namespace {
	int SQUARE_SIZE; //マスの大きさ
	int TOP_MARGIN = 70; //上側マージンの広さ
	int SIDE_MARGIN = 220; //左側マージンの広さ
	//const int RECEPTION = 100; //入力受付時間(ミリ秒)
	Coord selectCoord = Coord(0, 0);
	int boardPic;
	int blackPic;
	int whitePic;
	int selectPic;
	int boardYellowPic;
	bool InBoard;
	OthelloState state;
	const int playoutNum = 100;
	const int thinkTime = 100;
}

void MouseToCoord(int, int, Coord*);


void Othello_Initialize() {
	state = OthelloState();
	boardPic = LoadGraph("pic/Othello/board.png");
	boardYellowPic = LoadGraph("pic/Othello/boardYellow.png");
	blackPic = LoadGraph("pic/Othello/black.png");
	whitePic = LoadGraph("pic/Othello/white.png");
	selectPic = LoadGraph("pic/Othello/SelectFrame.png");

	//マスの大きさを設定
	if (WIN_SIZE_X - SIDE_MARGIN * 2 > WIN_SIZE_Y - TOP_MARGIN * 2) {
		SQUARE_SIZE = (WIN_SIZE_Y - TOP_MARGIN * 2) / BOARD_SIZE;
	}
	else {
		SQUARE_SIZE = (WIN_SIZE_X - SIDE_MARGIN * 2) / BOARD_SIZE;
	}
}

void Othello_Update() {
	//マウス位置の取得
	int mousePosX;
	int mousePosY;
	int mouseInput;
	GetMousePoint(&mousePosX , &mousePosY);
	mouseInput = GetMouseInput();
	MouseToCoord(mousePosX , mousePosY , &selectCoord);
	if (!state.isDone()) {
		if (state.turn == BLACK) {
			TimeKeeper tk_think = TimeKeeper(thinkTime);
			state.advance(mctsActionOthello(state, playoutNum));
			while (!tk_think.isTimeOver()) {}
			return;
		}
		else {
			/*if (mouseInput & MOUSE_INPUT_LEFT && state.isCanPut(selectCoord, state.turn)) {
				state.advance(selectCoord);
			}*/
			state.advance(randomActionOthello(state));
		}
	}
	//if(!state.isDone())state.advance(randomActionOthello(state));
}

void Othello_Draw() {

	int x_pos = SIDE_MARGIN;
	int y_pos = TOP_MARGIN;
	for (int i = 0; i < BOARD_SIZE; i++) {
		y_pos = TOP_MARGIN;
		for (int j = 0; j < BOARD_SIZE; j++) {
			switch (state.getPiece(Coord(i ,j))) {
			case CANPUT:
				DrawExtendGraph(x_pos, y_pos, x_pos + SQUARE_SIZE, y_pos + SQUARE_SIZE, boardYellowPic, false);
				break;
			case WHITE:
				DrawExtendGraph(x_pos, y_pos, x_pos + SQUARE_SIZE, y_pos + SQUARE_SIZE, boardPic, false);
				DrawExtendGraph(x_pos, y_pos, x_pos + SQUARE_SIZE, y_pos + SQUARE_SIZE, whitePic, true);
				break;
			case BLACK:
				DrawExtendGraph(x_pos, y_pos, x_pos + SQUARE_SIZE, y_pos + SQUARE_SIZE, boardPic, false);
				DrawExtendGraph(x_pos, y_pos, x_pos + SQUARE_SIZE, y_pos + SQUARE_SIZE, blackPic, true);
				break;
			default:
				DrawExtendGraph(x_pos, y_pos, x_pos + SQUARE_SIZE, y_pos + SQUARE_SIZE, boardPic, false);
			};
			y_pos += SQUARE_SIZE;
		}
		x_pos += SQUARE_SIZE;
	}
	//選択枠表示
	if (InBoard)DrawExtendGraph(SIDE_MARGIN + (selectCoord.x * SQUARE_SIZE), TOP_MARGIN + (selectCoord.y * SQUARE_SIZE), SIDE_MARGIN + ((selectCoord.x + 1) * SQUARE_SIZE), TOP_MARGIN + ((selectCoord.y + 1) * SQUARE_SIZE), selectPic, true);

	//個数表示
	SetFontSize(150);
	std::string black = std::to_string(state.getNum(BLACK));
	std::string white = std::to_string(state.getNum(WHITE));
	int blackWidth = GetDrawStringWidth(black.c_str(), (int)black.length());
	int whiteWidth = GetDrawStringWidth(white.c_str(), (int)white.length());
	DrawFormatString((int)((WIN_SIZE_X / 2 - SQUARE_SIZE * 4) / 2 - blackWidth / 2),(int)(WIN_SIZE_Y / 2), GetColor(0, 0, 0), "%s", black.c_str());
	DrawFormatString((int)(WIN_SIZE_X * 0.75 + SQUARE_SIZE * 2 - whiteWidth / 2), (int)(WIN_SIZE_Y / 2), GetColor(0, 0, 0), "%s", white.c_str());

	//ゲーム終了時の表示
	if (state.isDone()) {
		SetFontSize(60);
		int winWidth = GetDrawStringWidth("WIN!", -1);
		int loseWidth = GetDrawStringWidth("LOSE", -1);
		if (state.getBlackPlayerStatus() == LOSE) {
			DrawString((int)((WIN_SIZE_X / 2 - SQUARE_SIZE * 4) / 2 - winWidth / 2), (int)(WIN_SIZE_Y / 2 + 150), "LOSE", GetColor(0, 0, 255));
			DrawString((int)(WIN_SIZE_X * 0.75 + SQUARE_SIZE * 2 - loseWidth / 2), (int)(WIN_SIZE_Y / 2 + 150), "WIN!", GetColor(255, 0, 0));
		}
		else if(state.getBlackPlayerStatus() == WIN) {
			DrawString((int)((WIN_SIZE_X / 2 - SQUARE_SIZE * 4) / 2 - winWidth / 2 ), (int)(WIN_SIZE_Y / 2 + 150), "WIN!", GetColor(255, 0, 0));
			DrawString((int)(WIN_SIZE_X * 0.75 + SQUARE_SIZE * 2 - loseWidth / 2), (int)(WIN_SIZE_Y / 2 + 150), "LOSE", GetColor(0, 0, 255));
		}
	}
	else {
		//AI思考表示
		if (state.turn == BLACK) {
			SetFontSize(25);
			int width = GetDrawStringWidth("AI thinking...", -1);
			DrawString((WIN_SIZE_X / 2 - SQUARE_SIZE * 4) / 2 - width / 2, WIN_SIZE_Y / 2 + 150, "AI Thinking...", GetColor(0, 30, 200));
		}
		//ターン表示
		int r = 50;
		if (state.turn == BLACK) {
			DrawCircle(((WIN_SIZE_X / 2 - SQUARE_SIZE * 4)) / 2, WIN_SIZE_Y / 2 - r - 20, r, GetColor(255, 255, 0), true);
		}
		else {
			DrawCircle((int)(WIN_SIZE_X * 0.75 + SQUARE_SIZE * 2), WIN_SIZE_Y / 2 - r - 20, r, GetColor(255, 255, 0), true);
			DrawCircle((int)(WIN_SIZE_X * 0.75 + SQUARE_SIZE * 2), WIN_SIZE_Y / 2 - r - 20, r, GetColor(0, 0, 0), false);
		}
	}
}

/*
* マウスの座標を盤面上のPointに変換
*/
void MouseToCoord(int x, int y, Coord* point) {
	int pointX = (x - SIDE_MARGIN) / SQUARE_SIZE;
	int pointY = (y - TOP_MARGIN) / SQUARE_SIZE;
	Coord tmp = Coord(pointX, pointY);
	if (tmp.check() && (x - SIDE_MARGIN) > 0 && (y - TOP_MARGIN) > 0) {
		InBoard = true;
	}
	else {
		InBoard = false;
	}
	point->x = tmp.x;
	point->y = tmp.y;
}