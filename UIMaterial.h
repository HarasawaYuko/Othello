#pragma once
#include "include.h"
#include <functional>

class Button {
private:
	int pic;
	int onPic;
	int x;
	int y;
	int width;
	int height;
	std::function<void(void)> func;
public:
	Button(const int , const int, const int, const int, const int, const int, std::function<void(void)>);
	void update();
	void draw();
};