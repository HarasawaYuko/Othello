#pragma once
#include "include.h"
#include <functional>

bool isIn(const int, const int, const int, const int);

class Button {
private:
	int pic;
	int onPic;
	int x;
	int y;
	int width;
	int height;
	bool isOn;

public:
	Button() {};
	Button(const int , const int, const int, const int, const int, const int );
	void update(bool*);
	void draw();
};

