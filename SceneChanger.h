#pragma once

enum Scene{
	Scene_Othello,
	Scene_Maze,
	Scene_Menu,
	Scene_Game,
	Scene_Fin,
	Scene_None
};

//シーン変更用インターフェース
class SceneChanger {
public:
	virtual void ChangeScene(Scene next) = 0;
};