#include "Result.h"
#include "Share.h"

/**定数**/
//次へボタン
static const int NEXT_X = 100;
static const int NEXT_Y = 100;
static const int NEXT_WIDTH = 200;
static const int NEXT_HEIGHT = 200;
//もう一度ボタン
static const int RETRY_X = 300;
static const int RETRY_Y = 300;
static const int RETRY_WIDTH = 200;
static const int RETRY_HEIGHT = 200;

/**描画用変数**/
static bool onNext;
static bool onRetry;

/**音声判定用**/
static bool nowNext;
static bool nowRetry;

//コンストラクタ
Result::Result(SceneChanger *changer) 
	:BaseScene(changer)
{}

//デスストラクタ
Result::~Result() {
}

void Result::Initialize() {
	//画像のロード
	m_resultPic = LoadGraph("pic/Othello/result.png");
	//音声のロード
	m_resultSnd = LoadSoundMem("sound/Othello/result.mp3");
	m_nextSnd = LoadSoundMem("sound/Othello/next.mp3");
	m_retrySnd = LoadSoundMem("sound/Othello/start.mp3");

	//BGMを再生
	PlaySoundMem(m_resultSnd , DX_PLAYTYPE_LOOP , true);
}

void Result::Update() {
	//フラグのリセット
	onNext = false;
	onRetry = false;
	//マウス位置の取得
	int mousePosX;
	int mousePosY;
	int mouseInput = GetMouseInput();
	GetMousePoint(&mousePosX , &mousePosY);


	//次へボタン
	if (Share::isIn(NEXT_X , NEXT_Y , NEXT_WIDTH , NEXT_HEIGHT , mousePosX , mousePosY)) {
		onNext = true;
		//クリックされたら
		if (mouseInput & MOUSE_INPUT_LEFT) {
			m_sceneChanger->ChangeScene(Scene_Menu);
		}
	}

	//もう一度ボタン
	if (Share::isIn(RETRY_X, RETRY_Y, RETRY_WIDTH, RETRY_HEIGHT, mousePosX, mousePosY)) {
		onRetry = true;
		//クリックされたら
		if (mouseInput & MOUSE_INPUT_LEFT) {
			m_sceneChanger->ChangeScene(Scene_Game);
		}
	}
}

void Result::Draw() {
	//背景の表示
	DrawExtendGraph(0 , 0 ,WIN_SIZE_X ,WIN_SIZE_Y , m_resultPic , false);

	//結果の表示

	//もう一度ボタン
	SetFontSize(35);
	DrawBoxAA(RETRY_X , RETRY_Y , RETRY_X + RETRY_WIDTH , RETRY_Y +RETRY_HEIGHT , GetColor(190 ,190,190) ,true);
	int width = GetDrawStringWidth("RETRY" ,5 );
	int center_x = RETRY_X + RETRY_WIDTH / 2;
	DrawString(center_x - width / 2, RETRY_Y + 5, "START", GetColor(230, 230, 230));
	if (onRetry) {
		DrawBoxAA(RETRY_X, RETRY_Y, RETRY_X + RETRY_WIDTH, RETRY_Y + RETRY_HEIGHT, GetColor(101, 187, 233), false, 5.0);
	}

	//次へボタン
	SetFontSize(35);
	DrawBoxAA(NEXT_X, NEXT_Y, NEXT_X + NEXT_WIDTH, NEXT_Y + NEXT_HEIGHT, GetColor(190, 190, 190), true);
	width = GetDrawStringWidth("RETRY", 5);
	center_x = NEXT_X + NEXT_WIDTH / 2;
	DrawString(center_x - width / 2, NEXT_Y + 5, "START", GetColor(230, 230, 230));
	if (onNext) {
		DrawBoxAA(NEXT_X, NEXT_Y, NEXT_X + NEXT_WIDTH, NEXT_Y + NEXT_HEIGHT, GetColor(101, 187, 233), false, 5.0);
	}
}

void Result::Finalize() {
	//ロード画面の表示
	DrawExtendGraph(0, 0, WIN_SIZE_X + 5, WIN_SIZE_Y, Share::loadPic, true);
	//BGMを止める
	StopSoundMem(m_resultSnd);
	deleteMem();
}

void Result::deleteMem() {
	DeleteGraph(m_resultPic);
	DeleteSoundMem(m_resultSnd);
	DeleteSoundMem(m_nextSnd);
	DeleteSoundMem(m_retrySnd);
}