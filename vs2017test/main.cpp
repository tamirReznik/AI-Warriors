/*
Made By Tamir Reznik
*/

#include "main.h"
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;


//const int MSZ = 100; // it is defined in Room.h
const int NUM_ROOMS = 12;

int maze[MSZ][MSZ] = { 0 }; // 0 is SPACE
double security_map[MSZ][MSZ] = { 0 };// danger map: 0 means no danger
Room rooms[NUM_ROOMS];

int numOfWarriors = 0;

Warrior* warrior_1_T1, * warrior_2_T1;
Bullet* bulletTeam_1_Warrior_1, * bulletTeam_1_Warrior_2;
Grenade* grenadeTeam_1_Warrior_1, * grenadeTeam_1_Warrior_2;

Warrior* warrior_1_T2, * warrior_2_T2;
Bullet* bulletTeam_2_Warrior_1 = nullptr, * bulletTeam_2_Warrior_2 = nullptr;
Grenade* grenadeTeam_2_Warrior_1 = nullptr, * grenadeTeam_2_Warrior_2 = nullptr;

bool gameStarted = false;
bool showSecurityMap = false;
bool gameOver = false;


void init()
{
	glClearColor(0.8f, 0.7f, 0.5f, .0f);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);

	srand((unsigned int)time(NULL));

	InitMaze();
	InitRooms();
	DigTunnels();
	fillRoomsWithObjects();
	initWarriors();

}

void resetVars()
{
	grenadeTeam_1_Warrior_1 = nullptr;
	grenadeTeam_1_Warrior_2 = nullptr;
	grenadeTeam_2_Warrior_1 = nullptr;
	grenadeTeam_2_Warrior_2 = nullptr;
	bulletTeam_1_Warrior_1 = nullptr;
	bulletTeam_1_Warrior_2 = nullptr;
	bulletTeam_2_Warrior_1 = nullptr;
	bulletTeam_2_Warrior_2 = nullptr;

	gameStarted = false;
	showSecurityMap = false;
	gameOver = false;
}

void initWarriors()
{
	resetVars();
	GenerateSecurityMap();

	int roomTeam_1 = roomAssignment(&warrior_1_T1, TEAM_1, -1, -1);
	roomAssignment(&warrior_2_T1, TEAM_1, roomTeam_1, -1);

	warrior_1_T1->SetTeammate(&warrior_2_T1);
	warrior_1_T1->SetBullet(&bulletTeam_1_Warrior_1);
	warrior_1_T1->SetGrenade(&grenadeTeam_1_Warrior_1);

	warrior_2_T1->SetTeammate(&warrior_1_T1);
	warrior_2_T1->SetBullet(&bulletTeam_1_Warrior_2);
	warrior_2_T1->SetGrenade(&grenadeTeam_1_Warrior_2);

	int roomTeam_2 = roomAssignment(&warrior_1_T2, TEAM_2, -1, roomTeam_1);
	roomAssignment(&warrior_2_T2, TEAM_2, roomTeam_2, -1);

	warrior_1_T2->SetTeammate(&warrior_2_T2);
	warrior_1_T2->SetBullet(&bulletTeam_2_Warrior_1);
	warrior_1_T2->SetGrenade(&grenadeTeam_2_Warrior_1);

	warrior_2_T2->SetTeammate(&warrior_1_T2);
	warrior_2_T2->SetBullet(&bulletTeam_2_Warrior_2);
	warrior_2_T2->SetGrenade(&grenadeTeam_2_Warrior_2);

	/*cout << " enemy room: " << roomTeam_2 << "\n" << "col: " << rooms[roomTeam_2].GetCenterCol() << " row: " << rooms[roomTeam_2].GetCenterRow();
	cout << "\nwarrior 1 t2 Row: " << warrior_1_T2->GetMyNode()->GetRow() << " col: " << warrior_1_T2->GetMyNode()->GetCol();
	cout << "\nwarrior 1 t2 Row: " << warrior_2_T2->GetMyNode()->GetRow() << " col: " << warrior_2_T2->GetMyNode()->GetCol();
	*/
}

// init warrior and place him on random room 
// if specificRoom in NUM_ROOMS range then the warrior will assign there
// if specificRoom NOT in NUM_ROOMS range then the warrior will assign to random room that is not excludeRoom
// if spesific room and excludeRoom NOT in NUM_ROOMS range then the warrior will assign to random room
//returns the chosen room
int roomAssignment(Warrior** warrior, int team, int specificRoom, int excludeRoom)
{

	int randRoom;
	int row, col;
	bool notFound = true;

	if (specificRoom > -1 && specificRoom < NUM_ROOMS)
	{
		randRoom = specificRoom;
	}
	else if (excludeRoom > -1 && excludeRoom < NUM_ROOMS)
	{
		do
		{
			randRoom = rand() % NUM_ROOMS;
		} while (randRoom == excludeRoom);

	}
	else
	{
		randRoom = rand() % NUM_ROOMS;
	}

	do
	{
		row = (rooms[randRoom].GetCenterRow() - rooms[randRoom].GetHeight() / 2) + rand() % rooms[randRoom].GetHeight();
		col = (rooms[randRoom].GetCenterCol() - rooms[randRoom].GetWidth() / 2) + rand() % rooms[randRoom].GetWidth();
		if (maze[row][col] == SPACE)
		{
			notFound = false;
		}
	} while (notFound);

	int opponentTeam = team == TEAM_1 ? TEAM_2 : TEAM_1;
	*warrior = new Warrior(team, opponentTeam, row, col, NUM_ROOMS, &numOfWarriors, rooms, nullptr, security_map, warriorGotHit, &gameOver);
	numOfWarriors++;
	return randRoom;
}

// init the whole space with WALLS
void InitMaze()
{
	int i, j;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			maze[i][j] = WALL;
			security_map[i][j] = 0;
		}

}

void InitRooms()
{
	int i, j;
	int w, h, row, col;
	int min = 10;
	bool overlap;

	for (i = 0; i < NUM_ROOMS; i++)
	{
		do
		{
			overlap = false;
			w = min + rand() % 6 + 6;
			h = min + rand() % 6 + 6;
			col = 2 + w / 2 + rand() % (MSZ - w - 4);
			row = 2 + h / 2 + rand() % (MSZ - h - 4);
			// check if the above definitions don't make a room to overlap with any other room
			for (j = 0; j < i && !overlap; j++)
				if (rooms[j].IsOverlap(w, h, row, col))
					overlap = true;
		} while (overlap);

		rooms[i].SetWidth(w);
		rooms[i].SetHeigth(h);
		rooms[i].SetCenterX(col);
		rooms[i].SetCenterY(row);
		rooms[i].FillMaze(maze);


	}

}

void fillRoomsWithObjects()
{
	int i;
	int medic_2, medic_1;
	int ammo_2, ammo_1;

	for (i = 0; i < NUM_ROOMS; i++)
		rooms[i].BuildBarrier(maze);

	//add ammo and medicines stations
	medic_1 = rand() % NUM_ROOMS;
	do
	{
		medic_2 = rand() % NUM_ROOMS;
	} while (medic_2 == medic_1);

	do
	{
		ammo_1 = rand() % NUM_ROOMS;
	} while (ammo_1 == medic_1 || ammo_1 == medic_2);

	do
	{
		ammo_2 = rand() % NUM_ROOMS;
	} while (ammo_2 == ammo_1 || ammo_2 == medic_1 || ammo_2 == medic_2);

	rooms[medic_1].BuildFirstAidStation(maze);
	rooms[medic_2].BuildFirstAidStation(maze);

	rooms[ammo_1].BuildAmmunitionBunker(maze);
	rooms[ammo_2].BuildAmmunitionBunker(maze);

}

// the Node pn is already in pq. we need to replace it with a new copy (pn) because
// the new copy has a better f 
void UpdatePQ(priority_queue <Node, vector<Node>, CompareNodes>& pq, Node* pn)
{
	vector <Node> tmp;
	Node best = pq.top();

	while (!(best.operator==(*pn) && !pq.empty()))
	{
		pq.pop();
		tmp.push_back(best);
		best = pq.top();
	}

	// we either found pn in pq or pq got empty
	if (!pq.empty())
	{
		pq.push(*pn);
	}

	while (!tmp.empty())
	{
		best = tmp.at(tmp.size() - 1);
		pq.push(best);
		tmp.pop_back();
	}
}

// row, col are the coordinates of a new Node,
// trow, tcol are the coordinates of a target
void CheckNeighbor(Node* pcurrent, int row, int col, int trow, int tcol,
				   vector <Node>& grays, vector <Node>& blacks,
				   priority_queue <Node, vector<Node>, CompareNodes>& pq)
{
	Node* pneighbor;
	double space_cost = 0.1, wall_cost = 1.5, cost;
	vector <Node>::iterator it_gray;
	vector <Node>::iterator it_black;


	// the cost of motion to the new Node depends on whather it is SPACE or WALL
	if (maze[row][col] == SPACE)
		cost = space_cost;
	else cost = wall_cost;
	pneighbor = new Node(row, col, trow, tcol, pcurrent->GetG() + cost, pcurrent, 0);
	// now add it to pq if 
	// 1. it is white
	// 2. it is gray but it improves f of this Node 
	// 3. it is "white" target
	// 4. it is "gray" target and it has f better than the previous target
	it_gray = find(grays.begin(), grays.end(), *pneighbor);
	it_black = find(blacks.begin(), blacks.end(), *pneighbor);
	// it is white if it is not black and not grays and it is not target
	if (it_gray != grays.end())// && it_black == blacks.end()) // it is gray
	{
		if (pneighbor->GetF() < (*it_gray).GetF())
		{
			UpdatePQ(pq, pneighbor);
			// update this neighbor in grays
			grays.erase(it_gray);
			grays.push_back(*pneighbor);
		}
	}
	else 	if (it_black != blacks.end())
	{
		delete(pneighbor);
	}
	else // it is white
	{
		// paint it gray
		grays.push_back(*pneighbor);
		// add it to pq
		pq.push(*pneighbor);
	}
}

void CreatePath(Node* pn)
{
	while (fabs(pn->GetG()) > 0.01) // only at START g=0
	{
		maze[pn->GetRow()][pn->GetCol()] = SPACE;
		pn = pn->GetParent();
	}
}

// run A* that finds the "best" path from rooms[index1] to rooms[index2]
void DigTunnel(int index1, int index2)
{
	bool target_found = false;
	int r, c, tr, tc;

	Node* pcurrent;
	vector <Node> grays;
	vector <Node> blacks;
	priority_queue <Node, vector<Node>, CompareNodes> pq;
	vector <Node>::iterator it_gray;
	// create start Node
	r = rooms[index1].GetCenterRow();
	c = rooms[index1].GetCenterCol();
	tr = rooms[index2].GetCenterRow();
	tc = rooms[index2].GetCenterCol();
	Node pn = *(new Node(r, c, tr, tc, 0, nullptr, 0));
	// add it to pq and paint it gray
	pq.push(pn);
	grays.push_back(pn);

	while (!pq.empty() && !target_found)
	{
		// remove the top Node from pq
		pcurrent = new Node(pq.top());
		// check that it is not a target. If it is target than we just stop A*
		if (fabs(pcurrent->GetH()) < 0.01) // this is target
		{

			CreatePath(pcurrent);
			return;
		}
		pq.pop();
		// and paint it black
		it_gray = find(grays.begin(), grays.end(), *pcurrent);
		// remove pcurrent from grays
		if (it_gray != grays.end()) // pcurrent has been found
			grays.erase(it_gray);
		// and add it to blacks
		blacks.push_back(*pcurrent);
		// check the neighbor Nodes of pcurrent
		// check UP
		if (pcurrent->GetRow() < MSZ - 1)
		{
			CheckNeighbor(pcurrent, pcurrent->GetRow() + 1, pcurrent->GetCol(), tr, tc, grays, blacks, pq);
		}
		// check DOWN
		if (pcurrent->GetRow() > 0)
		{
			CheckNeighbor(pcurrent, pcurrent->GetRow() - 1, pcurrent->GetCol(), tr, tc, grays, blacks, pq);
		}
		// check left
		if (pcurrent->GetCol() > 0)
		{
			CheckNeighbor(pcurrent, pcurrent->GetRow(), pcurrent->GetCol() - 1, tr, tc, grays, blacks, pq);
		}
		// check right
		if (pcurrent->GetCol() < MSZ - 1)
		{
			CheckNeighbor(pcurrent, pcurrent->GetRow(), pcurrent->GetCol() + 1, tr, tc, grays, blacks, pq);
		}
	}
}

void DigTunnels()
{
	int i, j;

	for (i = 0; i < NUM_ROOMS; i++)
	{
		for (j = i + 1; j < NUM_ROOMS; j++)
		{
			DigTunnel(i, j); // A*
			cout << "Tunnel from " << i << " to " << j << " is ready\n";
		}
	}
	printTeamDeathmatch();
}

void DrawMaze()
{
	int i, j;
	double sx, sy; // cell size
	double x, y;

	sx = 2.0 / MSZ;
	sy = 2.0 / MSZ;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j])
			{
			case SPACE:
				if (showSecurityMap)
					glColor3d(1 - security_map[i][j], 1 - security_map[i][j], 1 - security_map[i][j]);
				else
					glColor3d(1, 1, 1);   // white

				break;
			case WALL:
				glColor3d(0.4, 0.0, 0.2);   // dark-red
				break;
			case MEDICINES:
				glColor3d(1, 0.2, 0.2);   // red
				break;
			case BARRIER:
				glColor3d(0.3, 0.2, 1);   // blue
				break;
			case TARGET:
				glColor3d(1, 0, 0);   // red
				break;
			case GRAY:
				glColor3d(0, 0.6, 0);   // dark green
				break;
			case BLACK:
				glColor3d(0.6, 1, 0.8);   // green
				break;
			case PATH:
				glColor3d(0.8, 0.4, 1);   // magenta
				break;
			case AMMO:
				glColor3d(0, 0.8, 0);   // green
				break;
			case TEAM_1:
				glColor3d(1.0, 0.4, .0);   // orange
				break;
			case TEAM_2:
				glColor3d(0.75, 0.1, 0.75);   // purple
				break;


			}
			// draw square maze[i][j]
			x = 2 * (j / (double)MSZ) - 1;
			y = 2 * (i / (double)MSZ) - 1;

			glBegin(GL_POLYGON);  // fill up
			glVertex2d(x, y);
			glVertex2d(x, y + sy);
			glVertex2d(x + sx, y + sy);
			glVertex2d(x + sx, y);
			glEnd();
			glColor3d(0, 0, 0);
			/*			glBegin(GL_LINE_LOOP);  // boundaries
						glVertex2d(x, y);
						glVertex2d(x, y + sy);
						glVertex2d(x + sx, y + sy);
						glVertex2d(x + sx, y);
						glEnd();
						*/
		}
}

// changes the security_map matrix
void GenerateSecurityMap()
{
	int num_explosions = 5000;
	int counter;
	int x, y;
	double  xx, yy;
	Grenade* pg1;

	for (counter = 1; counter <= num_explosions; counter++)
	{
		x = rand() % WIDTH; // x and y are random pixels
		y = rand() % HEIGHT;
		xx = (2 * x / (double)WIDTH) - 1;
		yy = (2 * y / (double)HEIGHT) - 1; // now xx,yy are in range [-1,1]
		pg1 = new Grenade(xx, yy);
		pg1->UpdateSecurityMap(maze, security_map, HIT_WEIGHT_SECURITY_MAP);
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMaze();

	displayWar();

	glutSwapBuffers(); // show all
}

void displayWar()
{
	warrior_1_T1->DrawMe(maze);
	warrior_2_T1->DrawMe(maze);
	warrior_1_T2->DrawMe(maze);
	warrior_2_T2->DrawMe(maze);

}

void warriorGotHit(int row, int col, double damage, int target)
{
	Warrior** enemy_1, ** enemy_2;
	if (target == TEAM_1)
	{
		enemy_1 = &warrior_1_T1;
		enemy_2 = &warrior_2_T1;
	}
	else if (target == TEAM_2)
	{
		enemy_1 = &warrior_1_T2;
		enemy_2 = &warrior_2_T2;

	}
	else
	{
		return;
	}
	double enemyHp;
	if ((*enemy_1)->GetMyNode()->DistanceFromMe(row, col) == 0)
	{
		enemyHp = (*enemy_1)->GetHp();
		(*enemy_1)->SetHp(max(enemyHp - damage, .0));
	}
	else if ((*enemy_2)->GetMyNode()->DistanceFromMe(row, col) == 0)
	{
		enemyHp = (*enemy_2)->GetHp();
		(*enemy_2)->SetHp(max(enemyHp - damage, .0));
	}


}

void announceWinner()
{

	if (warrior_1_T1->GetHp() <= 0 && warrior_2_T1->GetHp() <= 0
		&& !(warrior_1_T2->GetHp() <= 0 && warrior_2_T2->GetHp() <= 0))
		printTeamTwoWon();
	else if (warrior_1_T2->GetHp() <= 0 && warrior_2_T2->GetHp() <= 0
			 && !(warrior_1_T1->GetHp() <= 0 && warrior_2_T1->GetHp() <= 0))
		printTeamOneWon();
	else
		printDraw();

	cout << "\n\nFinal Score: \n";
	printGameStatus();

}

void printTeamOneWon()
{
	//  $$$$$$$$\ $$$$$$$$\  $$$$$$\  $$\      $$\         $$\         $$\      $$\  $$$$$$\  $$\   $$\ 
	//  \__$$  __|$$  _____|$$  __$$\ $$$\    $$$ |      $$$$ |        $$ | $\  $$ |$$  __$$\ $$$\  $$ |
	//     $$ |   $$ |      $$ /  $$ |$$$$\  $$$$ |      \_$$ |        $$ |$$$\ $$ |$$ /  $$ |$$$$\ $$ |
	//     $$ |   $$$$$\    $$$$$$$$ |$$\$$\$$ $$ |        $$ |        $$ $$ $$\$$ |$$ |  $$ |$$ $$\$$ |
	//     $$ |   $$  __|   $$  __$$ |$$ \$$$  $$ |        $$ |        $$$$  _$$$$ |$$ |  $$ |$$ \$$$$ |
	//     $$ |   $$ |      $$ |  $$ |$$ |\$  /$$ |        $$ |        $$$  / \$$$ |$$ |  $$ |$$ |\$$$ |
	//     $$ |   $$$$$$$$\ $$ |  $$ |$$ | \_/ $$ |      $$$$$$\       $$  /   \$$ | $$$$$$  |$$ | \$$ |
	//     \__|   \________|\__|  \__|\__|     \__|      \______|      \__/     \__| \______/ \__|  \__|

	cout << "  $$$$$$$$\\ $$$$$$$$\\  $$$$$$\\  $$\\      $$\\         $$\\         $$\\      $$\\  $$$$$$\\  $$\\   $$\\ \n"
		<< "  \\__$$  __|$$  _____|$$  __$$\\ $$$\\    $$$ |      $$$$ |        $$ | $\\  $$ |$$  __$$\\ $$$\\  $$ |\n"
		<< "     $$ |   $$ |      $$ /  $$ |$$$$\\  $$$$ |      \\_$$ |        $$ |$$$\\ $$ |$$ /  $$ |$$$$\\ $$ |\n"
		<< "     $$ |   $$$$$\\    $$$$$$$$ |$$\\$$\\$$ $$ |        $$ |        $$ $$ $$\\$$ |$$ |  $$ |$$ $$\\$$ |\n"
		<< "     $$ |   $$  __|   $$  __$$ |$$ \\$$$  $$ |        $$ |        $$$$  _$$$$ |$$ |  $$ |$$ \\$$$$ |\n"
		<< "     $$ |   $$ |      $$ |  $$ |$$ |\\$  /$$ |        $$ |        $$$  / \\$$$ |$$ |  $$ |$$ |\\$$$ |\n"
		<< "     $$ |   $$$$$$$$\\ $$ |  $$ |$$ | \\_/ $$ |      $$$$$$\\       $$  /   \\$$ | $$$$$$  |$$ | \\$$ |\n"
		<< "     \\__|   \\________|\\__|  \\__|\\__|     \\__|      \\______|      \\__/     \\__| \\______/ \\__|  \\__|\n\n";
}

void printTeamTwoWon()
{
	//  $$$$$$$$\ $$$$$$$$\  $$$$$$\  $$\      $$\        $$$$$$\        $$\      $$\  $$$$$$\  $$\   $$\ 
	//  \__$$  __|$$  _____|$$  __$$\ $$$\    $$$ |      $$  __$$\       $$ | $\  $$ |$$  __$$\ $$$\  $$ |
	//     $$ |   $$ |      $$ /  $$ |$$$$\  $$$$ |      \__/  $$ |      $$ |$$$\ $$ |$$ /  $$ |$$$$\ $$ |
	//     $$ |   $$$$$\    $$$$$$$$ |$$\$$\$$ $$ |       $$$$$$  |      $$ $$ $$\$$ |$$ |  $$ |$$ $$\$$ |
	//     $$ |   $$  __|   $$  __$$ |$$ \$$$  $$ |      $$  ____/       $$$$  _$$$$ |$$ |  $$ |$$ \$$$$ |
	//     $$ |   $$ |      $$ |  $$ |$$ |\$  /$$ |      $$ |            $$$  / \$$$ |$$ |  $$ |$$ |\$$$ |
	//     $$ |   $$$$$$$$\ $$ |  $$ |$$ | \_/ $$ |      $$$$$$$$\       $$  /   \$$ | $$$$$$  |$$ | \$$ |
	//     \__|   \________|\__|  \__|\__|     \__|      \________|      \__/     \__| \______/ \__|  \__|
	cout << "  $$$$$$$$\\ $$$$$$$$\\  $$$$$$\\  $$\\      $$\\        $$$$$$\\        $$\\      $$\\  $$$$$$\\  $$\\   $$\\ \n"
		<< "  \\__$$  __|$$  _____|$$  __$$\\ $$$\\    $$$ |      $$  __$$\\       $$ | $\\  $$ |$$  __$$\\ $$$\\  $$ |\n"
		<< "     $$ |   $$ |      $$ /  $$ |$$$$\\  $$$$ |      \\__/  $$ |      $$ |$$$\\ $$ |$$ /  $$ |$$$$\\ $$ |\n"
		<< "     $$ |   $$$$$\\    $$$$$$$$ |$$\\$$\\$$ $$ |       $$$$$$  |      $$ $$ $$\\$$ |$$ |  $$ |$$ $$\\$$ |\n"
		<< "     $$ |   $$  __|   $$  __$$ |$$ \\$$$  $$ |      $$  ____/       $$$$  _$$$$ |$$ |  $$ |$$ \\$$$$ |\n"
		<< "     $$ |   $$ |      $$ |  $$ |$$ |\\$  /$$ |      $$ |            $$$  / \\$$$ |$$ |  $$ |$$ |\\$$$ |\n"
		<< "     $$ |   $$$$$$$$\\ $$ |  $$ |$$ | \\_/ $$ |      $$$$$$$$\\       $$  /   \\$$ | $$$$$$  |$$ | \\$$ |\n"
		<< "     \\__|   \\________|\\__|  \\__|\\__|     \\__|      \\________|      \\__/     \\__| \\______/ \\__|  \\__|\n";
}

void printDraw()
{
	//  $$$$$$\ $$$$$$$$\ $$\  $$$$$$\         $$$$$$\        $$$$$$$\  $$$$$$$\   $$$$$$\  $$\      $$\ 
	//  \_$$  _|\__$$  __|$  |$$  __$$\       $$  __$$\       $$  __$$\ $$  __$$\ $$  __$$\ $$ | $\  $$ |
	//    $$ |     $$ |   \_/ $$ /  \__|      $$ /  $$ |      $$ |  $$ |$$ |  $$ |$$ /  $$ |$$ |$$$\ $$ |
	//    $$ |     $$ |       \$$$$$$\        $$$$$$$$ |      $$ |  $$ |$$$$$$$  |$$$$$$$$ |$$ $$ $$\$$ |
	//    $$ |     $$ |        \____$$\       $$  __$$ |      $$ |  $$ |$$  __$$< $$  __$$ |$$$$  _$$$$ |
	//    $$ |     $$ |       $$\   $$ |      $$ |  $$ |      $$ |  $$ |$$ |  $$ |$$ |  $$ |$$$  / \$$$ |
	//  $$$$$$\    $$ |       \$$$$$$  |      $$ |  $$ |      $$$$$$$  |$$ |  $$ |$$ |  $$ |$$  /   \$$ |
	//  \______|   \__|        \______/       \__|  \__|      \_______/ \__|  \__|\__|  \__|\__/     \__|
	cout << "\n\n  $$$$$$\\ $$$$$$$$\\ $$\\  $$$$$$\\         $$$$$$\\        $$$$$$$\\  $$$$$$$\\   $$$$$$\\  $$\\      $$\\ \n"
		<< "  \\_$$  _|\\__$$  __|$  |$$  __$$\\       $$  __$$\\       $$  __$$\\ $$  __$$\\ $$  __$$\\ $$ | $\\  $$ |\n"
		<< "    $$ |     $$ |   \\_/ $$ /  \\__|      $$ /  $$ |      $$ |  $$ |$$ |  $$ |$$ /  $$ |$$ |$$$\\ $$ |\n"
		<< "    $$ |     $$ |       \\$$$$$$\\        $$$$$$$$ |      $$ |  $$ |$$$$$$$  |$$$$$$$$ |$$ |$$ $$\$$ |\n"
		<< "    $$ |     $$ |        \\____$$\\       $$  __$$ |      $$ |  $$ |$$  __$$| $$  __$$ |$$$$  _$$$$ |\n"
		<< "    $$ |     $$ |       $$\\   $$ |      $$ |  $$ |      $$ |  $$ |$$ |  $$ |$$ |  $$ |$$$  / \\$$$ |\n"
		<< "  $$$$$$\\    $$ |       \\$$$$$$  |      $$ |  $$ |      $$$$$$$  |$$ |  $$ |$$ |  $$ |$$  /   \\$$ |\n"
		<< "  \\______|   \\__|        \\______/       \\__|  \\__|      \\_______/ \\__|  \\__|\\__|  \\__|\\__/     \\__|\n\n\n";
}

void printTeamDeathmatch()
{
	//    ______                           ____                __   __                       __         __  
	//   /_  __/___   ____ _ ____ ___     / __ \ ___   ____ _ / /_ / /_   ____ ___   ____ _ / /_ _____ / /_ 
	//    / /  / _ \ / __ `// __ `__ \   / / / // _ \ / __ `// __// __ \ / __ `__ \ / __ `// __// ___// __ \
	//   / /  /  __// /_/ // / / / / /  / /_/ //  __// /_/ // /_ / / / // / / / / // /_/ // /_ / /__ / / / /
	//  /_/   \___/ \__,_//_/ /_/ /_/  /_____/ \___/ \__,_/ \__//_/ /_//_/ /_/ /_/ \__,_/ \__/ \___//_/ /_/ 
	//   
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
		<< "   Left Click On The Screen To Show Score...\n\n"
		<< "    ______                           ____                __   __                       __         __  \n"
		<< "   /_  __/___   ____ _ ____ ___     / __ \\ ___   ____ _ / /_ / /_   ____ ___   ____ _ / /_ _____ / /_ \n"
		<< "    / /  / _ \\ / __ `// __ `__ \\   / / / // _ \\ / __ `// __// __ \\ / __ `__ \\ / __ `// __// ___// __ \\\n"
		<< "   / /  /  __// /_/ // / / / / /  / /_/ //  __// /_/ // /_ / / / // / / / / // /_/ // /_ / /__ / / / /\n"
		<< "  /_/   \\___/ \\__,_//_/ /_/ /_/  /_____/ \\___/ \\__,_/ \\__//_/ /_//_/ /_/ /_/ \\__,_/ \\__/ \\___//_/ /_/ \n";


}

void warriorsCalculations()
{

	warrior_1_T1->calculateNextStep(maze);
	warrior_2_T1->calculateNextStep(maze);
	warrior_1_T2->calculateNextStep(maze);
	warrior_2_T2->calculateNextStep(maze);

	if (gameOver)
	{
		gameStarted = false;
		printGameOver();
		announceWinner();

	}

	Sleep(120);
}

void idle()
{

	if (gameStarted && !gameOver)
		warriorsCalculations();

	if (gameOver && !gameStarted)
		gameStarted = true;

	glutPostRedisplay(); // indirect call to display
}

void printGameOver()
{
	cout << "\n\n    /$$$$$$\n"
		<< "   /$$__  $$\n"
		<< "  | $$  \\__/  /$$$$$$  /$$$$$$/$$$$   /$$$$$$         /$$$$$$  /$$    /$$ /$$$$$$   /$$$$$$\n"
		<< "  | $$ /$$$$ |____  $$| $$_  $$_  $$ /$$__  $$       /$$__  $$|  $$  /$$//$$__  $$ /$$__  $$\n"
		<< "  | $$|_  $$  /$$$$$$$| $$ \\ $$ \\ $$| $$$$$$$$      | $$  \\ $$ \\  $$/$$/| $$$$$$$$| $$  \\__/\n"
		<< "  | $$  \\ $$ /$$__  $$| $$ | $$ | $$| $$_____/      | $$  | $$  \\  $$$/ | $$_____/| $$\n"
		<< "  |  $$$$$$/|  $$$$$$$| $$ | $$ | $$|  $$$$$$$      |  $$$$$$/   \\  $/  |  $$$$$$$| $$\n"
		<< "   \\______/  \\_______/|__/ |__/ |__/ \\_______/       \\______/     \\_/    \\_______/|__/\n\n\n";
}

void menu(int choice)
{
	switch (choice)
	{
	case 1: // start/stop battle
		gameStarted = !gameStarted;
		break;
	case 2: // show security map
		showSecurityMap = !showSecurityMap;
		break;
	case 3: // start new game
		init();
		break;

	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printGameStatus();
	}
}

void printGameStatus()
{
	cout << "\n##################################"
		<< "\nTeam 1 (Orange): \n"
		<< "\n  Warrior 1: "
		<< "\n    HP: " << warrior_1_T1->GetHp()
		<< "\n    Ammo: " << warrior_1_T1->GetAmmo()
		<< "\n\n  Warrior 2: "
		<< "\n    HP: " << warrior_2_T1->GetHp()
		<< "\n    Ammo: " << warrior_2_T1->GetAmmo() << "\n"
		<< "\n\nTeam 2 (Purple): \n"
		<< "\n  Warrior 1: "
		<< "\n    HP: " << warrior_1_T2->GetHp()
		<< "\n    Ammo: " << warrior_1_T2->GetAmmo()
		<< "\n\n  Warrior 2: "
		<< "\n    HP: " << warrior_2_T2->GetHp()
		<< "\n    Ammo: " << warrior_2_T2->GetAmmo() << "\n"
		<< "##################################\n";
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("AI Warriors - Team Deathmatch");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutMouseFunc(mouse);

	// menu
	glutCreateMenu(menu);
	//glutAddMenuEntry("Fire bullet", 1);
	//glutAddMenuEntry("Throw grenade", 2);
	glutAddMenuEntry("Start/Stop Battle", 1);
	glutAddMenuEntry("Show/Hide Security Map", 2);
	glutAddMenuEntry("New Game", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();

	glutMainLoop();

	return 0;
}