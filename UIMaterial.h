#pragma once
#include "include.h"
#include <functional>

bool isIn(const int, const int, const int, const int);

//êF
const unsigned int COLOR_GREEN = GetColor(0x10, 0xB5, 0x67);
const unsigned int COLOR_BLUE = GetColor(0, 30, 150);
const unsigned int COLOR_LBLUE = GetColor(101, 187, 233);
const unsigned int COLOR_ORANGE = GetColor(0xFF, 0x99, 0x33);
const unsigned int COLOR_WHITE = GetColor(230 , 230 ,230);
const unsigned int COLOR_BlACK = GetColor(20, 20, 20);
const unsigned int COLOR_RED = GetColor(220, 45, 35);
const unsigned int COLOR_GRAY = GetColor(120, 120, 120);

class Button {
private:
	int pic;
	int onPic;
	int offPic;
	int x;
	int y;
	int width;
	int height;
	bool isOn;
	bool active = true;

public:
	Button() {};
	Button(const int , const int,const int , const int, const int, const int  , const int offPic = 1);
	void update(bool*);
	void draw();
	void turnOff();
	void turnOn();
};

class RadioButton {
private:
	int num;
	int x;
	int y;
	int space;
	int size;
	int fontsize;
	
	std::vector<std::string> strs;
	int select;
	int on;
public:
	RadioButton(){}
	RadioButton(const int , const int , const int ,const int, const int ,const int,const std::vector<std::string>);
	void update(bool*);
	void draw()const;
	int getSelect() const;
};

class SlideBar {
private:
	int length;
	int x;
	int y;
	int size;
	int selectX;
	bool on;
public:
	SlideBar(){}
	SlideBar(const int , const int , const int , const int);
	void update(bool*);
	void draw() const;
	float getRate()const;
};