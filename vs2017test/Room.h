#pragma once

const int MSZ = 120;
const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int BLACK = 4;
const int GRAY = 5;
const int PATH = 6;
const int AMMO = 7;
const int MEDICINES = 8;
const int BARRIER = 9;
const int TEAM_1 = 10;
const int TEAM_2 = 11;
const int TEAM_SIZE = 2;
const double HIT_WEIGHT_SECURITY_MAP = 0.004;
const double SAFE_PLACE_THRESHOLD = 0.25;

const int MAX_HP = 50;


class Room
{

private:
	int width, height, centerX, centerY;

public:
	Room();
	void DrawMe();

	void BuildFirstAidStation(int maze[MSZ][MSZ]);
	void BuildAmmunitionBunker(int maze[MSZ][MSZ]);
	void BuildBarrier(int maze[MSZ][MSZ]);
	void FillMaze(int maze[MSZ][MSZ]);
	bool IsOverlap(int w, int h, int row, int col);

	void SetWidth(int w)
	{
		width = w;
	};
	void SetHeigth(int h)
	{
		height = h;
	};
	void SetCenterX(int x)
	{
		centerX = x;
	};
	void SetCenterY(int y)
	{
		centerY = y;
	};

	int GetCenterRow()
	{
		return centerY;
	};
	int GetCenterCol()
	{
		return centerX;
	};
	int GetWidth()
	{
		return width;
	};
	int GetHeight()
	{
		return height;
	};
};

