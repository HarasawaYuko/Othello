#pragma once

#include "Task.h"
#include "SceneChanger.h"



class BaseScene : public Task {
protected:
	SceneChanger* m_sceneChanger;

public:
	BaseScene(SceneChanger*);
	virtual ~BaseScene();
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update() override;
	virtual void Draw() override;
};