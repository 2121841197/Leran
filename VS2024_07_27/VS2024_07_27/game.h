#pragma once
#ifndef _game_H
#define _game_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW 9
#define COL 9

#define ROWS ROW+2
#define COLS COL+2

#define EASY_COUNT 10

void Board_Init(char arr[ROWS][COLS], int rows, int cols, char set);
void Board_Display(char arr[ROWS][COLS], int row, int col);
void Mine_Set(char arr[ROWS][COLS], int row, int col);
void Mine_Find(char arr1[ROWS][COLS], char arr2[ROWS][COLS], int row, int col);



#endif // !_game_H
