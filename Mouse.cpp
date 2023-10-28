#include "Mouse.h"

//シングルトンへのアクセスを返す
Mouse* Mouse::instance() {
	static Mouse instance;
	return &instance;
}

//マウス情報の更新
void Mouse::update() {
	//マウス位置の取得
	GetMousePoint(&posX , &posY);
	int nowInput = GetMouseInput();
	for (int i = 0; i < INPUT_NUM; i++) {
		if ((nowInput >> i) & 1) {
			mouseInput[i]++;
		}
		else {
			mouseInput[i] = 0;
		}
	}
}

//クリックされているかを取得
bool Mouse::getClick(clickCode code) {
	return mouseInput[code] > 0;
}

//今クリックされた瞬間かを取得
bool Mouse::getClickNow(clickCode code) {
	return mouseInput[code] == 1;
}

int Mouse::getX() {
	return posX;
}

int Mouse::getY() {
	return posY;
}