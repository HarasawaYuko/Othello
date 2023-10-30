#include "Result.h"
#include "Share.h"
#include "Mouse.h"

/**定数**/
//AI表示
static       int AI_X;
static const int AI_Y = 70;
static const int AI_LETTER_SIZE = 50;

//レベル表示
static const int LEVEL_STR_LETTER_SIZE = 30;
static       int LEVEL_STR_X;
static const int LEVEL_STR_Y = 130;
static const int LEVEL_LETTER_SIZE = 35;
static       int LEVEL_X;
static       int LEVEL_Y;
//試合結果表示
static const int RESULT_LETTER_SIZE = 90;
static       int RESULT_X;
static const int RESULT_Y = 180;
static std::string RESULT_STR;
static       int RESULT_COLOR;

//個数表示
static const int NUM_SIZE = 60;
static       int NUM_X;
static const int NUM_Y = 290;

//ボタン
static const int BUTTON_X = 250;
static const int BUTTON_WIDTH = 300;
static const int BUTTON_HEIGHT = 70;
//次へボタン
static const int NEXT_Y = 375;
//もう一度ボタン
static const int RETRY_Y = 460;
//ボタンに表示する文字
static const int BUTTON_LETTER_SIZE = 40;
static       int NEXT_LETTER_X;
static       int RETRY_LETTER_X;
static const int BUTTON_LETTER_Y = (BUTTON_HEIGHT/2) - BUTTON_LETTER_SIZE/2;

//コンストラクタ
Result::Result(SceneChanger *changer) 
	:BaseScene(changer)
{
}

//デスストラクタ
Result::~Result() {
}

void Result::Initialize() {
	//BGMを再生
	m_resultSnd = LoadSoundMem("sound/Othello/result.mp3");
	ChangeVolumeSoundMem(130, m_resultSnd);
	PlaySoundMem(m_resultSnd, DX_PLAYTYPE_LOOP, true);

	//画像のロード
	m_resultPic = LoadGraph("pic/Othello/result.png");
	nextPic = LoadGraph("pic/Othello/next.png");
	nextOnPic = LoadGraph("pic/Othello/nextOn.png");
	retryPic = LoadGraph("pic/Othello/retry.png");
	retryOnPic = LoadGraph("pic/Othello/retryOn.png");
	
	//音声のロード
	m_nextSnd = LoadSoundMem("sound/Othello/next.mp3");
	m_retrySnd = LoadSoundMem("sound/Othello/start.mp3");
	//音量設定
	ChangeVolumeSoundMem(150, m_nextSnd);
	ChangeVolumeSoundMem(150, m_retrySnd);

	//AI表示位置
	SetFontSize(AI_LETTER_SIZE);
	AI_X = WIN_SIZE_X / 2 - GetDrawStringWidth(Share::AI[(int)Share::ai].c_str() , Share::AI[(int)Share::ai].size())/2;

	//結果表示位置計算
	switch (Share::playerStatus) {
	case WIN:
		RESULT_COLOR = GetColor(0xE8, 0x14, 0x14);
		RESULT_STR = "WIN";
		break;
	case LOSE:
		RESULT_COLOR = GetColor(0x34, 0x21, 0x87);
		RESULT_STR = "LOSE";
		break;
	case DRAW:
		RESULT_COLOR = GetColor(50, 50, 50);
		RESULT_STR = "DRAW";
		break;
	case NONE:
		RESULT_COLOR = GetColor(50, 50, 50);
		RESULT_STR = "DRAW";
		break;
	}
	SetFontSize(RESULT_LETTER_SIZE);
	RESULT_X = WIN_SIZE_X / 2 - GetDrawStringWidth(RESULT_STR.c_str() , RESULT_STR.size())/2;

	//レベル表示位置の計算
	SetFontSize(LEVEL_STR_LETTER_SIZE);
	int levelStrWidth = GetDrawStringWidth("LEVEL:", 6);
	SetFontSize(LEVEL_LETTER_SIZE);
	int levelWidth = GetDrawStringWidth(std::to_string(Share::level).c_str(), std::to_string(Share::level).size());
	LEVEL_STR_X = WIN_SIZE_X / 2 - (levelStrWidth + levelWidth) / 2;
	LEVEL_X = LEVEL_STR_X + levelStrWidth + 5;
	LEVEL_Y = LEVEL_STR_Y - (LEVEL_LETTER_SIZE - LEVEL_STR_LETTER_SIZE) + 2;

	//個数表示位置
	SetFontSize(NUM_SIZE);
	int blackWidth = GetDrawStringWidth(std::to_string(Share::blackNum).c_str(), std::to_string(Share::blackNum).size());
	int whiteWidth = GetDrawStringWidth(std::to_string(Share::whiteNum).c_str(), std::to_string(Share::whiteNum).size());
	NUM_X = WIN_SIZE_X / 2 - (blackWidth + whiteWidth + GetDrawStringWidth(" - ", 3)) / 2;

	//ボタン位置の計算
	SetFontSize(BUTTON_LETTER_SIZE);
	NEXT_LETTER_X = (BUTTON_WIDTH / 2) - (GetDrawStringWidth("NEXT", 4) / 2);
	RETRY_LETTER_X = (BUTTON_WIDTH / 2) - (GetDrawStringWidth("RETRY", 5) / 2);

	//ボタン
	nextButton = Button(nextPic, nextOnPic, BUTTON_X, NEXT_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	retryButton = Button(retryPic, retryOnPic, BUTTON_X, RETRY_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
}

void Result::Update() {
	//フラグのリセット
	onNext = false;
	onRetry = false;
	//マウス位置の取得
	Mouse::instance()->update();

	//次へボタン
	//if (isIn(BUTTON_X , NEXT_Y , BUTTON_WIDTH , BUTTON_HEIGHT)) {
	//	onNext = true;
	//	//クリックされたら
	//	if (mouseInput & MOUSE_INPUT_LEFT) {
	//		nowNext = true;
	//		m_sceneChanger->ChangeScene(Scene_Menu);
	//	}
	//}
	nextButton.update(&nowNext);
	if (nowNext) {
		m_sceneChanger->ChangeScene(Scene_Menu);
	}

	//もう一度ボタン
	//if (isIn(BUTTON_X, RETRY_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
	//	onRetry = true;
	//	//クリックされたら
	//	if (mouseInput & MOUSE_INPUT_LEFT) {
	//		nowRetry = true;
	//		m_sceneChanger->ChangeScene(Scene_Game);
	//	}
	//}
	retryButton.update(&nowRetry);
	if (nowRetry) {
		m_sceneChanger->ChangeScene(Scene_Game);
	}
}

void Result::Draw() {
	int width; //文字列の幅保存用

	//背景の表示
	DrawExtendGraph(0 , 0 ,WIN_SIZE_X ,WIN_SIZE_Y , m_resultPic , false);

	//レベルの表示
	SetFontSize(AI_LETTER_SIZE);
	DrawString(AI_X, AI_Y, Share::AI[(int)Share::ai].c_str(), GetColor(30, 30, 30));

	//レベルの表示
	SetFontSize(LEVEL_STR_LETTER_SIZE);
	DrawString(LEVEL_STR_X ,LEVEL_STR_Y , "LEVEL:" ,GetColor(30 , 30 ,30));
	SetFontSize(LEVEL_LETTER_SIZE);
	DrawFormatString(LEVEL_X , LEVEL_Y , GetColor(30 , 30 ,30) ,"%d" , Share::level);

	//結果の表示
	SetFontSize(RESULT_LETTER_SIZE);
	DrawString(RESULT_X, RESULT_Y, RESULT_STR.c_str(), RESULT_COLOR);

	//個数表示
	SetFontSize(NUM_SIZE);
	DrawFormatString(NUM_X , NUM_Y , GetColor(30 ,30 , 30) , "%d - %d" ,Share::blackNum , Share::whiteNum);

	/** ボタン **/
	SetFontSize(BUTTON_LETTER_SIZE);

	//次へボタン
	/*DrawBoxAA(BUTTON_X, NEXT_Y, BUTTON_X + BUTTON_WIDTH, NEXT_Y + BUTTON_HEIGHT, COLOR_BLUE, true);
	DrawString(BUTTON_X + NEXT_LETTER_X, NEXT_Y + BUTTON_LETTER_Y, "NEXT", GetColor(230, 230, 230));
	if (onNext) {
		DrawBoxAA(BUTTON_X, NEXT_Y, BUTTON_X + BUTTON_WIDTH, NEXT_Y + BUTTON_HEIGHT, GetColor(101, 187, 233), false, 5.0);
	}*/
	nextButton.draw();

	//もう一度ボタン
	/*DrawBoxAA(BUTTON_X, RETRY_Y, BUTTON_X + BUTTON_WIDTH, RETRY_Y + BUTTON_HEIGHT, COLOR_BLUE, true);
	DrawString(BUTTON_X + RETRY_LETTER_X, RETRY_Y + BUTTON_LETTER_Y, "RETRY", GetColor(230, 230, 230));
	if (onRetry) {
		DrawBoxAA(BUTTON_X, RETRY_Y, BUTTON_X + BUTTON_WIDTH, RETRY_Y + BUTTON_HEIGHT, GetColor(101, 187, 233), false, 5.0);
	}*/
	retryButton.draw();
}

void Result::Finalize() {
	//ロード画面の表示
	DrawExtendGraph(0, 0, WIN_SIZE_X + 5, WIN_SIZE_Y, Share::loadPic, true);
	//音声
	if (nowNext) {
		PlaySoundMem(m_nextSnd, DX_PLAYTYPE_NORMAL, true);
	}
	else if (nowRetry) {
		PlaySoundMem(m_retrySnd , DX_PLAYTYPE_NORMAL, true);
	}
	//BGMを止める
	StopSoundMem(m_resultSnd);
	//メモリの削除
	deleteMem();
}

void Result::deleteMem() {
	DeleteGraph(m_resultPic);
	DeleteSoundMem(m_resultSnd);
	DeleteSoundMem(m_nextSnd);
	DeleteSoundMem(m_retrySnd);
}