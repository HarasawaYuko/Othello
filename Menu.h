#pragma once

#include "BaseScene.h"
#include "SceneChanger.h"

#include "include.h"

class Menu :public BaseScene {
private:
	//�摜�n���h��
	int m_titlePic;
	int m_sideBarPic;
	int m_closeButtonPic;
	int m_closeFramePic;
	//�����n���h��
	int m_menuSnd;
	int m_startSnd;
	int m_radioSnd;
	int m_sideSnd;

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