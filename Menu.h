#pragma once

#include "BaseScene.h"
#include "SceneChanger.h"
#include "UIMaterial.h"
#include "include.h"

//���j���[��ʃN���X

class Menu :public BaseScene {
private:
	//�摜�n���h��
	int m_titlePic;
	int m_sideBarPic;
	int m_closeButtonPic;
	int m_closeFramePic;
	int m_rightSwipePic;
	int m_leftSwipePic;
	int m_rightSwipeSelectPic;
	int m_leftSwipeSelectPic;

	int m_startPic;
	int m_startOnPic;
	int m_closePic;
	int m_closeOnPic;

	//�����n���h��
	int m_menuSnd;
	int m_startSnd;
	int m_radioSnd;
	int m_sideSnd;
	int m_swipeSnd;

	//�`�攻��p
	//bool onStartButton;
	bool onRadioBlack;
	bool onRadioWhite;
	bool onSideBar;
	bool onClose;
	bool onSwipeRight;
	bool onSwipeLeft;

	//���͔���p�ϐ�
	bool nowStart;
	bool nowRadio;
	bool nowSide;
	bool nowSwipe;
	bool nowClose;

	Button startButton;
	Button closeButton;
	Button swipeLeftButton;
	Button swipeRightButton;
	RadioButton turnButton;

	//�`��p
	int m_bar_select_x;
	int m_level;

	void deleteMem();

public:
	Menu(SceneChanger*);
	~Menu();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};