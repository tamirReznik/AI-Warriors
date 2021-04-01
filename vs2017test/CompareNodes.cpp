#include "CompareNodes.h"

bool CompareWarNodes::compareCells(Node* n1, Node* n2)
{
	return n1->GetF() > n2->GetF();
}

bool CompareWarNodes::operator()(Node* n1, Node* n2)
{
	return compareCells(n1, n2);
}
