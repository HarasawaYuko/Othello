#pragma once
#include "include.h"

//���ʕϐ����Ǘ�����static�N���X

class Share {
private:
	Share();

public:
	static int playerColor;
	static int level;
	static int loadPic;
	//�v���g�^�C�v�錾
	static bool isIn(const int, const int, const int, const int, const int, const int);
	static void Share_Initialize();
};