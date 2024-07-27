/*******************************
* ѧϰstatic��extern
* ʵ��ɨ����Ϸ
* 
* 2024��7��27��
* 
* ���հ�
*******************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "game.h"

void menu(void)
{
	printf("************************\n");
	printf("******  1. play  *******\n");
	printf("******  0. exit  *******\n");
	printf("************************\n");

}

void game(void)
{
	char mine[ROWS][COLS] = { 0 };
	char show[ROWS][COLS] = { 0 };
	Board_Init(mine, ROWS, COLS, '0');
	Board_Init(show, ROWS, COLS, '*');

	Mine_Set(mine, ROW, COL);

	//Board_Display(mine, ROW, COL);
	Board_Display(show, ROW, COL);

	Mine_Find(mine, show, ROW, COL);

}

void test(void)
{
	int input = 0;
	srand((unsigned int)time(NULL));

	do
	{
		menu();
		printf("��ѡ��:>");
		scanf("%d", &input);

		switch (input)
		{
		case 0:
			printf("��Ϸ�������˳���Ϸ\n");
			break;
		case 1:
			game();
			break;
		default:
			printf("ѡ�����������ѡ��\n");
			break;
		}
	} while (input);
}

int main(void)
{
	test();
	return 0;
}