#pragma once
 // colors of cell



class Cell
{
private:
	int row, col;
	int color;
	double g; // distance from start 
	Cell* parent;
public:
	Cell();
	Cell(int r, int c, Cell* pc);
	int GetRow();
	int GetCol();
	Cell* GetParent();
	int GetColor();
	void SetRow(int r);
	void SetCol(int c);
};

