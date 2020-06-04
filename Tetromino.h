#pragma once
class Tetromino
{
public:
	Tetromino(char,int,int);
	~Tetromino();
	void rotate();	// cw rotation
	void moveLeft();
	void moveRight();
	void moveDown();

private:
	char type;	// type of the tetromino
	int x;		// tetromino left-top position
	int y;
	bool *pattern[4];     // tetromino cubes relative position
	int row;
	int col;
	int currentPattern;

	// helper funciton
	inline void swap(int k, int x1, int x2) {
		bool temp = pattern[k][x1];
		pattern[k][x1] = pattern[k][x2];
		pattern[k][x2] = temp;
	}

	void rotate_cw(int k) {
		for (int i = 0; i < row; i++)
			for (int j = i+1; j < col; j++)
				swap(k, i * col + j, j * col + i);

		for (int i = 0; i < row; i++)
			for (int j = 0; j < col / 2; j++)
				swap(k, i * col + j, i * col + (col - j - 1));
	}

	friend class Tetris;
};

