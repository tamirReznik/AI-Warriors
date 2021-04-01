#include "Grenade.h"
#include <math.h>
// x and y are in range [-1,1]
Grenade::Grenade(double x, double y, int target, void (*bulletHitCallBack)(int, int, double, int))
{
	this->bulletHitCallBack = bulletHitCallBack;
	this->target = target;
	isDone = false;
	int i;
	double dx, dy, alpha, teta = 2 * 3.14 / NUM_BULLETS_IN_GRENADE;
	cx = x;
	cy = y;

	for (i = 0, alpha = 0; i < NUM_BULLETS_IN_GRENADE; i++, alpha += teta)
	{
		dx = cos(alpha);
		dy = sin(alpha);
		bullets[i] = new Bullet(cx, cy, dx, dy, target, bulletHitCallBack);
		isExploded = false;
	}
}
Grenade::Grenade(double x, double y)
{
	isDone = false;
	int i;
	double dx, dy, alpha, teta = 2 * 3.14 / NUM_BULLETS_IN_GRENADE;
	cx = x;
	cy = y;

	for (i = 0, alpha = 0; i < NUM_BULLETS_IN_GRENADE; i++, alpha += teta)
	{
		dx = cos(alpha);
		dy = sin(alpha);
		bullets[i] = new Bullet(cx, cy, dx, dy, target, nullptr);
		isExploded = false;
	}
}

void Grenade::Explode()
{
	int i;

	isExploded = true;
	for (i = 0; i < NUM_BULLETS_IN_GRENADE; i++)
		bullets[i]->Fire(true);
}

void Grenade::Exploding(int maze[MSZ][MSZ])
{
	if (!hitBullets.empty())
	{
		hitBullets.clear();
	}

	int i;
	int explosionsCounter = NUM_BULLETS_IN_GRENADE;
	for (i = 0; i < NUM_BULLETS_IN_GRENADE; i++)
	{
		if (bullets[i]->IsMoving())
			bullets[i]->Move(maze);
		else
			bullets[i]->Fire(false);

		if (bullets[i]->IsHit())
		{
			explosionsCounter--;
			if (bullets[i]->GetDamage() > 0)
			{
				hitBullets.push_back(*(bullets[i]));
			}

		}
	}
	if (explosionsCounter == 0)
	{
		isDone = true;

	}
}

void Grenade::DrawMe()
{
	int i;

	for (i = 0; i < NUM_BULLETS_IN_GRENADE; i++)
	{
		bullets[i]->DrawMe();
	}
}

void Grenade::UpdateSecurityMap(int maze[MSZ][MSZ], double security_map[MSZ][MSZ], double delta)
{
	int i;
	bool is_moving = true;

	Explode(); // set bullets on fire
	while (is_moving)
	{
		is_moving = false;
		for (i = 0; i < NUM_BULLETS_IN_GRENADE; i++)
		{
			if (bullets[i]->IsMoving())
			{
				bullets[i]->UpdateMap(maze, security_map, delta);
				is_moving = true;
			}
		}
	}

}
