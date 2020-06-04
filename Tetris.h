#pragma once
#include "screen.h"
#include "Tetromino.h"
#include <vector>

using namespace std;

class Tetris {
public:
	Tetris();
	void run();

private:
	int sw;
	int sh;

	int w;
	int h;
	int xOffset;
	int yOffset;

	int wp;
	int hp;
	int xPreviewOffset;
	int yPreviewOffset;

	int score;
	int xScore;
	int yScore;

	int lv;
	int xlv;
	int ylv;
	
	bool isSet;
	vector<int> track;
	Screen *myScreen;
	wchar_t *field;

	// helper function
	inline bool rotate(Tetromino &);
	inline bool move(Tetromino&, char);
	void update(vector<int>&);
	bool isGameOver(vector<int>&);
};
