#pragma once
#include <functional>

#include "BaseScene.h"
#include "SceneChanger.h"
#include "State.h"
#include "include.h"

using AIFunction = std::function<uint64_t(const State &)>;

class Game :public BaseScene {
private:
	//�Q�[���i�s�p�ϐ�
	State m_state;
	//Coord m_selectCoord = Coord(0, 0);
	uint64_t m_selectSquare = 0;
	//Coord m_currentPut = Coord(-1, 0);
	uint64_t m_recentPut = 0;
	std::vector<std::pair<State, uint64_t>> undo_vec;
	AIFunction aiFunc;

	//�摜�n���h��
	int m_gamePic;
	int m_blackPic;
	int m_whitePic;
	int m_boxBlackPic;
	int m_boxWhitePic;
	int m_undoButtonPic;
	int m_undoButtonOffPic;
	int m_undoFramePic;
	int m_stopButtonPic;
	//�����n���h��
	int m_gameSnd;
	int m_pieceSnd;
	int m_passSnd;
	int m_undoSnd;
	int m_stopSnd;
	//AI�p
	int m_playoutNum;

	void getCoord(int* , int* , const uint64_t)const;
	void setSelectSquare(const int , const int);
public:
	//�֐�
	void deleteMem();

	Game(SceneChanger*);
	~Game();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};