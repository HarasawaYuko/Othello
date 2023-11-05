#include "Game.h"
#include "Share.h"
#include "Mouse.h"
#include "TimeKeeper.h"
#include "AiMcts.h"
#include "AiRandom.h"
#include "AialphaBeta.h"

/***�萔***/
static const int TOP_MARGIN = 50;//�㑤�}�[�W��
static const int SIDE_MARGIN = 150;//���}�[�W��
static const int SQUARE_SIZE = 62;//�}�X�̑傫��
static const int PLAYOUT_NUM_LEVEL = 5000;//1���x�����Ƃ̃v���C�A�E�g
static const int UNDO_NUM = 3;//undo�ł��鐔
static const int TO_RESULT_TIME = 10;

//���\���֌W
static const int BOX_WIDTH = 120;
static const int BOX_HEIGHT = 120;
static const int BOX_BLACK_X = 15;
static const int BOX_WHITE_X = WIN_SIZE_X - BOX_BLACK_X - BOX_WIDTH;
static const int BOX_Y = 300;

//Undo�{�^���֌W
static const int UNDO_X = 730;
static const int UNDO_Y = 10;
static const int UNDO_WIDTH = 60;
static const int UNDO_HEIGHT = 45;

//�I���{�^���֌W
static const int STOP_X = 10;
static const int STOP_Y = 10;
static const int STOP_WIDTH = 60;
static const int STOP_HEIGHT = 45;

//���s�\��������
static       int WIN_WIDTH;
static       int LOSE_WIDTH;



//�`�攻��p
static bool onBoard;

//��������p
static bool nowPut;
static bool nowToMenu;
static bool nowPass;
static bool nowUndo;
static bool nowStop;

//��ʑJ�ڔ���p
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
	
	//AI��ݒ�
	switch (Share::ai) {
	case AI_MCTS:
		aiFunc = [](const State& state) {return mctsActionOthello(state, PLAYOUT_NUM_LEVEL * Share::level); };
		break;
	case AI_Alpha:
		aiFunc = [](const State& state) {return alphaBetaAction(state, Share::level); };
		break;
	}

	//�摜�̃��[�h
	m_blackPic = LoadGraph("pic/black.png");
	m_whitePic = LoadGraph("pic/white.png");
	m_gamePic = LoadGraph("pic/game.png");
	m_boxBlackPic = LoadGraph("pic/boxBlack.png");
	m_boxWhitePic = LoadGraph("pic/boxWhite.png");
	m_undoPic = LoadGraph("pic/undo.png");
	m_undoOffPic = LoadGraph("pic/notUndo.png");
	m_undoOnPic = LoadGraph("pic/undoOn.png");
	m_stopPic = LoadGraph("pic/fin.png");
	m_stopOnPic = LoadGraph("pic/finOn.png");

	//�����̃��[�h
	m_pieceSnd = LoadSoundMem("sound/piece.mp3");
	m_gameSnd = LoadSoundMem("sound/game.mp3");
	m_passSnd = LoadSoundMem("sound/pass.mp3");
	m_undoSnd = LoadSoundMem("sound/undo.mp3");
	m_stopSnd = LoadSoundMem("sound/stop.mp3");

	//�{�^���̍쐬
	m_undoButton = Button(m_undoPic , m_undoOnPic ,UNDO_X , UNDO_Y , UNDO_WIDTH , UNDO_HEIGHT , m_undoOffPic);
	m_stopButton = Button(m_stopPic , m_stopOnPic ,STOP_X , STOP_Y , STOP_WIDTH , STOP_HEIGHT);

	//�������v�Z
	SetFontSize(50);
	WIN_WIDTH = GetDrawStringWidth("WIN!", 4);
	LOSE_WIDTH = GetDrawStringWidth("LOSE", 4);


	//���ʐݒ�
	ChangeVolumeSoundMem(110, m_gameSnd);

	//BGM���Đ�
	PlaySoundMem(m_gameSnd, DX_PLAYTYPE_LOOP, true);
}


void Game::Update() {
	//�t���O�̃��Z�b�g
	nowPut = false;
	nowPass = false;
	nowUndo = false;

	//�}�E�X�ʒu�̎擾
	Mouse::instance()->update();

	//�{�[�h��Ƀ}�E�X�����邩�擾
	setSelectSquare(Mouse::instance()->getX(), Mouse::instance()->getY());
	if (m_selectSquare == 0) {
		onBoard = false;
	}
	else {
		onBoard = true;
	}

	//������
	if (!m_state.isDone()) {
		//�v���C���[�̎��
		if (m_state.turn == Share::playerColor) {
			//�u����Ƃ��낪�Ȃ���΃p�X
			if (m_state.isCanPutAll() == 0) {
				m_state.advance(0);
				nowPass = true;
			}
			//�u��
			else if (Mouse::instance()->getClickNow(LEFT_CLICK) && m_state.isCanPut(m_selectSquare)) {
				//�u���O��
				undo_vec.push_back(std::make_pair(m_state, m_recentPut));
				if (undo_vec.size() > UNDO_NUM) {
					//�擪����L�^���폜
					undo_vec.erase(undo_vec.begin());
				}
				m_state.advance(m_selectSquare);
				nowPut = true;
				m_recentPut = m_selectSquare;
			}
		}
		//AI�̎��
		else {
			uint64_t tmp = aiFunc(m_state);
			m_state.advance(tmp);
			if (tmp != 0) {
				m_recentPut = tmp;
				nowPut = true;
			}
			else {
				nowPass = true;
			}
		}
	}
	//�����I����
	else {
		//�^�C�}�[���N�����Ă��Ȃ����
		if (!timer) {
			timerToResult = TimeKeeper(TO_RESULT_TIME);
			timer = true;
		}
		if (timerToResult.isTimeOver()) {
			m_sceneChanger->ChangeScene(Scene_Result);
		}
	}

	//Undo�{�^��
	//��Ԃ��ۑ�����ē��Ȃ����
	if (undo_vec.size() == 0) m_undoButton.turnOff();
	else m_undoButton.turnOn();
	m_undoButton.update(&nowUndo);
	if (nowUndo) {
		m_state = undo_vec.back().first;
		m_recentPut = undo_vec.back().second;
		undo_vec.erase(undo_vec.end() - 1);
	}

	//�I���{�^��
	m_stopButton.update(&nowStop);
	if (nowStop) {
		m_sceneChanger->ChangeScene(Scene_Menu);
		nowToMenu = true;
	}
}

void Game::Draw() {
	//�w�i
	DrawExtendGraph(0, 0, WIN_SIZE_X, WIN_SIZE_Y, m_gamePic, true);

	//�{�[�h
	int x_pos = SIDE_MARGIN;
	int y_pos = TOP_MARGIN;
	for (int x = 0; x < BOARD_SIZE; x++) {
		y_pos = TOP_MARGIN;
		for (int y = 0; y < BOARD_SIZE; y++) {
			int index = x + y * BOARD_SIZE;
			switch (m_state.getPiece(HIGHEST >> index)) {
			case CANPUT:
				DrawBoxAA((float)(x_pos + 1), (float)(y_pos + 1), (float)(x_pos + SQUARE_SIZE - 1), (float)(y_pos + SQUARE_SIZE - 1), COLOR_ORANGE, true, 2.0f);
				break;
			case WHITE:
				DrawBoxAA((float)(x_pos + 1), (float)(y_pos + 1), (float)(x_pos + SQUARE_SIZE - 1), (float)(y_pos + SQUARE_SIZE - 1), COLOR_GREEN, true, 2.0f);
				DrawExtendGraph(x_pos, y_pos, x_pos + SQUARE_SIZE, y_pos + SQUARE_SIZE, m_whitePic, true);
				break;
			case BLACK:
				DrawBoxAA((float)(x_pos + 1), (float)(y_pos + 1), (float)(x_pos + SQUARE_SIZE - 1), (float)(y_pos + SQUARE_SIZE - 1), COLOR_GREEN, true, 2.0f);
				DrawExtendGraph(x_pos, y_pos, x_pos + SQUARE_SIZE, y_pos + SQUARE_SIZE, m_blackPic, true);
				break;
			default:
				DrawBoxAA((float)(x_pos + 1), (float)(y_pos + 1), (float)(x_pos + SQUARE_SIZE - 1), (float)(y_pos + SQUARE_SIZE - 1), COLOR_GREEN, true, 2.0f);
			};
			y_pos += SQUARE_SIZE;
		}
		x_pos += SQUARE_SIZE;
	}
	


	//�I��g�\��
	if (onBoard && m_state.turn == Share::playerColor) {
		int x , y;
		getCoord(&x ,&y  , m_selectSquare);
		DrawBoxAA((float)(SIDE_MARGIN + (x * SQUARE_SIZE) + 1), (float)(TOP_MARGIN + (y * SQUARE_SIZE) + 1), (float)(SIDE_MARGIN + ((x + 1) * SQUARE_SIZE) - 1), (float)(TOP_MARGIN + ((y + 1) * SQUARE_SIZE) - 1), COLOR_BLUE, false, 5.0f);
	}
	//�u�����ꏊ
	if (m_recentPut != 0) {
		int x, y;
		getCoord(&x, &y, m_recentPut);
		DrawCircleAA((float)(SIDE_MARGIN + SQUARE_SIZE * x + SQUARE_SIZE / 2 - 3), (float)(TOP_MARGIN + SQUARE_SIZE * y + SQUARE_SIZE / 2 - 4), 10, 15, COLOR_LBLUE, true, 0);
	}

	//���\��
	DrawExtendGraph(BOX_BLACK_X, BOX_Y, BOX_BLACK_X + BOX_WIDTH, BOX_Y + BOX_HEIGHT, m_boxBlackPic, true);
	DrawExtendGraph(BOX_WHITE_X, BOX_Y, BOX_WHITE_X + BOX_WIDTH, BOX_Y + BOX_HEIGHT, m_boxWhitePic, true);
	DrawExtendGraph(BOX_BLACK_X + 2, BOX_Y + 2, BOX_BLACK_X + 52, BOX_Y + 52, m_blackPic, true);
	DrawExtendGraph(BOX_WHITE_X + 2, BOX_Y + 2, BOX_WHITE_X + 52, BOX_Y + 52, m_whitePic, true);

	SetFontSize(50);
	std::string black = std::to_string(m_state.getNum(BLACK));
	std::string white = std::to_string(m_state.getNum(WHITE));
	int blackWidth = GetDrawStringWidth(black.c_str(), (int)black.length());
	int whiteWidth = GetDrawStringWidth(white.c_str(), (int)white.length());
	DrawFormatString(BOX_BLACK_X + BOX_WIDTH - 5 - blackWidth, BOX_Y + BOX_HEIGHT - 55, COLOR_WHITE, "%s", black.c_str());
	DrawFormatString(BOX_WHITE_X + BOX_WIDTH - 5 - whiteWidth, BOX_Y + BOX_HEIGHT - 55, COLOR_BlACK, "%s", white.c_str());

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
	DrawString(BOX_BLACK_X + 50, BOX_Y + 40, blackStr.c_str(), COLOR_WHITE);
	DrawString(BOX_WHITE_X + 50, BOX_Y + 40, whiteStr.c_str(), COLOR_BlACK);

	////�Q�[���I�����̕\��
	if (m_state.isDone()) {
		SetFontSize(50);
		if (m_state.getBlackPlayerStatus() == LOSE) {
			DrawString((int)((WIN_CENT_X - SQUARE_SIZE * 4) / 2 - LOSE_WIDTH/2), (int)(WIN_SIZE_Y / 2 + 150), "LOSE", COLOR_LBLUE);
			DrawString((int)(WIN_SIZE_X - WIN_CENT_X / 2 + SQUARE_SIZE * 2 - WIN_WIDTH / 2), (int)(WIN_SIZE_Y / 2 + 150), "WIN!", COLOR_RED);
		}
		else if (m_state.getBlackPlayerStatus() == WIN) {
			DrawString((int)((WIN_CENT_X - SQUARE_SIZE * 4) / 2 - WIN_WIDTH/2), (int)(WIN_SIZE_Y / 2 + 150), "WIN!", COLOR_RED);
			DrawString((int)(WIN_SIZE_X - WIN_CENT_X / 2 + SQUARE_SIZE * 2 - LOSE_WIDTH / 2), (int)(WIN_SIZE_Y / 2 + 150), "LOSE", COLOR_LBLUE);
		}
	}
	else {
		//�^�[���\��
		if (m_state.turn == BLACK) {
			DrawBoxAA(BOX_BLACK_X - 4, BOX_Y - 4, BOX_BLACK_X + BOX_WIDTH + 4, BOX_Y + BOX_HEIGHT + 4, COLOR_LBLUE, false, 8);
		}
		else {
			DrawBoxAA(BOX_WHITE_X - 4, BOX_Y - 4, BOX_WHITE_X + BOX_WIDTH + 4, BOX_Y + BOX_HEIGHT + 4, COLOR_LBLUE, false, 8);
		}
	}

	//undo�{�^��
	m_undoButton.draw();

	//stop�{�^��
	m_stopButton.draw();

	//����
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
	//���[�h��ʂ̕\��
	DrawExtendGraph(0, 0, WIN_SIZE_X + 5, WIN_SIZE_Y , Share::loadPic, true);
	//BGM���~�߂�
	StopSoundMem(m_gameSnd);
	//���j���[��ʑJ�ڂȂ�SE
	if (nowToMenu) {
		PlaySoundMem(m_stopSnd , DX_PLAYTYPE_NORMAL, true);
	}

	//������
	m_recentPut = 0;

	//���҂̋L�^
	Share::playerStatus = m_state.getWinningStatus(Share::playerColor);
	Share::blackNum = m_state.getNum(BLACK);
	Share::whiteNum = m_state.getNum(WHITE);
	//�n���h���̉��
	deleteMem();
}

//�摜�A�����̃��������
void Game::deleteMem() {
	DeleteGraph(m_gamePic);
	DeleteGraph(m_blackPic);
	DeleteGraph(m_whitePic);
	DeleteGraph(m_boxBlackPic);
	DeleteGraph(m_boxWhitePic);
	DeleteGraph(m_undoPic);
	DeleteGraph(m_undoOnPic);
	DeleteGraph(m_undoOffPic);
	DeleteGraph(m_stopPic);
	DeleteGraph(m_stopOnPic);
	DeleteSoundMem(m_gameSnd);
	DeleteSoundMem(m_pieceSnd);
	DeleteSoundMem(m_passSnd);
	DeleteSoundMem(m_undoSnd);
	DeleteSoundMem(m_stopSnd);
}

//�I������Ă���}�X���擾��set����
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

//board�̃}�X�̍��p��xp yp�̐�ɋL�^����
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