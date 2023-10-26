#include "Mouse.h"

Mouse* Mouse::getInstance() {
	static Mouse instance;
	return &instance;
}

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

void Mouse::finalize() {

}

bool Mouse::getClick(clickCode code) {
	return mouseInput[code] > 0;
}

bool Mouse::getClickNow(clickCode code) {
	return mouseInput[code] == 1;
}

int Mouse::getX() {
	return posX;
}

int Mouse::getY() {
	return posY;
}