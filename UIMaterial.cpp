#include "UIMaterial.h"

Button::Button(const int pic , const int onPic , const int x, const int y , const int width, const int height, std::function<void(void)>f)
	: pic(pic) ,onPic(onPic) , x(x) , y(y) , width(width) , height(height) , func(f) , isOn(false)
{}

void Button::update() {

}

void Button::draw() {

}