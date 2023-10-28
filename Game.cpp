#include "Game.h"
#include "Share.h"
#include "Mouse.h"
#include "TimeKeeper.h"
#include "AiMcts.h"
#include "AiRandom.h"
#include "AialphaBeta.h"
#include "UIMaterial.h"

/***定数***/
static const int TOP_MARGIN = 50;//上側マージン
static const int SIDE_MARGIN = 150;//横マージン
static const int SQUARE_SIZE = 62;//マスの大きさ
static const int PLAYOUT_NUM_LEVEL = 10000;//1レベルごとのプレイアウト
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
	m_state = State();
	timer = false;
	
	//AIを設定
	switch (Share::ai) {
	case AI_MCTS:
		aiFunc = [](const State& state) {return mctsActionOthello(state, PLAYOUT_NUM_LEVEL * Share::level); };
		break;
	case AI_Alpha:
		aiFunc = [](const State& state) {return alphaBetaAction(state, Share::level); };
		break;
	}

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
	Mouse::instance()->update();

	setSelectSquare(Mouse::instance()->getX(), Mouse::instance()->getY());
	if (m_selectSquare == 0) {
		onBoard = false;
	}
	else {
		onBoard = true;
	}
	if (!m_state.isDone()) {
		//プレイヤーの手番
		if (m_state.turn == Share::playerColor) {
			//パス
			if (m_state.getLegalBoard() == 0) {
				m_state.advance(0);
				nowPass = true;
			}
			//置く
			else if (Mouse::instance()->getClickNow(LEFT_CLICK) && m_state.isCanPut(m_selectSquare)) {
				//置く前に
				undo_vec.push_back(std::make_pair(m_state, m_recentPut));
				if (undo_vec.size() > UNDO_NUM) {
					//先頭から削除
					undo_vec.erase(undo_vec.begin());
				}
				m_state.advance(m_selectSquare);
				nowPut = true;
				m_recentPut = m_selectSquare;
			}
		}
		//AIの手番
		else {
			TimeKeeper tk_think = TimeKeeper(THINK_TIME);
			uint64_t tmp = aiFunc(m_state);
			m_state.advance(tmp);
			if (tmp != 0) {
				m_recentPut = tmp;
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

	if (isIn(UNDO_X, UNDO_Y, UNDO_X + UNDO_WIDTH, UNDO_Y + UNDO_HEIGHT)) {
		onUndo = true;
		if (Mouse::instance()->getClickNow(LEFT_CLICK) && undo_vec.size() > 0) {
			m_state = undo_vec.back().first;
			m_recentPut = undo_vec.back().second;
			nowUndo = true;
			undo_vec.erase(undo_vec.end() - 1);
		}
	}
	//終了ボタン
	if (isIn(STOP_X, STOP_Y, STOP_X + STOP_WIDTH, STOP_Y + STOP_HEIGHT)) {
		onStop = true;
		if (Mouse::instance()->getClickNow(LEFT_CLICK)) {
			m_sceneChanger->ChangeScene(Scene_Menu);
			nowToMenu = true;
		}
	}
}

void Game::Draw() {
	//背景
	DrawExtendGraph(0, 0, WIN_SIZE_X, WIN_SIZE_Y, m_gamePic, true);

	//ボード
	int x_pos = SIDE_MARGIN;
	int y_pos = TOP_MARGIN;
	for (int x = 0; x < BOARD_SIZE; x++) {
		y_pos = TOP_MARGIN;
		for (int y = 0; y < BOARD_SIZE; y++) {
			int index = x + y * BOARD_SIZE;
			switch (m_state.getPiece(HIGHEST >> index)) {
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
		int x , y;
		getCoord(&x ,&y  , m_selectSquare);
		DrawBoxAA((float)(SIDE_MARGIN + (x * SQUARE_SIZE) + 1), (float)(TOP_MARGIN + (y * SQUARE_SIZE) + 1), (float)(SIDE_MARGIN + ((x + 1) * SQUARE_SIZE) - 1), (float)(TOP_MARGIN + ((y + 1) * SQUARE_SIZE) - 1), COLOR_BLUE, false, 5.0f);
	}
	//置いた場所
	if (m_recentPut != 0) {
		int x, y;
		getCoord(&x, &y, m_recentPut);
		DrawCircleAA((float)(SIDE_MARGIN + SQUARE_SIZE * x + SQUARE_SIZE / 2 - 3), (float)(TOP_MARGIN + SQUARE_SIZE * y + SQUARE_SIZE / 2 - 4), 10, 15, COLOR_LBLUE, true, 0);
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

	//初期化
	m_recentPut = 0;

	//勝者の記録
	Share::playerStatus = m_state.getWinningStatus(Share::playerColor);
	Share::blackNum = m_state.getNum(BLACK);
	Share::whiteNum = m_state.getNum(WHITE);
	//ハンドルの解放
	deleteMem();
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

void Game::setSelectSquare(const int x , const int y) {
	int pointX = (x - SIDE_MARGIN) / SQUARE_SIZE;
	int pointY = (y - TOP_MARGIN) / SQUARE_SIZE;
	
	if (0 <= pointX && pointX <BOARD_SIZE && 0 <= pointY && pointY < BOARD_SIZE) {
		m_selectSquare = HIGHEST >> (pointX + pointY * BOARD_SIZE);
	}
	else {
		m_selectSquare = 0;
	}
}

void Game::getCoord(int* xp, int* yp, const uint64_t board)const {
	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			if (((HIGHEST >> (x + y * BOARD_SIZE)) & board) != 0) {
				*xp = x;
				*yp = y;
				return;
			}
		}
	}
	return;
}