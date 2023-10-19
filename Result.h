#pragma once

#include "BaseScene.h"
#include "SceneChanger.h"

#include "include.h"

class Result :public BaseScene {
private:
	//�Q�[���i�s�p�ϐ�
	//�摜�n���h��
	int m_resultPic;
	//�����n���h��
	int m_resultSnd;
	int m_nextSnd;
	int m_retrySnd;

public:
	//�֐�
	void deleteMem();

	Result(SceneChanger*);
	~Result();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};