#pragma once

enum Scene{
	Scene_Othello,
	Scene_Maze,
	Scene_Menu,
	Scene_Game,
	Scene_Fin,
	Scene_None
};

//�V�[���ύX�p�C���^�[�t�F�[�X
class SceneChanger {
public:
	virtual void ChangeScene(Scene next) = 0;
};