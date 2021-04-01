#pragma once
#include "Node.h"

class CompareNodes
{
public:
	bool operator() (Node n1, Node n2)
	{
		return n1.GetF() > n2.GetF();
	}

};

class CompareWarNodes
{
public:


	static bool compareCells(Node* n1, Node* n2);

	bool operator() (Node* n1, Node* n2);


};

