#include "Node.h"
#include <math.h>

Node::Node()
{
	row = 0;
	col = 0;
	g = 0;
	h = 0;
	parent = nullptr;
}

// (r,c) are the coordinates of the new Node,
// (tr,tc) are the coordinates of the target. We'll need it to 
// compute Heuristics: h
Node::Node(int r, int c, int tr, int tc, double g, Node* p, double risk)
{
	row = r;
	col = c;
	this->g = g;
	parent = p;
	h = risk + Distance(r, c, tr, tc); // we could use Manhattan Distance

}

Node::Node(int r, int c, double g, Node* p)
{
	row = r;
	col = c;
	this->g = g;
	parent = p;
	h = 0;
}


double Node::Distance(int r1, int c1, int r2, int c2)
{
	return sqrt(pow((r1 - r2), 2) + pow((c1 - c2), 2));
}
double Node::DistanceFromMe(int otherRow, int otherCol)
{
	return sqrt(pow((row - otherRow), 2) + pow((col - otherCol), 2));
}

