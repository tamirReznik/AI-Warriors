#pragma once
class Point2D
{
private:
	int row, col;
public:
	Point2D(int r, int c);
	int getRow() { return row; }
	int getCol() { return col; }
};

