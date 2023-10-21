#pragma once
#include <functional>

#include "BaseScene.h"
#include "SceneChanger.h"

#include "OthelloState.h"
#include "include.h"

using AIFunction = std::function<Coord(const OthelloState &)>;

class Game :public BaseScene {
private:
	//ゲーム進行用変数
	OthelloState m_state;
	Coord m_selectCoord = Coord(0, 0);
	Coord m_currentPut = Coord(-1, 0);
	std::vector<std::pair<OthelloState, Coord>> undo_vec;
	AIFunction aiFunc;

	//画像ハンドル
	int m_gamePic;
	int m_blackPic;
	int m_whitePic;
	int m_boxBlackPic;
	int m_boxWhitePic;
	int m_undoButtonPic;
	int m_undoButtonOffPic;
	int m_undoFramePic;
	int m_stopButtonPic;
	//音声ハンドル
	int m_gameSnd;
	int m_pieceSnd;
	int m_passSnd;
	int m_undoSnd;
	int m_stopSnd;
	//AI用
	int m_playoutNum;
public:
	//関数
	void MouseToCoord(int, int, Coord*);
	void deleteMem();

	Game(SceneChanger*);
	~Game();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};