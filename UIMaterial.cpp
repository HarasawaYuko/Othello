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

Button::Button(const int pic , const int onPic , const int x, const int y , const int width, const int height , const int offPic)
	: pic(pic) ,onPic(onPic) , x(x) , y(y) , width(width) , height(height) , isOn(false) ,offPic(offPic)
{}

void Button::update(bool* click) {
	if (!active) return;
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
	if (!active) {
		DrawExtendGraph(x, y, x + width, y + height, offPic, true);
		return;
	}
	if(isOn){
		DrawExtendGraph(x , y , x+width , y+height , onPic ,true);
	}
	else{
		DrawExtendGraph(x, y, x + width, y + height, pic, true);
	}
}

void Button::turnOff() {
	active = false;
}

void Button::turnOn() {
	active = true;
}

/*********************/

RadioButton::RadioButton(const int x , const int y ,const int size , const int space, const int num ,const int font ,const std::vector<std::string> strs)
	:x(x) , y(y) ,size(size),space(space), num(num), on(-1), fontsize(font), strs(strs)
{}

void RadioButton::update(bool* click) {
	*click = false;
	on = -1;
	for (int i = 0; i < num; i++) {
		if (isIn(x + space * i, y, size * 2, size * 2)) {
			on = i;
			if (Mouse::instance()->getClickNow(LEFT_CLICK)) {
				select = i;
				*click = true;
			}
		}
	}
}

void RadioButton::draw() const{
	SetFontSize(fontsize);
	//num�J��Ԃ�
	for (int i = 0; i < num; i++) {
		DrawCircleAA((float)(x + size + i*space) , (float)(y + size), (float)size, 20, COLOR_WHITE, true);
		DrawString(x + size*2 + i*space + 10, y , strs[i].c_str(),COLOR_BlACK );
	}
	if (on >= 0) {
		DrawCircleAA((float)(x + size + on * space), (float)(y + size) , (float)(size, 20), COLOR_LBLUE , false , 5);
	}
	//�I���ӏ��\��
	DrawCircleAA((float)(x + size + select * space),(float)( y + size), (float)(size * 0.7), 20, COLOR_BLUE, true);
}

int RadioButton::getSelect() const {
	return select;
}