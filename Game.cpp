#include "Game.h"
#include "Share.h"
#include "TimeKeeper.h"
#include "OthelloAIMcts.h"
#include "OhtelloAIRandom.h"

/***定数***/
static const int TOP_MARGIN = 50;//上側マージン
static const int SIDE_MARGIN = 150;//横マージン
static const int SQUARE_SIZE = 62;//マスの大きさ
static const int PLAYOUT_MAX = 100;//プレイアウトの回数
static const int THINK_TIME = 1;//AIの思考時間
static const int UNDO_NUM = 3;//undoできる数
static const int TO_RESULT_TIME = 10;

//個数表示関係
static const int BOX_WIDTH = 120;
static const int BOX_HEIGHT = 120;
static const int BOX_BLACK_X = 15;
static const int BOX_WHITE_X = WIN_SIZE_X - BOX_BLACK_X - BOX_WIDTH;
static const int BOX_Y = 300;

//Undoボタン関係
static const int UNDO_X = 730;
static const int UNDO_Y = 10;
static const int UNDO_WIDTH = 60;
static const int UNDO_HEIGHT = 45;

//終了ボタン関係
static const int STOP_X = 10;
static const int STOP_Y = 10;
static const int STOP_WIDTH = 60;
static const int STOP_HEIGHT = 45;

//色
const unsigned int COLOR_BOARD = GetColor(0x10, 0xB5, 0x67);
static const unsigned int COLOR_BLUE = GetColor(0, 30, 150);
static const unsigned int COLOR_LBLUE = GetColor(101, 187, 233);
static const unsigned int COLOR_CANPUT = GetColor(0xFF, 0x99, 0x33);
static const unsigned int COLOR_SELECT = GetColor(0x00, 0x33, 0xCC);
static const unsigned int COLOR_RED = GetColor(220, 45, 35);


//描画判定用
static bool onBoard;
static bool onUndo;
static bool onStop;

//音声判定用
static bool nowPut;
static bool nowToMenu;
static bool nowPass;
static bool nowUndo;
static bool nowStop;

//画面遷移判定用
static bool timer;
static TimeKeeper timerToResult;

Game::Game(SceneChanger *changer) 
	:BaseScene(changer)
{}

Game::~Game() 
{}

void Game::Initialize() {
	m_state = OthelloState();
	timer = false;
	

	//画像のロード
	m_blackPic = LoadGraph("pic/Othello/black.png");
	m_whitePic = LoadGraph("pic/Othello/white.png");
	m_gamePic = LoadGraph("pic/Othello/game.png");
	m_boxBlackPic = LoadGraph("pic/Othello/boxBlack.png");
	m_boxWhitePic = LoadGraph("pic/Othello/boxWhite.png");
	m_undoButtonPic = LoadGraph("pic/Othello/undo.png");
	m_undoButtonOffPic = LoadGraph("pic/Othello/notUndo.png");
	m_undoFramePic = LoadGraph("pic/Othello/undoFrame.png");
	m_stopButtonPic = LoadGraph("pic/Othello/stop.png");

	//音声のロード
	m_pieceSnd = LoadSoundMem("sound/Othello/piece.mp3");
	m_gameSnd = LoadSoundMem("sound/Othello/game.mp3");
	m_passSnd = LoadSoundMem("sound/Othello/pass.mp3");
	m_undoSnd = LoadSoundMem("sound/Othello/undo.mp3");
	m_stopSnd = LoadSoundMem("sound/Othello/stop.mp3");
	
	//音量設定
	ChangeVolumeSoundMem(110, m_gameSnd);

	//プレイアウト回数
	m_playoutNum = (PLAYOUT_MAX / 10) * Share::level;

	//BGMを再生
	PlaySoundMem(m_gameSnd, DX_PLAYTYPE_LOOP, true);
}


void Game::Update() {
	//フラグのリセット
	nowPut = false;
	nowPass = false;
	nowUndo = false;
	onUndo = false;
	onStop = false;
	//マウス位置の取得
	int mousePosX;
	int mousePosY;
	int mouseInput;
	GetMousePoint(&mousePosX, &mousePosY);
	mouseInput = GetMouseInput();
	MouseToCoord(mousePosX, mousePosY, &m_selectCoord);
	if (!m_state.isDone()) {
		//プレイヤーの手番
		if (m_state.turn == Share::playerColor) {
			//パス
			if (!m_state.isCanPutAll(m_state.turn)) {
				m_state.advance(Coord(-1, -1));
				nowPass = true;
			}
			//置く
			else if (mouseInput & MOUSE_INPUT_LEFT && m_state.isCanPut(m_selectCoord, m_state.turn)) {
				//置く前に
				undo_vec.push_back(std::make_pair(m_state, m_currentPut));
				if (undo_vec.size() > UNDO_NUM) {
					//先頭から削除
					undo_vec.erase(undo_vec.begin());
				}
				m_state.advance(m_selectCoord);
				nowPut = true;
				m_currentPut = m_selectCoord;
			}
		}
		//AIの手番
		else {
			TimeKeeper tk_think = TimeKeeper(THINK_TIME);
			Coord tmp = mctsActionOthello(m_state, m_playoutNum);
			m_state.advance(tmp);
			if (tmp.check()) {
				m_currentPut = tmp;
				nowPut = true;
			}
			else {
				nowPass = true;
			}
			while (!tk_think.isTimeOver()) {}
		}
	}
	//試合終了時
	else {
		//タイマーが起動していなければ
		if (!timer) {
			timerToResult = TimeKeeper(TO_RESULT_TIME);
			timer = true;
		}
		if (timerToResult.isTimeOver()) {
			m_sceneChanger->ChangeScene(Scene_Result);
		}
	}
	if (Share::isIn(UNDO_X, UNDO_Y, UNDO_X + UNDO_WIDTH, UNDO_Y + UNDO_HEIGHT, mousePosX, mousePosY)) {
		onUndo = true;
		if (mouseInput & MOUSE_INPUT_LEFT && undo_vec.size() > 0) {
			m_state = undo_vec.back().first;
			m_currentPut = undo_vec.back().second;
			nowUndo = true;
			undo_vec.erase(undo_vec.end() - 1);
		}
	}
	//終了ボタン
	if (Share::isIn(STOP_X, STOP_Y, STOP_X + STOP_WIDTH, STOP_Y + STOP_HEIGHT, mousePosX, mousePosY)) {
		onStop = true;
		if (mouseInput & MOUSE_INPUT_LEFT) {
			m_sceneChanger->ChangeScene(Scene_Menu);
			nowToMenu = true;
			m_currentPut.x = -1;
			m_currentPut.y = -1;
		}
	}
}

void Game::Draw() {
	//背景
	DrawExtendGraph(0, 0, WIN_SIZE_X, WIN_SIZE_Y, m_gamePic, true);

	//ボード
	int x_pos = SIDE_MARGIN;
	int y_pos = TOP_MARGIN;
	for (int i = 0; i < BOARD_SIZE; i++) {
		y_pos = TOP_MARGIN;
		for (int j = 0; j < BOARD_SIZE; j++) {
			switch (m_state.getPiece(Coord(i, j))) {
			case CANPUT:
				DrawBoxAA((float)(x_pos + 1), (float)(y_pos + 1), (float)(x_pos + SQUARE_SIZE - 1), (float)(y_pos + SQUARE_SIZE - 1), COLOR_CANPUT, true, 2.0f);
				break;
			case WHITE:
				DrawBoxAA((float)(x_pos + 1), (float)(y_pos + 1), (float)(x_pos + SQUARE_SIZE - 1), (float)(y_pos + SQUARE_SIZE - 1), COLOR_BOARD, true, 2.0f);
				DrawExtendGraph(x_pos, y_pos, x_pos + SQUARE_SIZE, y_pos + SQUARE_SIZE, m_whitePic, true);
				break;
			case BLACK:
				DrawBoxAA((float)(x_pos + 1), (float)(y_pos + 1), (float)(x_pos + SQUARE_SIZE - 1), (float)(y_pos + SQUARE_SIZE - 1), COLOR_BOARD, true, 2.0f);
				DrawExtendGraph(x_pos, y_pos, x_pos + SQUARE_SIZE, y_pos + SQUARE_SIZE, m_blackPic, true);
				break;
			default:
				DrawBoxAA((float)(x_pos + 1), (float)(y_pos + 1), (float)(x_pos + SQUARE_SIZE - 1), (float)(y_pos + SQUARE_SIZE - 1), COLOR_BOARD, true, 2.0f);
			};
			y_pos += SQUARE_SIZE;
		}
		x_pos += SQUARE_SIZE;
	}
	//選択枠表示
	if (onBoard && m_state.turn == Share::playerColor) {
		DrawBoxAA((float)(SIDE_MARGIN + (m_selectCoord.x * SQUARE_SIZE) + 1), (float)(TOP_MARGIN + (m_selectCoord.y * SQUARE_SIZE) + 1), (float)(SIDE_MARGIN + ((m_selectCoord.x + 1) * SQUARE_SIZE) - 1), (float)(TOP_MARGIN + ((m_selectCoord.y + 1) * SQUARE_SIZE) - 1), COLOR_BLUE, false, 5.0f);
	}
	//置いた場所
	if (m_currentPut.check()) {
		DrawCircleAA((float)(SIDE_MARGIN + SQUARE_SIZE * m_currentPut.x + SQUARE_SIZE / 2 - 3), (float)(TOP_MARGIN + SQUARE_SIZE * m_currentPut.y + SQUARE_SIZE / 2 - 4), 10, 15, COLOR_LBLUE, true, 0);
	}

	//個数表示
	DrawExtendGraph(BOX_BLACK_X, BOX_Y, BOX_BLACK_X + BOX_WIDTH, BOX_Y + BOX_HEIGHT, m_boxBlackPic, true);
	DrawExtendGraph(BOX_WHITE_X, BOX_Y, BOX_WHITE_X + BOX_WIDTH, BOX_Y + BOX_HEIGHT, m_boxWhitePic, true);
	DrawExtendGraph(BOX_BLACK_X + 2, BOX_Y + 2, BOX_BLACK_X + 52, BOX_Y + 52, m_blackPic, true);
	DrawExtendGraph(BOX_WHITE_X + 2, BOX_Y + 2, BOX_WHITE_X + 52, BOX_Y + 52, m_whitePic, true);

	SetFontSize(50);
	std::string black = std::to_string(m_state.getNum(BLACK));
	std::string white = std::to_string(m_state.getNum(WHITE));
	int blackWidth = GetDrawStringWidth(black.c_str(), (int)black.length());
	int whiteWidth = GetDrawStringWidth(white.c_str(), (int)white.length());
	DrawFormatString(BOX_BLACK_X + BOX_WIDTH - 5 - blackWidth, BOX_Y + BOX_HEIGHT - 55, GetColor(230, 230, 230), "%s", black.c_str());
	DrawFormatString(BOX_WHITE_X + BOX_WIDTH - 5 - whiteWidth, BOX_Y + BOX_HEIGHT - 55, GetColor(50, 50, 50), "%s", white.c_str());

	SetFontSize(20);
	std::string blackStr;
	std::string whiteStr;
	if (Share::playerColor == BLACK) {
		blackStr = "Player";
		whiteStr = "     AI";
	}
	else {
		blackStr = "     AI";
		whiteStr = "Player";
	}
	DrawString(BOX_BLACK_X + 50, BOX_Y + 40, blackStr.c_str(), GetColor(230, 230, 230));
	DrawString(BOX_WHITE_X + 50, BOX_Y + 40, whiteStr.c_str(), GetColor(50, 50, 50));

	////ゲーム終了時の表示
	if (m_state.isDone()) {
		SetFontSize(50);
		int winWidth = GetDrawStringWidth("WIN!", 4);
		int loseWidth = GetDrawStringWidth("LOSE", 4);
		if (m_state.getBlackPlayerStatus() == LOSE) {
			DrawString((int)((WIN_SIZE_X / 2 - SQUARE_SIZE * 4) / 2 - winWidth / 2), (int)(WIN_SIZE_Y / 2 + 150), "LOSE", COLOR_LBLUE);
			DrawString((int)(WIN_SIZE_X * 0.75 + SQUARE_SIZE * 2 - loseWidth / 2), (int)(WIN_SIZE_Y / 2 + 150), "WIN!", COLOR_RED);
		}
		else if (m_state.getBlackPlayerStatus() == WIN) {
			DrawString((int)((WIN_SIZE_X / 2 - SQUARE_SIZE * 4) / 2 - winWidth / 2), (int)(WIN_SIZE_Y / 2 + 150), "WIN!", COLOR_RED);
			DrawString((int)(WIN_SIZE_X * 0.75 + SQUARE_SIZE * 2 - loseWidth / 2), (int)(WIN_SIZE_Y / 2 + 150), "LOSE", COLOR_LBLUE);
		}
	}
	else {
		//ターン表示
		if (m_state.turn == BLACK) {
			DrawBoxAA(BOX_BLACK_X - 4, BOX_Y - 4, BOX_BLACK_X + BOX_WIDTH + 4, BOX_Y + BOX_HEIGHT + 4, COLOR_LBLUE, false, 8);
		}
		else {
			DrawBoxAA(BOX_WHITE_X - 4, BOX_Y - 4, BOX_WHITE_X + BOX_WIDTH + 4, BOX_Y + BOX_HEIGHT + 4, COLOR_LBLUE, false, 8);
		}
	}
	//undoボタン
	if (undo_vec.empty()) {
		DrawExtendGraph(UNDO_X, UNDO_Y, UNDO_X + UNDO_WIDTH, UNDO_Y + UNDO_HEIGHT, m_undoButtonOffPic, true);
	}
	else {
		DrawExtendGraph(UNDO_X, UNDO_Y, UNDO_X + UNDO_WIDTH, UNDO_Y + UNDO_HEIGHT, m_undoButtonPic, true);
	}
	if (onUndo) {
		DrawExtendGraph(UNDO_X, UNDO_Y, UNDO_X + UNDO_WIDTH, UNDO_Y + UNDO_HEIGHT, m_undoFramePic, true);
	}
	//stopボタン
	DrawExtendGraph(STOP_X, STOP_Y, STOP_X + STOP_WIDTH, STOP_Y + STOP_HEIGHT, m_stopButtonPic, true);
	if (onStop) {
		DrawExtendGraph(STOP_X, STOP_Y, STOP_X + STOP_WIDTH, STOP_Y + STOP_HEIGHT, m_undoFramePic, true);
	}
	//音声
	if (nowPut) {
		PlaySoundMem(m_pieceSnd, DX_PLAYTYPE_NORMAL, true);
	}
	if (nowPass) {
		PlaySoundMem(m_passSnd, DX_PLAYTYPE_NORMAL, true);
	}
	if (nowUndo) {
		PlaySoundMem(m_undoSnd, DX_PLAYTYPE_NORMAL, true);
	}
}

void Game::Finalize() {
	//ロード画面の表示
	DrawExtendGraph(0, 0, WIN_SIZE_X + 5, WIN_SIZE_Y , Share::loadPic, true);
	//BGMを止める
	StopSoundMem(m_gameSnd);
	//メニュー画面遷移ならSE
	if (nowToMenu) {
		PlaySoundMem(m_stopSnd , DX_PLAYTYPE_NORMAL, true);
	}
	//勝者の記録
	Share::playerStatus = m_state.getWinningStatus(Share::playerColor);
	Share::blackNum = m_state.getNum(BLACK);
	Share::whiteNum = m_state.getNum(WHITE);
	//ハンドルの解放
	deleteMem();
}

/*
* マウスの座標を盤面上のPointに変換
*/
void Game::MouseToCoord(int x, int y, Coord* point) {
	int pointX = (x - SIDE_MARGIN) / SQUARE_SIZE;
	int pointY = (y - TOP_MARGIN) / SQUARE_SIZE;
	Coord tmp = Coord(pointX, pointY);
	if (tmp.check() && (x - SIDE_MARGIN) > 0 && (y - TOP_MARGIN) > 0) {
		onBoard = true;
	}
	else {
		onBoard = false;
	}
	point->x = tmp.x;
	point->y = tmp.y;
}


void Game::deleteMem() {
	DeleteGraph(m_gamePic);
	DeleteGraph(m_blackPic);
	DeleteGraph(m_whitePic);
	DeleteGraph(m_boxBlackPic);
	DeleteGraph(m_boxWhitePic);
	DeleteGraph(m_undoButtonPic);
	DeleteGraph(m_undoFramePic);
	DeleteGraph(m_undoButtonOffPic);
	DeleteGraph(m_stopButtonPic);
	DeleteSoundMem(m_gameSnd);
	DeleteSoundMem(m_pieceSnd);
	DeleteSoundMem(m_passSnd);
	DeleteSoundMem(m_undoSnd);
	DeleteSoundMem(m_stopSnd);
}