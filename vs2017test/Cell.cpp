#include "Cell.h"

Cell::Cell()
{
}

Cell::Cell(int r, int c, Cell* pc)
{
	row = r;
	col = c;
	parent = pc;
}

int Cell::GetRow()
{
	return row;
}

int Cell::GetCol()
{
	return col;
}

Cell* Cell::GetParent()
{
	return parent;
}

int Cell::GetColor()
{
	return color;
}

void Cell::SetRow(int r)
{
	row = r;
}

void Cell::SetCol(int c)
{
	col = c;
}

void Cell::SetColor(int c)
{
	color = c;
}
