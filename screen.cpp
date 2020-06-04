#include "screen.h"
#include <windows.h>

using namespace std;

Screen::Screen() {
	width = 10;
	height = 20;
	SetConsoleScreenBufferSize(myScreen, {(short)width,(short)height });
}

Screen::Screen(int w, int h) {
	width = w;
	height = h;
	SetConsoleScreenBufferSize(myScreen, {(short) width,(short)height });
}

void Screen::clear() {
	
}

void Screen::draw(wchar_t* &field) {
	SetConsoleActiveScreenBuffer(myScreen);
	DWORD out = 0;
	WriteConsoleOutputCharacter(myScreen, field, width * height, { 0,0 }, &out);
}



