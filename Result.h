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
	int retryPic;
	int retryOnPic;
	int nextPic;
	int nextOnPic;

	//音声ハンドル
	int m_resultSnd;
	int m_nextSnd;
	int m_retrySnd;

	//音声判定用
	bool nowNext;
	bool nowRetry;

	//ボタン
	Button nextButton;
	Button retryButton;

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