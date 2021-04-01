#pragma once
#include "Room.h"
#include "Node.h"
#include <vector>
#include <queue>
#include "Bullet.h"
#include "Grenade.h"
#include "CompareNodes.h"
#include <iostream>
#include <queue>
#include <stack>
#include <math.h>

class Warrier
{
	const int FIRST_AID_INIT_AMOUNT = 1;
	const int FIRST_AID_HP = 30;
	const int MAX_AMMO = 60;
private:
	enum status
	{
		BETWEEN_ROOMS, IN_ROOM, IDLE
	};
	bool* isGameOver;
	int myStatus;
	int myMaze[MSZ][MSZ];
	double mySecurityMap[MSZ][MSZ];
	void (*warrierGotHitCallBack)(int, int, double, int);
	Bullet** myBullet;
	Grenade** myGrenade;
	Warrier** myTeammate;
	Room* rooms;
	int firstAid;
	int radarRate, rateCounter, * numOfWarriers;
	int myTeam, enemyTeam, numOfRooms;
	int myRoom_targetRoom[2];// index 0 = my current room| index 1 = target room
	int currentTarget;
	Node* myNode;
	Node* targetNode;
	Node* ObjectsInRoom[3]; //index : 0->enemy | 1-> HP | 2-> AMMO | or -1 if not exist in room 
	double hp, hpThreshold;
	int myAmmo;
	std::stack<Node*> path;

	//Methods

	bool CheckNeighborClosestTarget(Node* current, int row, int col, bool* bfs_is_on, std::vector <Node*>* BFSGrays, int target);
	bool findClosestTarget(int maze[MSZ][MSZ], int target);
	bool closeRadar(int maze[MSZ][MSZ]);
	int checkMyCurrentRoom();
	int whichRoom(Node* node, int farFromEdge);
	bool CheckNeighborRadar(Node* current, int row, int col, bool* bfs_is_on, std::vector <Node*>* BFSGrays);
	void copyMaze(int maze[MSZ][MSZ]);
	void copySecurityMap(double maze[MSZ][MSZ]);
	void RestorePath(Node* current);
	double distanceFromTeammate(int row, int col);
	bool findRoomsIndex(Node* myNode, Node* targetNode);
	void warrierStep(int maze[MSZ][MSZ]);
	void checkMySurroundingForAmmoAndHP(int maze[MSZ][MSZ]);
	void shareAmmoAndHp();
	void findPathInsideRoom(int maze[MSZ][MSZ], Node* target);
	bool CheckNeighborInsideRoom(Node* current, Node* neighbor, std::priority_queue<Node*, std::vector<Node*>, CompareWarNodes>* aStarGrays);
	void initObjectsInRoom();
	void calculateAttacks(int maze[MSZ][MSZ]);
	void calcStepTargetExist(int maze[MSZ][MSZ]);
	void shootToKill(int maze[MSZ][MSZ]);
	void calcStepTargetNOTExist(int maze[MSZ][MSZ]);
	void findPath(int maze[MSZ][MSZ]);
	bool findSafeSpotInRoom(int maze[MSZ][MSZ]);
	bool CheckNeighborSafeSpotInRoom(Node* current, int row, int col, bool* bfs_is_on, std::vector <Node*>* BFSGrays);
	void findPathBetweenRooms(int targetRoom, int maze[MSZ][MSZ]);
	bool CheckNeighborBetweenRooms(Node* current, Node* neighbor, std::priority_queue<Node*, std::vector<Node*>, CompareWarNodes>* aStarGrays, int targetRoom);

public:


	Warrier(int team, int enemyTeam, int row, int col, int number_of_rooms, int* rate, Room* gameRooms, Warrier** teammate, double security_map[MSZ][MSZ], void (*warrierGotHit)(int, int, double, int), bool* gameOver);
	void DrawMe(int maze[MSZ][MSZ]);
	void calculateNextStep(int maze[MSZ][MSZ]);


	void SetFirstAid(int newFirstAid)
	{
		firstAid = newFirstAid;
	}
	void SetHp(double hp)
	{
		this->hp = hp;
	}
	double GetHp()
	{
		return hp;
	}
	void SetAmmo(int ammo)
	{
		this->myAmmo = ammo;
	}

	int GetAmmo()
	{
		return this->myAmmo;
	}

	Node* GetMyNode()
	{
		return myNode;
	}
	void SetMyNode(Node* newNode)
	{
		myNode = newNode;
	}

	void SetTeammate(Warrier** teammate)
	{
		this->myTeammate = teammate;
	}
	Warrier** GetTeammate()
	{
		return myTeammate;
	}
	int GetTeam()
	{
		return myTeam;
	}
	void SetBullet(Bullet** bullet)
	{
		myBullet = bullet;
	}
	void SetGrenade(Grenade** grenade)
	{
		myGrenade = grenade;
	}
};

