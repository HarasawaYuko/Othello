#pragma once

//抽象クラス

class Task {
public:
	//実装しなくても良い
	virtual void Initialize(){}
	virtual void Finalize(){}
	//実装が必須
	virtual void Update() = 0;
	virtual void Draw() = 0;
};