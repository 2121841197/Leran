#pragma once

#include <Windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>

#define WALL L'¡õ'
#define BODY L'¡ñ'
#define FOOD L'¡ï'

#define POS_X 24
#define POS_Y 5

#define KEY_PRESS(VK) ((GetAsyncKeyState(VK)&0x1) ? 1 : 0)

typedef enum
{
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	RIGHT = 4
}DIRECTION;

typedef enum
{
	OK,
	KILL_BY_WALL,
	KILL_BY_SELF,
	END_NORMAL
}GAME_STATUS;

typedef struct SnakeNode
{
	int x;
	int y;

	struct SnakeNode* next;
}SnakeNode, *pSnakeNode;

typedef struct
{
	pSnakeNode _pSnake;
	pSnakeNode _pFood;
	DIRECTION _Dir;
	GAME_STATUS _Status;
	int _foodWeight;
	int _Socre;
	int _SleepTime;
}Snake, *pSnake;

void Game_Start(pSnake ps);
void WelcomeTo_Game(void);
void CreateMap(void);
void Snake_Init(pSnake ps);
void Create_Food(pSnake ps);
void Game_Run(pSnake ps);
void Game_End(pSnake ps);
void Set_Pos(short x, short y);