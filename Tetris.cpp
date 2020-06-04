#include "Tetris.h"
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <stdio.h>

#define BLOCK L'\u25A2'

using namespace std;


/*
			xOffset   w	   xOffset+w+1
				^ 0123456789 ^
 yOffset <		- ---------- -   
				|		     |
 				|		     |
				|            |
h				|    Field   |
				|            |
				|            |
				|            |
yOffset+h+1	<	- ---------- -
*/



Tetris::Tetris() {
	// screen size (field size)
	sw = 30;
	sh = 30;
	field = new wchar_t[sw * sh];
	myScreen = new Screen(sw, sh);

	// play field size
	w = 10;
	h = 20;
	// where to put the play field
	xOffset = 1;
	yOffset = 1;

	// preview window
	wp = 5;
	hp = 4;
	xPreviewOffset = 15;
	yPreviewOffset = 1;

	// lv postion
	lv = 1;
	xlv = xPreviewOffset;
	ylv = yPreviewOffset + hp+3;

	//score position
	score = 0;
	xScore = xlv;
	yScore = ylv+1;


	// initialize screen buffer
	// initialize field
	for (int i = 0; i < sh; i++) {
		for (int j = 0; j < sw; j++) {
			if ((i == yOffset || i == yOffset + h + 1) && (j >= xOffset && j <= xOffset + w + 1)) field[i * sw + j] = '-'; 
			else if ((i >= yOffset && i <= yOffset + h + 1) && (j == xOffset || j == xOffset + w + 1)) field[i * sw + j] = '|'; 
			else if ((i == yPreviewOffset || i == yPreviewOffset + hp + 1) && (j >= xPreviewOffset && j <= xPreviewOffset + wp + 1)) field[i * sw + j] = '-';
			else if ((i >= yPreviewOffset && i <= yPreviewOffset + hp + 1) && (j == xPreviewOffset || j == xPreviewOffset + wp + 1)) field[i * sw + j] = '|';
			else field[i * sw + j] = ' '; 
		}
	}

	swprintf_s(&field[ylv * sw + xlv], 10, L"Level %3d", lv); 
	field[ylv * sw + xlv + 9] = ' '; // for some reason the null character is shown, remove the null character
	swprintf_s(&field[yScore * sw + xScore], 10, L"Score %3d", score);
	field[yScore * sw + xScore + 9] = ' '; // same

	isSet = false;
}

void Tetris::run() {
	vector<int> rowlist;
	srand(time(NULL)); // use to generate random seed
	char type = "IJLOSTZ"[rand() % 7];
	int speed = 10;
	while (!isGameOver(rowlist)) {
		int speed = 11 - lv;
		if (speed <= 0) speed = 1;

		Tetromino t = Tetromino(type, xOffset + 4, yOffset - 2); // initial tetromino
		Tetromino t_next = Tetromino("IJLOSTZ"[rand() % 7], xPreviewOffset + 2, yPreviewOffset + 2); // prepare next tetromino
		{
			// clear the preview window
			for (int i = yPreviewOffset + 1; i <= yPreviewOffset + hp; i++) {
				for (int j = xPreviewOffset + 1; j <= xPreviewOffset + wp; j++) {
					field[i * sw + j] = ' ';
				}
			}
			// draw next tetromino in the preview window
			for (int i = 0; i < t_next.row; i++) {
				for (int j = 0; j < t_next.col; j++) {
					if (t_next.pattern[0][i * t_next.col + j]) field[(t_next.y + i) * sw + (t_next.x + j)] = BLOCK;
				}
			}
		}
		
		unsigned long count = 0; // game tick

		// to avoid continuous rotate key pressing
		unsigned long preTime = 0;
		unsigned long delay = 2;

		isSet = false;
		track.clear();

		while (!isSet) {
			if (++count % speed == 0) move(t, 'D'); // drop the tetromino

			// input commands
			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && (count - preTime >= delay)) {
				preTime = count;
				rotate(t);
			}

			if (GetAsyncKeyState((int)'S') & 0x8000 || GetAsyncKeyState((int)'s') & 0x8000) move(t, 'D');
			if (GetAsyncKeyState((int)'A') & 0x8000 || GetAsyncKeyState((int)'a') & 0x8000) move(t, 'L');
			if (GetAsyncKeyState((int)'D') & 0x8000 || GetAsyncKeyState((int)'d') & 0x8000) move(t, 'R');

			myScreen->draw(field);
			Sleep(50);
		}

		{// check affected line
			rowlist.clear();
			for (int i = 0; i < t.row; i++) {
				for (int j = 0; j < t.col; j++) {
					if (t.pattern[t.currentPattern][i * t.col + j]) {
						int currentRow = t.y + i;
						rowlist.push_back(currentRow);
						for (int k = xOffset + 1; k <= xOffset + w; k++) {
							if (field[(t.y + i) * sw + k] == ' ') {
								rowlist.pop_back();
								break;
							}
						}
						break;
					}
				}
			}
		}

		update(rowlist); // remove the complete row and drag all the pieces above down
		type = t_next.type; // load next type;
	}
}

inline bool Tetris::rotate(Tetromino &t) {
	int after = (t.currentPattern==3? 0 : t.currentPattern+1);
	int row = t.row;
	int col = t.col;
	vector<int> temp;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (t.pattern[after][i * row + j]) {
				int x = (t.x + j);
				int y = (t.y + i);
				int index = y * sw + x;
				if (x<=xOffset || x>xOffset+w || y> yOffset+h) return false; // out of boun
				if ( field[index] == BLOCK && find(track.begin(), track.end(), index) == track.end()) return false; // block by other tetromino
				temp.push_back(index);
			}
		}
	}

	for (int i : track) field[i] = ' ';
	track.clear();

	for (int i : temp) {
		if ((i / sw) > yOffset) {
			track.push_back(i);
			field[i] = BLOCK;
		}
	}
	t.rotate();
	return true;
}


inline bool Tetris::move(Tetromino& t, char dir) {
	int row = t.row;
	int col = t.col;	
	int tx = t.x;
	int ty = t.y;
	vector<int> temp;
	switch (dir) {
	case 'L':
		tx--;
		break;
	case 'R':
		tx++;
		break;
	case 'U':
		ty--;
		break;
	case 'D':
		ty++;
		break;
	}

	for (int i = 0; i <row; i++) {
		for (int j = 0; j < col; j++) {
			if (t.pattern[t.currentPattern][i * row + j]) {
				int x = (tx + j);
				int y = (ty + i);
				int index = y * sw + x;
				if (!(x > xOffset && x <= xOffset + w)) return false; // out of bound left/right
				if (y > yOffset + h) { // touch the bottom
					isSet = true;
					return false;
				}
				if (field[index] == BLOCK && find(track.begin(), track.end(), index) == track.end()) {// block by other teromino
					if (dir == 'D') isSet = true; // cannot move down, set
					return false;
				}
				temp.push_back(index);
			}
		}
	}

	for (int i : track) field[i] = ' ';
	track.clear();

	for (int i : temp) {
		if ((i/sw) > yOffset) {
			track.push_back(i);
			field[i] = BLOCK;
		}
	}

	switch (dir) {
	case 'L':
		t.moveLeft();
		break;
	case 'R':
		t.moveRight();
		break;
	case 'D':
		t.moveDown();
		break;
	}
	
	return true;
}

void Tetris::update(vector<int> & removeRows) {
	if (!removeRows.empty()) {
		int n = removeRows.size();

		// use a simple scoring system here
		score += n * lv * (n >= 4 ? 2 : 1);
		lv = score / 10 + 1;

		swprintf_s(&field[ylv * sw + xlv], 10, L"Level %3d", lv);
		field[ylv * sw + xlv + 9] = ' '; // for some reason the null character is shown, remove the null character
		swprintf_s(&field[yScore * sw + xScore], 10, L"Score %3d", score);
		field[yScore * sw + xScore + 9] = ' '; // same

		// change line to ' '
		for (int i : removeRows) {
			for (int j = xOffset + 1; j <= xOffset + w; j++) {
				if(i>yOffset) field[i * sw + j] = ' ';
			}
		}
		
		myScreen->draw(field);
		Sleep(100); // pause so player can see the "animation"

		// drag all the tetromino down n line
		int lastrow = removeRows.back();
		for (int i = lastrow; i > yOffset + n; i--) {
			for (int j = xOffset + 1; j <= xOffset + w; j++) {
				field[i * sw + j] = field[(i - n) * sw + j];
			}
		}

		myScreen->draw(field);
		Sleep(100); // pause so player can see the "animation"
	}
}


bool Tetris::isGameOver(vector<int>& rowlist) {
	for (int i : rowlist) {
		if (i <= yOffset) return true;
	}
	return false;
}