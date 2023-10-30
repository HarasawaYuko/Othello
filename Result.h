#pragma once

#include "BaseScene.h"
#include "SceneChanger.h"
#include "UIMaterial.h"

#include "include.h"

class Result :public BaseScene {
private:
	//�Q�[���i�s�p�ϐ�
	//�摜�n���h��
	int m_resultPic;
	int retryPic;
	int retryOnPic;
	int nextPic;
	int nextOnPic;

	//�����n���h��
	int m_resultSnd;
	int m_nextSnd;
	int m_retrySnd;

	//��������p
	bool nowNext;
	bool nowRetry;

	//�{�^��
	Button nextButton;
	Button retryButton;

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