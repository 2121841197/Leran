#define _CRT_SECURE_NO_WARNINGS
#include "game.h"

void Board_Init(char arr[ROWS][COLS], int rows, int cols, char set)
{
	int i = 0;
	for (i = 0; i < rows; i++)
	{
		int j = 0;
		for (j = 0; j < cols; j++)
		{
			arr[i][j] = set;
		}
	}
}

void Board_Display(char arr[ROWS][COLS], int row, int col)
{
	int i = 0;
	printf("-----扫雷游戏-----\n");

	for (i = 0; i < col + 1; i++)
	{
		printf("%d ", i);
	}
	printf("\n");

	for (i = 1; i < row + 1; i++)
	{
		int j = 0;

		printf("%d ", i);

		for (j = 1; j < col + 1; j++)
		{
			printf("%c ", arr[i][j]);
		}
		printf("\n");
	}
}

void Mine_Set(char arr[ROWS][COLS], int row, int col)
{
	int count = EASY_COUNT;
	while (count)
	{
		int x = rand() % row + 1;
		int y = rand() % col + 1;
		if (arr[x][y] == '0')
		{
			arr[x][y] = '1';
			count--;
		}
	}
}

static int Get_Mine_Count(char arr[ROWS][COLS], int x, int y)
{
	return (arr[x - 1][y] + arr[x - 1][y - 1] + arr[x][y - 1] + arr[x + 1][y - 1] + arr[x + 1][y] +
		arr[x + 1][y + 1] + arr[x][y + 1] + arr[x - 1][y + 1] - 8 * '0');
}

void Mine_Find(char arr1[ROWS][COLS], char arr2[ROWS][COLS], int row, int col)
{
	int x = 0;
	int y = 0;
	int win = 0;
	while (win < row * col - EASY_COUNT)
	{
		printf("请输入要排查的坐标:");
		scanf("%d %d", &x, &y);

		if (x > 0 && x <= row && y > 0 && y <= col)
		{
			if (arr1[x][y] == '1')
			{
				printf("你输了，请重新开始游戏\n");
				Board_Display(arr1, ROW, COL);
				break;
			}
			else
			{
				int count = Get_Mine_Count(arr1, x, y);
				arr2[x][y] = count + '0';
				Board_Display(arr2, ROW, COL);
				win++;
			}
		}
		else
		{
			printf("坐标不存在，请重新输入\n");
		}
	}
	if (win < row * col - EASY_COUNT)
	{
		printf("恭喜游戏胜利！！！\n");
		Board_Display(arr1, ROW, COL);

	}
}

