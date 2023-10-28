#pragma once

#include "BaseScene.h"
#include "SceneChanger.h"

#include "include.h"

class Result :public BaseScene {
private:
	//ゲーム進行用変数
	//画像ハンドル
	int m_resultPic;
	//音声ハンドル
	int m_resultSnd;
	int m_nextSnd;
	int m_retrySnd;

	//描画用変数
	static bool onNext;
	static bool onRetry;

	//音声判定用
	static bool nowNext;
	static bool nowRetry;

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