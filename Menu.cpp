#include "Menu.h"
#include "Share.h"
#include "Mouse.h"

//�^�[���I�����W�I�{�^���֌W
static const int TURN_RADIO_Y = 230;
static const int TURN_RADIO_X = 200;
static const int TURN_RADIO_SIZE = 20;
static const int TURN_RADIO_SELECT_SIZE = 13;
static const int TURN_RADIO_SPACE = 210;

//AI�֌W
static const int AI_LEVEL_MAX[] = { 10 , 7 };
static       int AI_INDEX = 0;
static const int AI_NUM = 2;

//Ai�I���X���C�v�{�^���֌W
static const int SWIPE_Y = 310;
static const int SWIPE_X = 200;
static const int SWIPE_WIDTH = 65;
static const int SWIPE_HEIGHT = 65;
static const int SWIPE_SPACE = 400 - SWIPE_WIDTH;
static const int AI_STR_SIZE = 40;
static       int AI_STR_X;
static const int AI_STR_Y = SWIPE_Y + (SWIPE_HEIGHT/2) - (AI_STR_SIZE/2);

//�J�n�{�^���֌W
static const int START_BUTTON_WIDTH = 210;
static const int START_BUTTON_HEIGHT = 70;
static const int START_BUTTON_X = WIN_CENT_X - START_BUTTON_WIDTH/2;
static const int START_BUTTON_Y = 460;


//�T�C�h�o�[�֌W
static const int BAR_X = 180;
static const int BAR_Y = 420;
static const int BAR_WIDTH = 350;
static const int BAR_HEIGHT = 6;
static const int BAR_SELECT_SIZE = 13;


//����{�^���֌W
static const int CLOSE_X = 730;
static const int CLOSE_Y = 10;
static const int CLOSE_WIDTH = 60;
static const int CLOSE_HEIGHT = 45;

Menu::Menu(SceneChanger *changer) 
	:BaseScene(changer),
	m_bar_select_x(BAR_X),
	beforeLevel(1)
{}

Menu::~Menu(){}

void Menu::Initialize() {
	//��Ԃ̏�����
	Share::playerColor = BLACK;
	Share::level = 1;

	//�摜�̃��[�h
	m_titlePic = LoadGraph("pic/Title.png");
	m_sideBarPic = LoadGraph("pic/SideBar.png");
	m_closeButtonPic = LoadGraph("pic/fin.png");
	m_closeFramePic = LoadGraph("pic/finFrame.png");
	m_rightSwipePic = LoadGraph("pic/rightSwipe.png");
	m_leftSwipePic = LoadGraph("pic/leftSwipe.png");
	m_rightSwipeSelectPic = LoadGraph("pic/rightSwipeSelect.png");
	m_leftSwipeSelectPic = LoadGraph("pic/leftSwipeSelect.png");
	m_startPic = LoadGraph("pic/start.png");
	m_startOnPic = LoadGraph("pic/startOn.png");
	m_closePic = LoadGraph("pic/fin.png");
	m_closeOnPic = LoadGraph("pic/finOn.png");


	//�{�^���쐬
	startButton = Button( m_startPic, m_startOnPic,START_BUTTON_X , START_BUTTON_Y , START_BUTTON_WIDTH , START_BUTTON_HEIGHT );
	closeButton = Button(m_closePic, m_closeOnPic, CLOSE_X, CLOSE_Y, CLOSE_WIDTH, CLOSE_HEIGHT);
	swipeLeftButton = Button(m_leftSwipePic , m_leftSwipeSelectPic , SWIPE_X ,SWIPE_Y , SWIPE_WIDTH , SWIPE_HEIGHT);
	swipeRightButton = Button(m_rightSwipePic, m_rightSwipeSelectPic, SWIPE_X + SWIPE_SPACE, SWIPE_Y, SWIPE_WIDTH, SWIPE_HEIGHT);
	turnButton = RadioButton(TURN_RADIO_X, TURN_RADIO_Y, TURN_RADIO_SIZE,TURN_RADIO_SPACE, 2, 35, {"BLACK" ,"WHITE"});
	levelBar = SlideBar(BAR_X , BAR_Y , BAR_WIDTH ,BAR_SELECT_SIZE);

	//�����̃��[�h
	m_menuSnd = LoadSoundMem("sound/menu.mp3");
	m_startSnd = LoadSoundMem("sound/start.mp3");
	m_radioSnd = LoadSoundMem("sound/radio.mp3");
	m_sideSnd = LoadSoundMem("sound/side.mp3");
	m_swipeSnd = LoadSoundMem("sound/swipe.mp3");

	//���ʐݒ�
	ChangeVolumeSoundMem(130, m_menuSnd);

	//BGM���Đ�
	PlaySoundMem(m_menuSnd, DX_PLAYTYPE_LOOP, true);
}

void Menu::Update() {
	//�t���O�̃��Z�b�g
	onClose = false;
	onRadioBlack = false;
	onRadioWhite = false;
	onSideBar = false;
	onSwipeLeft = false;
	onSwipeRight = false;
	nowStart = false;
	nowRadio = false;
	nowSlide = false;
	nowSwipe = false;
	nowClose = false;

	//�}�E�X�ʒu�̎擾
	Mouse::instance()->update();

	turnButton.update(&nowRadio);
	if (nowRadio) {
		Share::playerColor = (piece)turnButton.getSelect();
	}

	//AI�I��p�X���C�v�{�^��
	swipeLeftButton.update(&nowSwipe);
	if (nowSwipe) {
		AI_INDEX--;
		if (AI_INDEX < 0) AI_INDEX = AI_NUM - 1;
	}
	else{
		swipeRightButton.update(&nowSwipe);
		if (nowSwipe) {
			AI_INDEX = (AI_INDEX + 1) % AI_NUM;
		}
	}

	//�T�C�h�o�[
	//���x���̐ݒ�
	levelBar.update(&nowSlide);
	if (nowSlide) {
		beforeLevel = Share::level;
		Share::level = (float)(levelBar.getRate() * AI_LEVEL_MAX[(int)Share::ai]) + 1;
		if (beforeLevel == Share::level) {
			nowSlide = false;
		}
	}

	//�J�n�{�^��
	startButton.update(&nowStart);
	if (nowStart) {
		m_sceneChanger->ChangeScene(Scene_Game);
	}

	//����{�^��
	closeButton.update(&nowClose);
	if (nowClose) {
		m_sceneChanger->ChangeScene(Scene_Fin);
	}
}

void Menu::Draw() {
	DrawExtendGraph(0, 0, WIN_SIZE_X, WIN_SIZE_Y, m_titlePic, true);
	//��ԑI��p���W�I�{�^��
	turnButton.draw();

	//AI�I��\��
	swipeLeftButton.draw();
	swipeRightButton.draw();

	SetFontSize(AI_STR_SIZE);
	int width = GetDrawStringWidth(Share::AI[AI_INDEX].c_str() , (int)Share::AI[AI_INDEX].size());
	AI_STR_X = WIN_SIZE_X / 2 - width / 2;
	DrawString(AI_STR_X , AI_STR_Y , Share::AI[AI_INDEX].c_str() , COLOR_BlACK);

	//��Փx�����o�[
	levelBar.draw();
	SetFontSize(35);
	DrawFormatString(BAR_X + BAR_WIDTH + 95, BAR_Y - 20, COLOR_BlACK, "%d", Share::level);
	SetFontSize(20);
	DrawFormatString(BAR_X + BAR_WIDTH + 20, BAR_Y - 8, COLOR_BlACK, "LEVEL:");

	//�J�n�{�^��
	startButton.draw();

	//����{�^��
	closeButton.draw();

	//����
	if (nowRadio) {
		PlaySoundMem(m_radioSnd, DX_PLAYTYPE_NORMAL, true);
	}
	if (nowSlide) {
		PlaySoundMem(m_sideSnd, DX_PLAYTYPE_NORMAL, true);
	}
	if (nowSwipe) {
		PlaySoundMem(m_swipeSnd, DX_PLAYTYPE_NORMAL, true);

	}
}

void Menu::Finalize() {
	//���[�h��ʂ̕\��
	DrawExtendGraph(0, 0, WIN_SIZE_X+5, WIN_SIZE_Y, Share::loadPic, true);
	//�Q�[����ʑJ�ڂȂ�SE
	if (nowStart) {
		PlaySoundMem(m_startSnd, DX_PLAYTYPE_NORMAL, true);
	}
	//BGM���~�߂�
	StopSoundMem(m_menuSnd);
	//�n���h���̉��
	deleteMem();

	//AI�̐ݒ�
	Share::ai = (Ai)AI_INDEX;
}

void Menu::deleteMem() {
	DeleteGraph(m_titlePic);
	DeleteGraph(m_sideBarPic);
	DeleteGraph(m_closeButtonPic);
	DeleteGraph(m_closeFramePic);
	DeleteGraph(m_rightSwipePic);
	DeleteGraph(m_rightSwipeSelectPic);
	DeleteGraph(m_leftSwipePic);
	DeleteGraph(m_leftSwipeSelectPic);
	DeleteSoundMem(m_menuSnd);
	DeleteSoundMem(m_startSnd);
	DeleteSoundMem(m_radioSnd);
	DeleteSoundMem(m_sideSnd);
	DeleteSoundMem(m_swipeSnd);
}