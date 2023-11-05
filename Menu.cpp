#include "Menu.h"
#include "Share.h"
#include "Mouse.h"

//ターン選択ラジオボタン関係
static const int TURN_RADIO_Y = 230;
static const int TURN_RADIO_X = 200;
static const int TURN_RADIO_SIZE = 20;
static const int TURN_RADIO_SELECT_SIZE = 13;
static const int TURN_RADIO_SPACE = 210;

//AI関係
static const int AI_LEVEL_MAX[] = { 10 , 7 };
static       int AI_INDEX = 0;
static const int AI_NUM = 2;

//Ai選択スワイプボタン関係
static const int SWIPE_Y = 310;
static const int SWIPE_X = 200;
static const int SWIPE_WIDTH = 65;
static const int SWIPE_HEIGHT = 65;
static const int SWIPE_SPACE = 400 - SWIPE_WIDTH;
static const int AI_STR_SIZE = 40;
static       int AI_STR_X;
static const int AI_STR_Y = SWIPE_Y + (SWIPE_HEIGHT/2) - (AI_STR_SIZE/2);

//開始ボタン関係
static const int START_BUTTON_WIDTH = 210;
static const int START_BUTTON_HEIGHT = 70;
static const int START_BUTTON_X = WIN_CENT_X - START_BUTTON_WIDTH/2;
static const int START_BUTTON_Y = 460;


//サイドバー関係
static const int BAR_X = 180;
static const int BAR_Y = 420;
static const int BAR_WIDTH = 350;
static const int BAR_HEIGHT = 6;
static const int BAR_SELECT_SIZE = 13;


//閉じるボタン関係
static const int CLOSE_X = 730;
static const int CLOSE_Y = 10;
static const int CLOSE_WIDTH = 60;
static const int CLOSE_HEIGHT = 45;

Menu::Menu(SceneChanger *changer) 
	:BaseScene(changer)
{}

Menu::~Menu(){}

void Menu::Initialize() {
	//手番の初期化
	Share::playerColor = BLACK;
	Share::level = 1;

	//画像のロード
	m_titlePic = LoadGraph("pic/Title.png");
	m_closeButtonPic = LoadGraph("pic/fin.png");
	m_rightSwipePic = LoadGraph("pic/rightSwipe.png");
	m_leftSwipePic = LoadGraph("pic/leftSwipe.png");
	m_rightSwipeSelectPic = LoadGraph("pic/rightSwipeSelect.png");
	m_leftSwipeSelectPic = LoadGraph("pic/leftSwipeSelect.png");
	m_startPic = LoadGraph("pic/start.png");
	m_startOnPic = LoadGraph("pic/startOn.png");
	m_closePic = LoadGraph("pic/fin.png");
	m_closeOnPic = LoadGraph("pic/finOn.png");


	//ボタン作成
	m_startButton = Button( m_startPic, m_startOnPic,START_BUTTON_X , START_BUTTON_Y , START_BUTTON_WIDTH , START_BUTTON_HEIGHT );
	m_closeButton = Button(m_closePic, m_closeOnPic, CLOSE_X, CLOSE_Y, CLOSE_WIDTH, CLOSE_HEIGHT);
	m_swipeLeftButton = Button(m_leftSwipePic , m_leftSwipeSelectPic , SWIPE_X ,SWIPE_Y , SWIPE_WIDTH , SWIPE_HEIGHT);
	m_swipeRightButton = Button(m_rightSwipePic, m_rightSwipeSelectPic, SWIPE_X + SWIPE_SPACE, SWIPE_Y, SWIPE_WIDTH, SWIPE_HEIGHT);
	m_turnButton = RadioButton(TURN_RADIO_X, TURN_RADIO_Y, TURN_RADIO_SIZE,TURN_RADIO_SPACE, 2, 35, {"BLACK" ,"WHITE"});
	m_levelBar = SlideBar(BAR_X , BAR_Y , BAR_WIDTH ,BAR_SELECT_SIZE);

	//音声のロード
	m_menuSnd = LoadSoundMem("sound/menu.mp3");
	m_startSnd = LoadSoundMem("sound/start.mp3");
	m_radioSnd = LoadSoundMem("sound/radio.mp3");
	m_sideSnd = LoadSoundMem("sound/side.mp3");
	m_swipeSnd = LoadSoundMem("sound/swipe.mp3");

	//音量設定
	ChangeVolumeSoundMem(130, m_menuSnd);

	//BGMを再生
	PlaySoundMem(m_menuSnd, DX_PLAYTYPE_LOOP, true);
}

void Menu::Update() {
	//フラグのリセット
	m_nowStart = false;
	m_nowRadio = false;
	m_nowSlide = false;
	m_nowSwipe = false;
	m_nowClose = false;

	//マウス位置の取得
	Mouse::instance()->update();

	m_turnButton.update(&m_nowRadio);
	if (m_nowRadio) {
		Share::playerColor = (piece)m_turnButton.getSelect();
	}

	//AI選択用スワイプボタン
	m_swipeLeftButton.update(&m_nowSwipe);
	if (m_nowSwipe) {
		AI_INDEX--;
		Share::level = (int)(m_levelBar.getRate() * AI_LEVEL_MAX[AI_INDEX]) + 1;
		if (AI_INDEX < 0) AI_INDEX = AI_NUM - 1;
	}
	else{
		m_swipeRightButton.update(&m_nowSwipe);
		if (m_nowSwipe) {
			AI_INDEX = (AI_INDEX + 1) % AI_NUM;
			Share::level = (int)(m_levelBar.getRate() * AI_LEVEL_MAX[AI_INDEX]) + 1;
		}
	}

	//サイドバー
	//レベルの設定
	m_levelBar.update(&m_nowSlide);
	if (m_nowSlide) {
		int beforeLevel = Share::level;
		Share::level = (int)(m_levelBar.getRate() * AI_LEVEL_MAX[AI_INDEX]) + 1;
		if (beforeLevel == Share::level) {
			m_nowSlide = false;
		}
	}

	//開始ボタン
	m_startButton.update(&m_nowStart);
	if (m_nowStart) {
		m_sceneChanger->ChangeScene(Scene_Game);
	}

	//閉じるボタン
	m_closeButton.update(&m_nowClose);
	if (m_nowClose) {
		m_sceneChanger->ChangeScene(Scene_Fin);
	}
}

void Menu::Draw() {
	DrawExtendGraph(0, 0, WIN_SIZE_X, WIN_SIZE_Y, m_titlePic, true);
	//手番選択用ラジオボタン
	m_turnButton.draw();

	//AI選択表示
	m_swipeLeftButton.draw();
	m_swipeRightButton.draw();

	SetFontSize(AI_STR_SIZE);
	int width = GetDrawStringWidth(Share::AI[AI_INDEX].c_str() , (int)Share::AI[AI_INDEX].size());
	AI_STR_X = WIN_SIZE_X / 2 - width / 2;
	DrawString(AI_STR_X , AI_STR_Y , Share::AI[AI_INDEX].c_str() , COLOR_BlACK);

	//難易度調整バー
	m_levelBar.draw();
	SetFontSize(35);
	DrawFormatString(BAR_X + BAR_WIDTH + 95, BAR_Y - 20, COLOR_BlACK, "%d", Share::level);
	SetFontSize(20);
	DrawFormatString(BAR_X + BAR_WIDTH + 20, BAR_Y - 8, COLOR_BlACK, "LEVEL:");

	//開始ボタン
	m_startButton.draw();

	//閉じるボタン
	m_closeButton.draw();

	//音声
	if (m_nowRadio) {
		PlaySoundMem(m_radioSnd, DX_PLAYTYPE_NORMAL, true);
	}
	if (m_nowSlide) {
		PlaySoundMem(m_sideSnd, DX_PLAYTYPE_NORMAL, true);
	}
	if (m_nowSwipe) {
		PlaySoundMem(m_swipeSnd, DX_PLAYTYPE_NORMAL, true);

	}
}

void Menu::Finalize() {
	//ロード画面の表示
	DrawExtendGraph(0, 0, WIN_SIZE_X+5, WIN_SIZE_Y, Share::loadPic, true);
	//ゲーム画面遷移ならSE
	if (m_nowStart) {
		PlaySoundMem(m_startSnd, DX_PLAYTYPE_NORMAL, true);
	}
	//BGMを止める
	StopSoundMem(m_menuSnd);
	//ハンドルの解放
	deleteMem();

	//AIの設定
	Share::ai = (Ai)AI_INDEX;
}

void Menu::deleteMem() {
	DeleteGraph(m_titlePic);
	DeleteGraph(m_closeButtonPic);
	DeleteGraph(m_rightSwipePic);
	DeleteGraph(m_rightSwipeSelectPic);
	DeleteGraph(m_leftSwipePic);
	DeleteGraph(m_leftSwipeSelectPic);
	DeleteGraph(m_startPic);
	DeleteGraph(m_startOnPic);
	DeleteGraph(m_closePic);
	DeleteGraph(m_closeOnPic);
	DeleteSoundMem(m_menuSnd);
	DeleteSoundMem(m_startSnd);
	DeleteSoundMem(m_radioSnd);
	DeleteSoundMem(m_sideSnd);
	DeleteSoundMem(m_swipeSnd);
}