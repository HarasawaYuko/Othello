#include "UIMaterial.h"
#include "Mouse.h"

bool isIn(const int x, const int y, const int width, const int height) {
	if (x < Mouse::instance()->getX() && Mouse::instance()->getX() < x + width
		&& y < Mouse::instance()->getY() && Mouse::instance()->getY() < y + height) {
		return true;
	}
	else {
		return false;
	}
}

Button::Button(const int pic , const int onPic , const int x, const int y , const int width, const int height)
	: pic(pic) ,onPic(onPic) , x(x) , y(y) , width(width) , height(height) , isOn(false)
{}

void Button::update(bool* click) {
	*click = false;
	isOn = false;
	if (isIn(x ,y , width ,height)) {
		isOn = true;
		if (Mouse::instance()->getClickNow(LEFT_CLICK)) {
			*click = true;
		}
	}
}

void Button::draw() {
	if(isOn){
		DrawExtendGraph(x , y , x+width , y+height , onPic ,true);
	}
	else{
		DrawExtendGraph(x, y, x + width, y + height, pic, true);
	}
}