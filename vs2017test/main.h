#pragma once
#include "glut.h"
#include <time.h>
#include <vector>
#include "Cell.h"
#include <string>
#include <iostream>
#include "Room.h"
#include "Node.h"
#include <queue>
#include "CompareNodes.h"
#include "EqualNodes.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Warrier.h"


void init();
void displayWar();
void InitMaze();
void InitRooms();
void DigTunnels();
void initWarriers();
void warriorsCalculations();
int roomAssignment(Warrier** warrier, int team, int specificRoom, int excludeRoom);
void lookForGrenadeCasualties();
void lookForBulletCasualties();
void lookForCasualties();
void warrierGotHit(int row, int col, double damage, int target);
void GenerateSecurityMap();
void fillRoomsWithObjects();
void GenerateFireworks();
void printGameOver();
void printTeamOneWon();
void printDraw();
void printTeamTwoWon();
void announceWinner();
void resetVars();
void printTeamDeathmatch();
void printGameStatus();