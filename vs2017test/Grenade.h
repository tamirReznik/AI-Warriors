#pragma once
#include "Bullet.h"
#include "Room.h"
#include <vector>
const int NUM_BULLETS_IN_GRENADE = 20;

class Grenade
{
private:
	void (*bulletHitCallBack)(int, int, double, int);
	std::vector <Bullet> hitBullets;
	Bullet* bullets[NUM_BULLETS_IN_GRENADE];
	double cx, cy;
	bool isExploded, isDone;
	int target;
public:
	Grenade(double x, double y, int target, void (*bulletHitCallBack)(int, int, double, int));
	Grenade(double x, double y);
	void Explode();

	void Exploding(int maze[MSZ][MSZ]);
	void DrawMe();
	void UpdateSecurityMap(int maze[MSZ][MSZ],
						   double security_map[MSZ][MSZ], double delta);
	bool IsDone()
	{
		return isDone;
	}
	bool IsExploded()
	{
		return isExploded;
	}
	std::vector <Bullet> GetHitBullets()
	{
		return hitBullets;
	}

};

