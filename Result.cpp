#include "Result.h"
#include "Share.h"
#include "Mouse.h"

/**�萔**/
//AI�\��
static       int AI_X;
static const int AI_Y = 70;
static const int AI_LETTER_SIZE = 50;

//���x���\��
static const int LEVEL_STR_LETTER_SIZE = 30;
static       int LEVEL_STR_X;
static const int LEVEL_STR_Y = 130;
static const int LEVEL_LETTER_SIZE = 35;
static       int LEVEL_X;
static       int LEVEL_Y;
//�������ʕ\��
static const int RESULT_LETTER_SIZE = 90;
static       int RESULT_X;
static const int RESULT_Y = 180;
static std::string RESULT_STR;
static       int RESULT_COLOR;

//���\��
static const int NUM_SIZE = 60;
static       int NUM_X;
static const int NUM_Y = 290;

//�{�^��
static const int BUTTON_X = 250;
static const int BUTTON_WIDTH = 300;
static const int BUTTON_HEIGHT = 70;
//���փ{�^��
static const int NEXT_Y = 375;
//������x�{�^��
static const int RETRY_Y = 460;
//�{�^���ɕ\�����镶��
static const int BUTTON_LETTER_SIZE = 40;
static       int NEXT_LETTER_X;
static       int RETRY_LETTER_X;
static const int BUTTON_LETTER_Y = (BUTTON_HEIGHT/2) - BUTTON_LETTER_SIZE/2;

//�R���X�g���N�^
Result::Result(SceneChanger *changer) 
	:BaseScene(changer)
{
}

//�f�X�X�g���N�^
Result::~Result() {
}

void Result::Initialize() {
	//BGM���Đ�
	m_resultSnd = LoadSoundMem("sound/Othello/result.mp3");
	ChangeVolumeSoundMem(130, m_resultSnd);
	PlaySoundMem(m_resultSnd, DX_PLAYTYPE_LOOP, true);

	//�摜�̃��[�h
	m_resultPic = LoadGraph("pic/result.png");
	nextPic = LoadGraph("pic/next.png");
	nextOnPic = LoadGraph("pic/nextOn.png");
	retryPic = LoadGraph("pic/retry.png");
	retryOnPic = LoadGraph("pic/retryOn.png");
	
	//�����̃��[�h
	m_nextSnd = LoadSoundMem("sound/next.mp3");
	m_retrySnd = LoadSoundMem("sound/start.mp3");
	//���ʐݒ�
	ChangeVolumeSoundMem(150, m_nextSnd);
	ChangeVolumeSoundMem(150, m_retrySnd);

	//AI�\���ʒu
	SetFontSize(AI_LETTER_SIZE);
	AI_X = WIN_SIZE_X / 2 - GetDrawStringWidth(Share::AI[(int)Share::ai].c_str() , (int)Share::AI[(int)Share::ai].size())/2;

	//���ʕ\���ʒu�v�Z
	switch (Share::playerStatus) {
	case WIN:
		RESULT_COLOR = COLOR_RED;
		RESULT_STR = "WIN";
		break;
	case LOSE:
		RESULT_COLOR = COLOR_BLUE;
		RESULT_STR = "LOSE";
		break;
	case DRAW:
		RESULT_COLOR = COLOR_BlACK;
		RESULT_STR = "DRAW";
		break;
	case NONE:
		RESULT_COLOR = COLOR_BlACK;
		RESULT_STR = "DRAW";
		break;
	}
	SetFontSize(RESULT_LETTER_SIZE);
	RESULT_X = WIN_SIZE_X / 2 - GetDrawStringWidth(RESULT_STR.c_str() , (int)RESULT_STR.size())/2;

	//���x���\���ʒu�̌v�Z
	SetFontSize(LEVEL_STR_LETTER_SIZE);
	int levelStrWidth = GetDrawStringWidth("LEVEL:", 6);
	SetFontSize(LEVEL_LETTER_SIZE);
	int levelWidth = GetDrawStringWidth(std::to_string(Share::level).c_str(), (int)std::to_string(Share::level).size());
	LEVEL_STR_X = WIN_SIZE_X / 2 - (levelStrWidth + levelWidth) / 2;
	LEVEL_X = LEVEL_STR_X + levelStrWidth + 5;
	LEVEL_Y = LEVEL_STR_Y - (LEVEL_LETTER_SIZE - LEVEL_STR_LETTER_SIZE) + 2;

	//���\���ʒu
	SetFontSize(NUM_SIZE);
	int blackWidth = GetDrawStringWidth(std::to_string(Share::blackNum).c_str(), (int)std::to_string(Share::blackNum).size());
	int whiteWidth = GetDrawStringWidth(std::to_string(Share::whiteNum).c_str(), (int)std::to_string(Share::whiteNum).size());
	NUM_X = WIN_SIZE_X / 2 - (blackWidth + whiteWidth + GetDrawStringWidth(" - ", 3)) / 2;

	//�{�^���ʒu�̌v�Z
	SetFontSize(BUTTON_LETTER_SIZE);
	NEXT_LETTER_X = (BUTTON_WIDTH / 2) - (GetDrawStringWidth("NEXT", 4) / 2);
	RETRY_LETTER_X = (BUTTON_WIDTH / 2) - (GetDrawStringWidth("RETRY", 5) / 2);

	//�{�^���̍쐬
	nextButton = Button(nextPic, nextOnPic, BUTTON_X, NEXT_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	retryButton = Button(retryPic, retryOnPic, BUTTON_X, RETRY_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
}

void Result::Update() {
	//�}�E�X�ʒu�̎擾
	Mouse::instance()->update();

	//���փ{�^��
	nextButton.update(&nowNext);
	if (nowNext) {
		m_sceneChanger->ChangeScene(Scene_Menu);
	}

	//������x�{�^��
	retryButton.update(&nowRetry);
	if (nowRetry) {
		m_sceneChanger->ChangeScene(Scene_Game);
	}
}

void Result::Draw() {
	//�w�i�̕\��
	DrawExtendGraph(0 , 0 ,WIN_SIZE_X ,WIN_SIZE_Y , m_resultPic , false);

	//���x���̕\��
	SetFontSize(AI_LETTER_SIZE);
	DrawString(AI_X, AI_Y, Share::AI[(int)Share::ai].c_str(), COLOR_BlACK);

	//���x���̕\��
	SetFontSize(LEVEL_STR_LETTER_SIZE);
	DrawString(LEVEL_STR_X ,LEVEL_STR_Y , "LEVEL:" ,COLOR_BlACK);
	SetFontSize(LEVEL_LETTER_SIZE);
	DrawFormatString(LEVEL_X , LEVEL_Y , COLOR_BlACK ,"%d" , Share::level);

	//���ʂ̕\��
	SetFontSize(RESULT_LETTER_SIZE);
	DrawString(RESULT_X, RESULT_Y, RESULT_STR.c_str(), RESULT_COLOR);

	//���\��
	SetFontSize(NUM_SIZE);
	DrawFormatString(NUM_X , NUM_Y , COLOR_BlACK, "%d - %d" ,Share::blackNum , Share::whiteNum);

	/** �{�^�� **/
	SetFontSize(BUTTON_LETTER_SIZE);

	//���փ{�^��
	nextButton.draw();

	//������x�{�^��
	retryButton.draw();
}

void Result::Finalize() {
	//���[�h��ʂ̕\��
	DrawExtendGraph(0, 0, WIN_SIZE_X + 5, WIN_SIZE_Y, Share::loadPic, true);
	//����
	if (nowNext) {
		PlaySoundMem(m_nextSnd, DX_PLAYTYPE_NORMAL, true);
	}
	else if (nowRetry) {
		PlaySoundMem(m_retrySnd , DX_PLAYTYPE_NORMAL, true);
	}
	//BGM���~�߂�
	StopSoundMem(m_resultSnd);
	//�������̍폜
	deleteMem();
}

void Result::deleteMem() {
	DeleteGraph(retryPic);
	DeleteGraph(retryOnPic);
	DeleteGraph(nextPic);
	DeleteGraph(nextOnPic);
	DeleteGraph(m_resultPic);

	DeleteSoundMem(m_resultSnd);
	DeleteSoundMem(m_nextSnd);
	DeleteSoundMem(m_retrySnd);
}