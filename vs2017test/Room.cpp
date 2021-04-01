#include "Room.h"
#include "glut.h"
#include <time.h>

Room::Room()
{
	width = 0;
	height = 0;
	centerX = 0;
	centerY = 0;
}

void Room::DrawMe()
{
	glColor3d(1, 1, 1); // white
	glBegin(GL_POLYGON);
	glVertex2d(0.5, 0.2);
	glVertex2d(0.8, 0.2);
	glVertex2d(0.8, -0.1);
	glVertex2d(0.5, -0.1);
	glEnd();
}

void Room::FillMaze(int maze[MSZ][MSZ])
{
	int i, j;

	for (i = centerY - height / 2; i < centerY + height / 2; i++)
		for (j = centerX - width / 2; j < centerX + width / 2; j++)
			maze[i][j] = SPACE;
}

bool Room::IsOverlap(int w, int h, int row, int col)
{
	int hdist, vdist; // horizontal and vertical distances between rooms centers

	hdist = abs(col - centerX);
	vdist = abs(row - centerY);
	return hdist < w / 2 + width / 2 + 2 && vdist < h / 2 + height / 2 + 2;
}

void Room::BuildFirstAidStation(int maze[MSZ][MSZ])
{
	srand((unsigned int)time(NULL));

	int i, j;
	int y, x;

	bool isInValidSpace = false;
	do
	{
		isInValidSpace = false;
		y = rand() % (height - 2) + 2 + centerY - height / 2;
		x = rand() % (width - 2) + 2 + centerX - width / 2;
		for (i = y - 2; i <= y + 2 && !isInValidSpace; i++)
		{
			for (j = x - 2; j <= x + 2 && !isInValidSpace; j++)
			{
				if (maze[i][j] != SPACE)
				{
					isInValidSpace = true;
				}
			}
		}
	} while (isInValidSpace);

	/*

	medicine station (red):
						 *
						***
						 *
	*/

	maze[y][x] = MEDICINES;
	maze[y - 1][x] = MEDICINES;
	maze[y + 1][x] = MEDICINES;
	maze[y][x - 1] = MEDICINES;
	maze[y][x + 1] = MEDICINES;
}

void Room::BuildAmmunitionBunker(int maze[MSZ][MSZ])
{
	srand((unsigned int)time(NULL));

	int i, j;
	int y, x;

	bool isInValidSpace;
	do
	{
		isInValidSpace = false;
		y = rand() % (height - 2) + 2 + centerY - height / 2;
		x = rand() % (width - 2) + 2 + centerX - width / 2;
		for (i = y - 2; i <= y + 2 && !isInValidSpace; i++)
		{
			for (j = x - 2; j <= x + 2 && !isInValidSpace; j++)
			{
				if (maze[i][j] != SPACE)
				{
					isInValidSpace = true;
				}
			}
		}
	} while (isInValidSpace);

	/*

	ammo bunker (green):
					***
					* *
					***
	*/

	maze[y + 1][x - 1] = AMMO;
	maze[y + 1][x] = AMMO;
	maze[y + 1][x + 1] = AMMO;

	maze[y][x - 1] = AMMO;
	maze[y][x + 1] = AMMO;

	maze[y - 1][x - 1] = AMMO;
	maze[y - 1][x] = AMMO;
	maze[y - 1][x + 1] = AMMO;
}

void Room::BuildBarrier(int maze[MSZ][MSZ])
{
	srand((unsigned int)time(NULL));

	int i, j;
	int y, x;

	bool isInValidSpace = false;
	do
	{
		isInValidSpace = false;
		y = (rand() % (height - 4)) + 4 + centerY - height / 2;
		x = (rand() % (width - 4)) + 4 + centerX - width / 2;
		for (i = y - 4; i <= y + 4 && !isInValidSpace; i++)
		{
			for (j = x - 4; j <= x + 4 && !isInValidSpace; j++)
			{
				if (maze[i][j] != SPACE)
				{
					isInValidSpace = true;
				}
			}
		}
	} while (isInValidSpace);

	/*

	Barrier (blue):
						 * *
						 *
						 *
						 *
						 *
						 * *
	*/


	if (rand() % 10 > 4)
	{
		maze[y - 1][x] = BARRIER;
		maze[y + 1][x] = BARRIER;
		maze[y - 2][x] = BARRIER;
		maze[y + 2][x] = BARRIER;
		maze[y - 3][x + 1] = BARRIER;
		maze[y + 3][x + 1] = BARRIER;
		maze[y - 3][x] = BARRIER;
		maze[y + 3][x] = BARRIER;
	}
	else
	{
		maze[y][x - 1] = BARRIER;
		maze[y][x + 1] = BARRIER;
		maze[y][x - 2] = BARRIER;
		maze[y][x + 2] = BARRIER;
		maze[y + 1][x - 3] = BARRIER;
		maze[y + 1][x + 3] = BARRIER;
		maze[y][x - 3] = BARRIER;
		maze[y][x + 3] = BARRIER;
	}
	maze[y][x] = BARRIER;
}