#pragma once

#include "BaseScene.h"
#include "SceneChanger.h"
#include "UIMaterial.h"

#include "include.h"

class Result :public BaseScene {
private:
	//ゲーム進行用変数
	//画像ハンドル
	int m_resultPic;
	int m_retryPic;
	int m_retryOnPic;
	int m_nextPic;
	int m_nextOnPic;

	//音声ハンドル
	int m_resultSnd;
	int m_nextSnd;
	int m_retrySnd;

	//音声判定用
	bool m_nowNext;
	bool m_nowRetry;

	//ボタン
	Button m_nextButton;
	Button m_retryButton;

public:
	//関数
	void deleteMem();

	Result(SceneChanger*);
	~Result();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};