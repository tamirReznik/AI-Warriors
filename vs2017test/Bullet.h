#pragma once
#include "room.h"
#include <stdlib.h>
#include <math.h>
#include "Node.h"
#include "glut.h"
const double SPEED = 0.01;

class Bullet
{
private:
	void (*bulletHitCallBack)(int, int, double, int);
	double x, y;
	int initialRow, initialCol, hitRow, hitCol;
	double damage;
	double dirx, diry;
	bool isMoving, isHit;
	int target;
public:
	Bullet(double cx, double cy, double dx, double dy, int target, void (*bulletHitCallBack)(int, int, double, int));
	Bullet(double cx, double cy);
	void DrawMe();
	void Move(int maze[MSZ][MSZ]);
	void CalculateDamage();
	void UpdateMap(int maze[MSZ][MSZ], double security_map[MSZ][MSZ], double delta);

	void Fire(bool f)
	{
		isMoving = f;
	}
	bool IsMoving()
	{
		return isMoving;
	}
	bool IsHit()
	{
		return isHit;
	}
	void SetIsHit(bool isHit)
	{
		this->isHit = isHit;
	}

	double GetX()
	{
		return x;
	}
	double GetY()
	{
		return y;
	}

	int GetInitialRow()
	{
		return initialRow;
	}
	int GetInitialCol()
	{
		return initialCol;
	}

	int GetHitRow()
	{
		return hitRow;
	}
	int GetHitCol()
	{
		return hitCol;
	}
	double GetDamage()
	{
		return damage;
	}
	int GetTargert()
	{
		return target;
	}

};

