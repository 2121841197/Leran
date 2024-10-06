
#include "Snake.h"

void test(void)
{
	int ch = 0;

	do
	{
		Snake snake = { 0 };

		Game_Start(&snake);

		Game_Run(&snake);

		Game_End(&snake);

		Set_Pos(0, 27);
		printf("再来一局吗？(Y/N):");
		ch = getchar();
		getchar();
	} while (ch == 'Y' || ch == 'y');
}

int main(void)
{
	setlocale(LC_ALL, "");
	srand((unsigned int)time(NULL));
	test();

	return 0;
}