#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>//windows编程头文件
#include <time.h>
#include <conio.h>//控制台输入输出头文件
#include<easyx.h>
#include <graphics.h>
#pragma comment(lib,"winmm.lib")

#define Width 800
#define High 480
#define SIZEMAX		100		// 定义蛇的最大长度
#define SNAKELEN	10		// 蛇的大小和蛇每次移动的距离
#define SPEED		150	// 初始速度 
int speed = SPEED;		// 方便以后在程序中改变速度
int score = 0;

void drawMap();//绘制游戏页面
void initGame();
void init();			// 初始化游戏
void setfoodcrd();		// 设置食物要出现的坐标
void showfood();		// 在相应的坐标画出食物
void snakemove();		// 移动蛇，实际就是改变蛇各节的坐标
void judgeslod();		// 判断蛇是否撞墙或者撞到自己
void judgesef();		// 判断蛇是否吃到食物了
void showsnake();		// 在屏幕上画出蛇
void changeskdir();		// 接受用户输入，改变蛇的前进方向
void gameover();		// 游戏结束 


IMAGE bk;//背景
IMAGE fm;//封面图
IMAGE bk2;
IMAGE over;


typedef enum	// 定义表示方向的枚举变量
{
	left, right, up, down
}DIR;

typedef struct	// 定义一个结构体表示坐标
{
	int x;
	int y;
}COOR;

struct FOOD		// 定义结构体，来存储食物的相关信息
{
	COOR crd;	// 食物出现的坐标
	int flag;	// 食物的状态，1表示食物还没被吃，0表示食物被吃了
}food;

struct SNAKE	// 定义结构体，来存储蛇的相关信息
{
	COOR crd[SIZEMAX];	// 保存蛇的每个结数的坐标，最多有100节
	int node;			// 保存蛇当前节数
	DIR dir;			// 当前蛇行进的方向

}snake;

void initGame()//开始界面333
{
	mciSendString(L"open 贪吃蛇背景音.mp3 alias MUS", 0, 0, 0);
	mciSendString(L"play MUS repeat", 0, 0, 0);
	initgraph(800, 480);
	loadimage(&fm, _T("fm.jpg"), Width, High);
	putimage(0, 0, &fm);//显示背景
	setcolor(RED);
	setbkmode(TRANSPARENT);//透明的背景风格
	settextstyle(50, 0, L"楷体");//设置字体，大小
	outtextxy(300, 250, L"开始游戏");
	settextstyle(25, 0, L"楷体");
	outtextxy(300, 350, L"按空格键继续");
	system("pause");
	system("cls");
}

void drawMap()
{
	loadimage(&bk2, _T("bk2.jpg"), Width, High);
	putimage(0, 0, &bk2);
	setcolor(RED);
	setbkmode(TRANSPARENT);//透明的背景风格
	settextstyle(25, 0, L"楷体");//设置字体，大小
	outtextxy(350, 50, L"[游戏说明]\n\n\n");
	outtextxy(200, 100, L"这是一个贪吃蛇，是时候展示真正的技术了！");
	outtextxy(350, 150, L"[控制说明]");
	outtextxy(200, 200, L"W,S,A,D代表上，下，左，右");
	outtextxy(200, 250, L"注意每吃完一个食物会加速！！！");
	outtextxy(200, 300, L"注意贪吃蛇留下的痕迹会干扰视觉！！！");
	outtextxy(350, 350, L"按空格键继续");
	system("pause");
	system("cls");
	loadimage(&bk, _T("bk.png"), Width, High);
	putimage(0, 0, &bk);
}

void init()
{
	srand(time(NULL));
	food.flag = 0;
	snake.crd[0].x = 0 + SNAKELEN;
	snake.crd[0].y = 0;
	snake.crd[1].x = 0;
	snake.crd[1].y = 0;
	snake.node = 2;
	snake.dir = right;
}

void setfoodcrd()
{
	food.crd.x = rand() % (600 / SNAKELEN) * SNAKELEN;
	food.crd.y = rand() % (480 / SNAKELEN) * SNAKELEN;
	food.flag = 1;
}

void showfood()
{
	rectangle(food.crd.x, food.crd.y, food.crd.x + SNAKELEN, food.crd.y + SNAKELEN);
}

void snakemove()
{
	int i; for (i = snake.node; i > 0; i--)
	{
		snake.crd[i].x = snake.crd[i - 1].x;
		snake.crd[i].y = snake.crd[i - 1].y;
	}
	switch (snake.dir)
	{
	case up:
		snake.crd[0].y -= SNAKELEN;
		break;
	case down:
		snake.crd[0].y += SNAKELEN;
		break;
	case left:
		snake.crd[0].x -= SNAKELEN;
		break;
	case right:
		snake.crd[0].x += SNAKELEN;
	} showsnake();
}

void judgeslod()
{
	int i;
	if (snake.crd[0].x < 0 || snake.crd[0].x >= Width||
		snake.crd[0].y < 0 || snake.crd[0].y >= High)
	{
		gameover();
	}
	for (i = snake.node - 1; i > 0; i--)
	{
		if (snake.crd[0].x == snake.crd[i].x && snake.crd[0].y == snake.crd[i].y)
		{
			gameover();
		}

	}
}

void judgesef()
{
	if (snake.crd[0].x == food.crd.x && snake.crd[0].y == food.crd.y)
	{
		snake.node++;//吃到食物，蛇身长度加1
		score += 10;//每个食物得10分
		speed -= 7;//随着吃的食物越来越多，速度会越来越快

		setcolor(BLACK);
		rectangle(food.crd.x, food.crd.y, food.crd.x + SNAKELEN, food.crd.y + SNAKELEN);
		setcolor(WHITE);

		food.flag = 0;
	}
}

void showsnake()
{
	int i;
	for (i = snake.node - 1; i >= 0; i--)
	{
		rectangle(snake.crd[i].x, snake.crd[i].y, snake.crd[i].x + SNAKELEN, snake.crd[i].y + SNAKELEN);
	}
	setcolor(BLACK);
	rectangle(snake.crd[snake.node].x, snake.crd[snake.node].y, snake.crd[snake.node].x + SNAKELEN, snake.crd[snake.node].y + SNAKELEN);
	setcolor(WHITE); rectangle(snake.crd[snake.node - 1].x, snake.crd[snake.node - 1].y, snake.crd[snake.node - 1].x + SNAKELEN, snake.crd[snake.node - 1].y + SNAKELEN);
}

void changeskdir()
{
	char key;
	key = _getch();
	switch (key)
	{
	case 'w':
	case 'W':
		if (snake.dir != down)
		{
			snake.dir = up;
		}
		break;
	case 's':
	case 'S':
		if (snake.dir != up)
		{
			snake.dir = down;
		}
		break;
	case 'a':
	case 'A':
		if (snake.dir != right)
		{
			snake.dir = left;
		}
		break;
	case 'd':
	case 'D':
		if (snake.dir != left)
		{
			snake.dir = right;
		}
		break;
	}
}

void gameover()
{
	system("cls");
	loadimage(&over, _T("over.jpg"), Width, High);
	putimage(0, 0, &over);
	setcolor(RED);
	setbkmode(TRANSPARENT);//透明的背景风格
	settextstyle(50, 0, L"楷体");//设置字体，大小
	outtextxy(270, 100, L"本次游戏得分：");
	TCHAR s[500];
	swprintf_s(s, _T("%d"), score);
	outtextxy(400, 150, s);
	system("pause");

}

int main()
{
	initGame();
	drawMap();
	init();
	while (1)
	{
		while (!_kbhit())
		{
			if (!food.flag)
			{
				setfoodcrd();
			}
			showfood();
			judgeslod();
			judgesef();
			snakemove();
			Sleep(speed);
		}
		changeskdir();
	}
	return 0;
}

