#pragma once

#include "BaseScene.h"

//�V�[���Ǘ�

class SceneMgr :public SceneChanger, Task {
private:
	BaseScene* m_scene;//���݂̃V�[���C���X�^���X
	Scene m_next_scene; //���̃V�[��

public:
	SceneMgr();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	bool fin = false;
	
	//�V�[���̕ύX
	void ChangeScene(Scene next) override;
};