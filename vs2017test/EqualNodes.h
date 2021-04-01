#pragma once
#include "Node.h"

class EqualNodes
{
private: 
	Node* pn;
	EqualNodes(Node* p) { pn = p; };

	bool operator ()(Node* pother)
	{
		return pn->GetRow() == pother->GetRow() && pn->GetCol() == pother->GetCol();
	}

};

