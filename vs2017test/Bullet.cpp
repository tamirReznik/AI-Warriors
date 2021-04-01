#include "Bullet.h"
#include <iostream>
using namespace std;


Bullet::Bullet(double cx, double cy, double dx, double dy, int target, void (*bulletHitCallBack)(int, int, double, int))
{
	this->bulletHitCallBack = bulletHitCallBack;
	this->target = target;
	x = cx;
	y = cy;
	initialRow = MSZ * (y + 1) / 2.0;
	initialCol = MSZ * (x + 1) / 2.0;
	hitCol = -1;
	hitRow = -1;
	dirx = dx;
	diry = dy;
	damage = .0;
	isHit = false;
	isMoving = false;
}

Bullet::Bullet(double cx, double cy)  // cx,cy are coordinates in {(-1,1),(-1,1)}
{
	this->bulletHitCallBack = nullptr;
	x = cx;
	y = cy;
	initialRow = MSZ * (y + 1) / 2.0;
	initialCol = MSZ * (x + 1) / 2.0;
	double alpha = (rand() % 360) * 3.14 / 180; // alpha in radians
	hitCol = -1;
	hitRow = -1;
	dirx = cos(alpha);
	diry = sin(alpha);
	isMoving = false;
	isHit = false;
	damage = .0;
	this->target = INT_MAX;
}

void Bullet::DrawMe()
{
	if (!this->IsHit())
	{
		glColor3d(0, 0, 0);
		glBegin(GL_POLYGON);
		glVertex2d(x, y + 0.01);
		glVertex2d(x + 0.01, y);
		glVertex2d(x, y - 0.01);
		glVertex2d(x - 0.01, y);
		glEnd();
	}
}

void Bullet::Move(int maze[MSZ][MSZ])
{
	int row, col;
	col = MSZ * (x + 1) / 2.0;
	row = MSZ * (y + 1) / 2.0;

	if (maze[row][col] == WALL
		|| maze[row][col] == BARRIER
		|| maze[row][col] == target)
	{
		hitCol = col;
		hitRow = row;
		SetIsHit(true);
		Fire(false);
		if (maze[row][col] == target)
		{
			CalculateDamage();
		}
	}
	else // moving on
	{
		x += SPEED * dirx;
		y += SPEED * diry;
	}
}
void Bullet::CalculateDamage()
{
	int dx = hitCol - this->GetInitialCol();
	int dy = hitRow - this->GetInitialRow();

	double distance = sqrt(pow(dx, 2) + pow(dy, 2));
	damage = MAX_HP / ((distance + 1) * 2);
	//cout << "distance: " << distance << " dx: " << dx << " dy: " << dy << "\n";

	if (this->bulletHitCallBack != nullptr)
		this->bulletHitCallBack(hitRow, hitCol, damage, target);


}
void Bullet::UpdateMap(int maze[MSZ][MSZ], double security_map[MSZ][MSZ], double delta)
{
	int row, col;
	col = MSZ * (x + 1) / 2.0;
	row = MSZ * (y + 1) / 2.0;

	if (maze[row][col] == WALL || maze[row][col] == BARRIER)
		Fire(false);
	else // moving on
	{
		x += SPEED * dirx;
		y += SPEED * diry;
		security_map[row][col] += (delta);
	}

}
