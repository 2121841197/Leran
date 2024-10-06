#include "Snake.h"

void Set_Pos(short x, short y)
{
	//获取标准输出的句柄(?来标识不同设备的数值)
	HANDLE hOutput = NULL;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	//设置标准输出上光标的位置为pos
	COORD pos = { x, y };
	SetConsoleCursorPosition(hOutput, pos);
}

void WelcomeTo_Game(void)
{
	Set_Pos(40, 15);
	wprintf(L"欢迎来到贪吃蛇小游戏");
	Set_Pos(42, 20);//让按任意键继续的出现的位置好看点
	system("pause");
	system("cls"); //清理屏幕
	Set_Pos(25, 12);
	wprintf(L"用 ↑ . ↓ . ← . → 分别控制蛇的移动， F3为加速，F4为减速\n");
	Set_Pos(25, 13);
	wprintf(L"加速将能得到更高的分数\n");
	Set_Pos(40, 25);//让按任意键继续的出现的位置好看点
	system("pause");
	system("cls");
}

void Create_Map(void)
{
	int i = 0;
	//上(0,0)-(56, 0)
	Set_Pos(0, 0);
	for (i = 0; i < 58; i += 2)
	{
		wprintf(L"%c", WALL);
	}
	//下(0,26)-(56, 26)
	Set_Pos(0, 26);
	for (i = 0; i < 58; i += 2)
	{
		wprintf(L"%c", WALL);
	}
	//左
	//x是0，y从1开始增⻓
	for (i = 1; i < 26; i++)
	{
		Set_Pos(0, i);
		wprintf(L"%c", WALL);
	}
	//x是56，y从1开始增⻓
	for (i = 1; i < 26; i++)
	{
		Set_Pos(56, i);
		wprintf(L"%c", WALL);
	}

}

void Snake_Init(pSnake ps)
{
	int i = 0;
	pSnakeNode cur = NULL;

	//创建蛇⾝节点，并初始化坐标
	//头插法
	for (i = 0; i < 5; i++)
	{
		//创建蛇⾝的节点
		cur = (pSnakeNode)malloc(sizeof(SnakeNode));
		if (cur == NULL)
		{
			perror("InitSnake()::malloc()");
			return;
		}
		//设置坐标
		cur->next = NULL;
		cur->x = POS_X + 2 * i;
		cur->y = POS_Y;

		//头插法
		if (ps->_pSnake == NULL)
		{
			ps->_pSnake = cur;
		}
		else
		{
			cur->next = ps->_pSnake;
			ps->_pSnake = cur;
		}
	}

	//打印蛇的⾝体
	cur = ps->_pSnake;
	while (cur)
	{
		Set_Pos(cur->x, cur->y);
		wprintf(L"%lc", BODY);
		cur = cur->next;
	}

	//初始化贪吃蛇数据
	ps->_SleepTime = 200; //毫秒
	ps->_Socre = 0;
	ps->_Status = OK;
	ps->_Dir = RIGHT;
	ps->_foodWeight = 10;

}

void Create_Food(pSnake ps)
{
	int x = 0;
	int y = 0;

again:
	//产⽣的x坐标应该是2的倍数，这样才可能和蛇头坐标对⻬。
	do
	{
		x = rand() % 53 + 2;
		y = rand() % 25 + 1;
	} while (x % 2 != 0);

	pSnakeNode cur = ps->_pSnake;//获取指向蛇头的指针
	//⻝物不能和蛇⾝冲突
	while (cur)
	{
		if (cur->x == x && cur->y == y)
		{
			goto again;
		}
		cur = cur->next;
	}

	pSnakeNode pFood = (pSnakeNode)malloc(sizeof(SnakeNode)); //创建⻝物
	if (pFood == NULL)
	{
		perror("CreateFood::malloc()");
		return;
	}
	else
	{
		pFood->x = x;
		pFood->y = y;
		Set_Pos(pFood->x, pFood->y);
		wprintf(L"%c", FOOD);
		ps->_pFood = pFood;
	}
}

void Game_Start(pSnake ps)
{
	system("mode con cols=100 lines=30");

	system("title 贪吃蛇");

	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	//影藏光标操作
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOutput, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = false; //隐藏控制台光标
	SetConsoleCursorInfo(hOutput, &CursorInfo);//设置控制台光标状态

	WelcomeTo_Game();

	Create_Map();

	Snake_Init(ps);

	Create_Food(ps);
}

void Print_HelpInfo(void)
{
	Set_Pos(64, 10);
	wprintf(L"%ls", L"不能穿墙，不能咬到自己\n");
	Set_Pos(64, 12);
	wprintf(L"%ls", L"用↑.↓.←.→分别控制蛇的移动.");
	Set_Pos(64, 13);
	wprintf(L"%ls", L"F3 为加速，F4 为减速\n");
	Set_Pos(64, 14);
	wprintf(L"%ls", L"ESC ：退出游戏.space：暂停游戏.");
	Set_Pos(64, 15);
	wprintf(L"%ls", L"诸葛空白巨献.");
}

void Pause(void)
{
	while (1)
	{
		Sleep(200);
		if (KEY_PRESS(VK_SPACE))
		{
			break;
		}
	}
}

//pSnakeNode psn 是下⼀个节点的地址
//pSnake ps 维护蛇的指针
int NextIs_Food(pSnakeNode pn, pSnake ps)
{
	return (ps->_pFood->x == pn->x) && (ps->_pFood->y == pn->y);
}

//pSnakeNode psn 是下⼀个节点的地址
//pSnake ps 维护蛇的指针
void Eat_Food(pSnakeNode pn, pSnake ps)
{
	//头插法
	ps->_pFood->next = ps->_pSnake;
	ps->_pSnake = ps->_pFood;

	//释放⻝物节点
	free(pn);
	pn = NULL;

	//打印蛇
	pSnakeNode cur = ps->_pSnake;
	while (cur)
	{
		Set_Pos(cur->x, cur->y);
		wprintf(L"%c", BODY);
		cur = cur->next;
	}
	ps->_Socre += ps->_foodWeight;

	//创建新的⻝物
	Create_Food(ps);
}

//pSnakeNode psn 是下⼀个节点的地址
//pSnake ps 维护蛇的指针
void No_Food(pSnakeNode pn, pSnake ps)
{
	//头插法
	pn->next = ps->_pSnake;
	ps->_pSnake = pn;
	
	//打印蛇
	pSnakeNode cur = ps->_pSnake;
	while (cur->next->next != NULL)
	{
		Set_Pos(cur->x, cur->y);
		wprintf(L"%c", BODY);
		cur = cur->next;
	}
		
	//最后⼀个位置打印空格，然后释放节点
	Set_Pos(cur->next->x, cur->next->y);
	printf("  ");
	free(cur->next);
	cur->next = NULL;
	
}

//pSnake ps 维护蛇的指针
int KillBy_Wall(pSnake ps)
{
	if ((ps->_pSnake->x == 0)
		|| (ps->_pSnake->x == 56)
		|| (ps->_pSnake->y == 0)
		|| (ps->_pSnake->y == 26))
	{
		ps->_Status = KILL_BY_WALL;
		return 1;
	}
	return 0;
}

//pSnake ps 维护蛇的指针
int KillBy_Self(pSnake ps)
{
	pSnakeNode cur = ps->_pSnake->next;
	while (cur)
	{
		if ((ps->_pSnake->x == cur->x)
			&& (ps->_pSnake->y == cur->y))
		{
			ps->_Status = KILL_BY_SELF;
			return 1;
		}
		cur = cur->next;
		
	}
	return 0;
	
}

void Snake_Move(pSnake ps)
{
	//创建下⼀个节点
	pSnakeNode pNextNode = (pSnakeNode)malloc(sizeof(SnakeNode));
	if (pNextNode == NULL)
	{
		perror("SnakeMove()::malloc()");
		return;
	}

	//确定下⼀个节点的坐标，下⼀个节点的坐标根据，蛇头的坐标和⽅向确定
	switch (ps->_Dir)
	{
		case UP:
		{
			pNextNode->x = ps->_pSnake->x;
			pNextNode->y = ps->_pSnake->y - 1;
		}
		break;
		case DOWN:
		{
			pNextNode->x = ps->_pSnake->x;
			pNextNode->y = ps->_pSnake->y + 1;
		}
		break;
		case LEFT:
		{
			pNextNode->x = ps->_pSnake->x - 2;
			pNextNode->y = ps->_pSnake->y;
		}
		break;
		case RIGHT:
		{
			pNextNode->x = ps->_pSnake->x + 2;
			pNextNode->y = ps->_pSnake->y;
		}
		break;
	}

	//如果下⼀个位置就是⻝物
	if (NextIs_Food(pNextNode, ps))
	{
			Eat_Food(pNextNode, ps);
	}
	else//如果没有⻝物
	{
			No_Food(pNextNode, ps);
	}
	KillBy_Wall(ps);
	KillBy_Self(ps);
}

void Game_Run(pSnake ps)
{

	//打印右侧帮助信息
	Print_HelpInfo();
	do
	{
		Set_Pos(64, 10);
		printf("得分：%d ", ps->_Socre);
		Set_Pos(64, 11);
		printf("每个食物得分：%d分", ps->_foodWeight);

		if (KEY_PRESS(VK_UP) && ps->_Dir != DOWN)
		{
			ps->_Dir = UP;
		}
		else if (KEY_PRESS(VK_DOWN) && ps->_Dir != UP)
		{
			ps->_Dir = DOWN;
		}
		else if (KEY_PRESS(VK_LEFT) && ps->_Dir != RIGHT)
		{
			ps->_Dir = LEFT;
		}
		else if (KEY_PRESS(VK_RIGHT) && ps->_Dir != LEFT)
		{
			ps->_Dir = RIGHT;
		}
		else if (KEY_PRESS(VK_SPACE))
		{
			Pause();
		}
		else if (KEY_PRESS(VK_ESCAPE))
		{
			ps->_Status = END_NORMAL;
			break;
		}
		else if (KEY_PRESS(VK_F3))
		{
			if (ps->_SleepTime >= 80)
			{
				ps->_SleepTime -= 30;
				ps->_foodWeight += 2;//⼀个⻝物分数最⾼是20分
			}
		}
		else if (KEY_PRESS(VK_F4))
		{
			if (ps->_SleepTime < 320)
			{
				ps->_SleepTime += 30;
				ps->_foodWeight -= 2;//⼀个⻝物分数最低是2分
			}
		}

		//蛇每次⼀定之间要休眠的时间，时间短，蛇移动速度就快
		Snake_Move(ps);
		Sleep(ps->_SleepTime);

	} while (ps->_Status == OK);
}

void Game_End(pSnake ps)
{
	pSnakeNode cur = ps->_pSnake;
	Set_Pos(21, 14);
	switch (ps->_Status)
	{
		case END_NORMAL:
			printf("您主动退出游戏\n");
			break;
		case KILL_BY_SELF:
			printf("您撞上⾃⼰了 ,游戏结束!\n");
			break;
		case KILL_BY_WALL:
			printf("您撞墙了,游戏结束!\n");
			break;
		
	}
	
	//释放蛇⾝的节点
	while (cur)
	{
		pSnakeNode del = cur;
		cur = cur->next;
		free(del);
	}
	
}

