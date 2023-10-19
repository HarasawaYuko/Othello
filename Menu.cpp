#include "Menu.h"
#include "OthelloState.h"
#include "Share.h"

//���W�I�{�^���֌W
static const int TURN_RADIO_Y = 250;
static const int TURN_RADIO_X = 210;
static const int TURN_RADIO_SIZE = 20;
static const int TURN_RADIO_SELECT_SIZE = 13;
static const int TURN_RADIO_SPACE = 210;
//�J�n�{�^���֌W
static const int START_BUTTON_X = 320;
static const int START_BUTTON_Y = 480;
static const int START_BUTTON_WIDTH = 160;
static const int START_BUTTON_HEIGHT = 50;
//�T�C�h�o�[�֌W
static const int BAR_X = 180;
static const int BAR_Y = 320;
static const int BAR_LENGTH = 350;
static const int BAR_HEIGHT = 6;
static const int BAR_SELECT_SIZE = 13;


//����{�^���֌W
static const int CLOSE_X = 730;
static const int CLOSE_Y = 10;
static const int CLOSE_WIDTH = 60;
static const int CLOSE_HEIGHT = 45;

//�F
const unsigned int COLOR_BOARD = GetColor(0x10, 0xB5, 0x67);
static const unsigned int COLOR_BLUE = GetColor(0, 10, 140);
static const unsigned int COLOR_LBLUE = GetColor(101, 187, 233);
static const unsigned int COLOR_CANPUT = GetColor(0xFF, 0x99, 0x33);
static const unsigned int COLOR_SELECT = GetColor(0x00, 0x33, 0xCC);
static const unsigned int COLOR_RED = GetColor(220, 45, 35);

//�`�攻��p
static bool onStartButton;
static bool onRadioBlack;
static bool onRadioWhite;
static bool onSideBar;
static bool onClose;

//��������p�ϐ�
static bool nowStart;
static bool nowRadio;
static bool nowSide;

Menu::Menu(SceneChanger *changer) 
	:BaseScene(changer),
	m_bar_select_x(BAR_X),
	m_level(1)
{}

Menu::~Menu(){}

void Menu::Initialize() {
	//��Ԃ̏�����
	Share::playerColor = BLACK;
	Share::level = 1;

	//�摜�̃��[�h
	m_titlePic = LoadGraph("pic/Othello/Title.png");
	m_sideBarPic = LoadGraph("pic/Othello/SideBar.png");
	m_closeButtonPic = LoadGraph("pic/Othello/fin.png");
	m_closeFramePic = LoadGraph("pic/Othello/finFrame.png");

	//�����̃��[�h
	m_menuSnd = LoadSoundMem("sound/Othello/menu.mp3");
	m_startSnd = LoadSoundMem("sound/Othello/start.mp3");
	m_radioSnd = LoadSoundMem("sound/Othello/radio.mp3");
	m_sideSnd = LoadSoundMem("sound/Othello/side.mp3");
	//���ʐݒ�
	ChangeVolumeSoundMem(130, m_menuSnd);

	//BGM���Đ�
	PlaySoundMem(m_menuSnd, DX_PLAYTYPE_LOOP, true);
}

void Menu::Update() {

	//�t���O�̃��Z�b�g
	nowStart = false;
	nowRadio = false;
	nowSide = false;
	onClose = false;
	onRadioBlack = false;
	onRadioWhite = false;
	onStartButton = false;
	onSideBar = false;
	//�}�E�X�ʒu�̎擾
	int mousePosX;
	int mousePosY;
	int mouseInput;
	GetMousePoint(&mousePosX, &mousePosY);
	mouseInput = GetMouseInput();

	//���W�I�{�^��
	int radioBlackLeft = TURN_RADIO_X - TURN_RADIO_SIZE;
	int radioBlackRight = TURN_RADIO_X + TURN_RADIO_SIZE;
	int radioBlackTop = TURN_RADIO_Y - TURN_RADIO_SIZE;
	int radioBlackUnder = TURN_RADIO_Y + TURN_RADIO_SIZE;
	if (radioBlackLeft <= mousePosX && mousePosX <= radioBlackRight && radioBlackTop <= mousePosY && mousePosY <= radioBlackUnder) {
		if (mouseInput & MOUSE_INPUT_LEFT) {
			Share::playerColor = BLACK;
			nowRadio = true;
		}
		onRadioBlack = true;
	}
	if (radioBlackLeft + TURN_RADIO_SPACE <= mousePosX && mousePosX <= radioBlackRight + TURN_RADIO_SPACE && radioBlackTop <= mousePosY && mousePosY <= radioBlackUnder) {
		if (mouseInput & MOUSE_INPUT_LEFT) {
			Share::playerColor = WHITE;
			nowRadio = true;
		}
		onRadioWhite = true;
	}

	//�T�C�h�o�[
	if (BAR_X <= mousePosX && mousePosX <= BAR_X + BAR_LENGTH && BAR_Y - BAR_SELECT_SIZE <= mousePosY && mousePosY <= BAR_Y + BAR_SELECT_SIZE) {
		onSideBar = true;
		if (mouseInput & MOUSE_INPUT_LEFT) {
			m_bar_select_x = mousePosX;
			//���x���̐ݒ�
			int nextLevel = (mousePosX - BAR_X) / ((BAR_LENGTH / 9) - 1) + 1;
			if (Share::level != nextLevel) {
				nowSide = true;
				Share::level = nextLevel;
			}
		}
	}

	//�J�n�{�^��
	if (START_BUTTON_X <= mousePosX && mousePosX <= START_BUTTON_X + START_BUTTON_WIDTH && START_BUTTON_Y <= mousePosY && mousePosY <= START_BUTTON_Y + START_BUTTON_HEIGHT) {
		if (mouseInput & MOUSE_INPUT_LEFT) {
			m_sceneChanger->ChangeScene(Scene_Game);
			nowStart = true;
		}
		onStartButton = true;
	}
	if (mouseInput & MOUSE_INPUT_RIGHT) {
	}
	//����{�^��
	if (Share::isIn(CLOSE_X, CLOSE_Y, CLOSE_X + CLOSE_WIDTH, CLOSE_Y + CLOSE_HEIGHT, mousePosX, mousePosY)) {
		onClose = true;
		if (mouseInput & MOUSE_INPUT_LEFT) {
			m_sceneChanger->ChangeScene(Scene_Fin);
		}
	}
}

void Menu::Draw() {
	//��ԑI��p���W�I�{�^��
	SetFontSize(35);
	DrawExtendGraph(0, 0, WIN_SIZE_X, WIN_SIZE_Y, m_titlePic, true);
	DrawCircleAA(TURN_RADIO_X, TURN_RADIO_Y, TURN_RADIO_SIZE, 20, GetColor(230, 230, 230), true);
	DrawString(TURN_RADIO_X + 25, TURN_RADIO_Y - TURN_RADIO_SIZE, "BLACK", GetColor(20, 20, 20));
	DrawCircleAA(TURN_RADIO_X + TURN_RADIO_SPACE, TURN_RADIO_Y, TURN_RADIO_SIZE, 20, GetColor(230, 230, 230), true);
	DrawString(TURN_RADIO_X + TURN_RADIO_SPACE + 25, TURN_RADIO_Y - TURN_RADIO_SIZE, "WHITE", GetColor(20, 20, 20));
	//�I��\��
	if (Share::playerColor == BLACK) {
		DrawCircleAA(TURN_RADIO_X, TURN_RADIO_Y, TURN_RADIO_SELECT_SIZE, 20, GetColor(10, 70, 150), true, 2);
	}
	else {
		DrawCircleAA(TURN_RADIO_X + TURN_RADIO_SPACE, TURN_RADIO_Y, TURN_RADIO_SELECT_SIZE, 20, GetColor(10, 70, 150), true, 2);
	}
	//�}�E�X�\��
	if (onRadioBlack) {
		DrawCircleAA(TURN_RADIO_X, TURN_RADIO_Y, TURN_RADIO_SIZE, 20, GetColor(101, 187, 233), false, 5);
	}
	else if (onRadioWhite) {
		DrawCircleAA(TURN_RADIO_X + TURN_RADIO_SPACE, TURN_RADIO_Y, TURN_RADIO_SIZE, 20, GetColor(101, 187, 233), false, 5);
	}

	//��Փx�����o�[
	DrawBoxAA(BAR_X, BAR_Y, BAR_X + BAR_LENGTH, BAR_Y + BAR_HEIGHT, GetColor(120, 120, 120), true);
	//�I���ӏ��\��
	DrawCircle(m_bar_select_x, BAR_Y, BAR_SELECT_SIZE, GetColor(10, 70, 150), true);
	if (onSideBar) {
		DrawCircle(m_bar_select_x, BAR_Y, BAR_SELECT_SIZE, GetColor(101, 187, 233), false, 3);
	}
	DrawFormatString(BAR_X + BAR_LENGTH + 95, BAR_Y - 20, GetColor(20, 20, 20), "%d", Share::level);
	SetFontSize(20);
	DrawFormatString(BAR_X + BAR_LENGTH + 20, BAR_Y - 8, GetColor(20, 20, 20), "LEVEL:");

	//�J�n�{�^��
	SetFontSize(35);
	DrawBoxAA(START_BUTTON_X, START_BUTTON_Y, START_BUTTON_X + START_BUTTON_WIDTH, START_BUTTON_Y + START_BUTTON_HEIGHT, GetColor(10, 70, 150), true);
	int width = GetDrawStringWidth("START", 5);
	int center_x = START_BUTTON_X + START_BUTTON_WIDTH / 2;
	DrawString(center_x - width / 2, START_BUTTON_Y + 5, "START", GetColor(230, 230, 230));
	if (onStartButton) {
		DrawBoxAA(START_BUTTON_X, START_BUTTON_Y, START_BUTTON_X + START_BUTTON_WIDTH, START_BUTTON_Y + START_BUTTON_HEIGHT, GetColor(101, 187, 233), false, 5.0);
	}

	//����{�^��
	DrawExtendGraph(CLOSE_X, CLOSE_Y, CLOSE_X + CLOSE_WIDTH, CLOSE_Y + CLOSE_HEIGHT, m_closeButtonPic, true);
	if (onClose) {
		DrawExtendGraph(CLOSE_X, CLOSE_Y, CLOSE_X + CLOSE_WIDTH, CLOSE_Y + CLOSE_HEIGHT, m_closeFramePic, true);
	}

	//����
	if (nowRadio) {
		PlaySoundMem(m_radioSnd, DX_PLAYTYPE_NORMAL, true);
	}
	if (nowSide) {
		PlaySoundMem(m_sideSnd, DX_PLAYTYPE_NORMAL, true);
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
}

void Menu::deleteMem() {
	DeleteGraph(m_titlePic);
	DeleteGraph(m_sideBarPic);
	DeleteGraph(m_closeButtonPic);
	DeleteGraph(m_closeFramePic);
	DeleteSoundMem(m_menuSnd);
	DeleteSoundMem(m_startSnd);
	DeleteSoundMem(m_radioSnd);
	DeleteSoundMem(m_sideSnd);
}