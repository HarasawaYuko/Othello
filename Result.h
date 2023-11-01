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
	int m_retryPic;
	int m_retryOnPic;
	int m_nextPic;
	int m_nextOnPic;

	//�����n���h��
	int m_resultSnd;
	int m_nextSnd;
	int m_retrySnd;

	//��������p
	bool m_nowNext;
	bool m_nowRetry;

	//�{�^��
	Button m_nextButton;
	Button m_retryButton;

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