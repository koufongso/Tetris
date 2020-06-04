#include "Tetromino.h"
#include <iostream>

using namespace std;

Tetromino::Tetromino(char type, int x, int y){
	this->type = type;
	this->x = x;
	this->y = y;
	currentPattern = 0;
	switch (type)
	{
		case 'I':
			for(int i=0; i<4;i++)
				pattern[i] = new bool[16]{	0,0,0,0,
											1,1,1,1,
											0,0,0,0,
											0,0,0,0};
			row = 4;
			col = 4;
			break;
		case 'J':
			for (int i = 0; i < 4; i++)
				pattern[i] = new bool[9]{	1,0,0,
											1,1,1,
											0,0,0 };
			row = 3;
			col = 3;
			break;
		case 'L':
			for (int i = 0; i < 4; i++)
				pattern[i] = new bool[9]{	0,0,1,
											1,1,1,
											0,0,0 };
			row = 3;
			col = 3;
			break;
		case 'O':
			for (int i = 0; i < 4; i++)
				pattern[i] = new bool[4]{	1,1,
											1,1 };
			row = 2;
			col = 2;
			break;
		case 'S':
			for (int i = 0; i < 4; i++)
				pattern[i] = new bool[9]{	0,1,1,
											1,1,0,
											0,0,0 };
			row = 3;
			col = 3;
			break;
		case 'T':
			for (int i = 0; i < 4; i++)
				pattern[i] = new bool[9]{	0,1,0,
											1,1,1,
											0,0,0 };
			row = 3;
			col = 3;
			break;
		case 'Z':
			for (int i = 0; i < 4; i++)
				pattern[i] = new bool[9]{	1,1,0,
											0,1,1,
											0,0,0 };
			row = 3;
			col = 3;
			break;
		default:
			for(int i=0; i< 4; i++) pattern[i] = nullptr;
			row = -1;
			col = -1;
			cerr << "Unknown type of tetromino" << endl;
	}

	if(pattern[0]!=nullptr)
		for (int i = 1; i < 4; i++)
			for(int j=0;j<i;j++) rotate_cw(i);
}

Tetromino::~Tetromino() {
	for(int i=0; i<4; i++) delete[] pattern[i];
}

void Tetromino::rotate() { 
	currentPattern = (currentPattern >= 3 ? 0 : currentPattern + 1); 
}
void Tetromino::moveLeft() { x--; }
void Tetromino::moveRight() { x++; }
void Tetromino::moveDown() { y++; }