#pragma once
#include <windows.h>
#include "Tetromino.h"
using namespace std;

class Screen {
public:
	Screen();
	Screen(int, int);
	void draw(wchar_t * &field);
	void clear();

private:
	HANDLE myScreen = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	// HANDLE myScreen = GetStdHandle(STD_OUTPUT_HANDLE);
	int width;
	int height;
};

