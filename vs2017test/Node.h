#pragma once

class Node
{
private:
	int row, col;
	Node* parent;
	double g, h;
public:
	Node();
	Node(int r, int c, double g, Node* p);
	Node(int r, int c, int tr, int tc, double g, Node* p, double risk);
	double Distance(int r1, int c1, int r2, int c2);
	double DistanceFromMe(int otherRow, int otherCol);
	void SetRow(int r)
	{
		row = r;
	};
	void SetCol(int c)
	{
		col = c;
	};
	void SetG(double g)
	{
		this->g = g;
	};
	void SetParent(Node* parent)
	{
		this->parent = parent;
	}
	int GetRow()
	{
		return row;
	};
	int GetCol()
	{
		return col;
	};
	Node* GetParent()
	{
		return parent;
	};
	double GetF()
	{
		return g + h;
	};
	double GetG()
	{
		return g;
	};
	double GetH()
	{
		return h;
	};

	void SetH(double h)
	{
		this->h = h;
	};

	bool operator == (const Node& other)
	{
		return row == other.row && col == other.col;
	}
};

