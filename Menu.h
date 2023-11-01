#pragma once

#include "BaseScene.h"
#include "SceneChanger.h"
#include "UIMaterial.h"
#include "include.h"

//メニュー画面クラス

class Menu :public BaseScene {
private:
	//画像ハンドル
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

	//音声ハンドル
	int m_menuSnd;
	int m_startSnd;
	int m_radioSnd;
	int m_sideSnd;
	int m_swipeSnd;

	//入力判定用変数
	bool m_nowStart;
	bool m_nowRadio;
	bool m_nowSlide;
	bool m_nowSwipe;
	bool m_nowClose;

	Button m_startButton;
	Button m_closeButton;
	Button m_swipeLeftButton;
	Button m_swipeRightButton;
	RadioButton m_turnButton;
	SlideBar m_levelBar;

	void deleteMem();

public:
	Menu(SceneChanger*);
	~Menu();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};