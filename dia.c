#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

// ! 定义区域

// 获取坐标
#define Get(C) for (C, i = n[T[0]]; j = x + i % 4, k = y + i / 4 % 4, i; i >>= 4)
#define Next(C) for (C, i = n[T[1]]; nj = i % 4, nk = i / 4 % 4, i; i >>= 4)

// 每种方块的类型
// 使用一个数字储存
// 转化为二进制使用
int n[] = {
	51264,
	12816,
	21520,
	21520,
	25872,
	34113,
	21537,
	38208,
	25921,
	38481,
	38484,
	38209,
	25922,
	43345,
	34388,
	38160,
	25920,
	38177,
	42580,
	38993};
// 变量
int i, j, k, d, my = 0;
int nj, nk = 0;
int c = -1;
// 地图
int *map = NULL;
// 地图大小
int height = 25;
int wide = 15;
int size = 0;
// 坐标
int x = 0;
int y = 0;
// 类型
int T[2] = {0};
// 积分
int marks = 0;
// 困难程度
int flag = 0;
int num = 0;
// 提示区域
int *imply = NULL;
// 下一个元素
int *next = NULL;

// ! 函数区域

void welcome()
{
	system("cls");
	printf("*****************************************************************************\n");
	printf("*                          Welcome to play Tetris!                          *\n");
	printf("*                             operating method                              *\n");
	printf("*                    W : turn  A : left  S : speeder  D : right             *\n");
	printf("*                                                                           *\n");
	printf("*                    1.PLAY                             0.exit              *\n");
	printf("*                                                                           *\n");
	printf("*                             Have a good game.                             *\n");
	printf("*                                                                           *\n");
	printf("*                                                                           *\n");
	printf("*****************************************************************************\n");
}

void move(int *v, int l)
{
	// 用Get取出四个位置坐标，并判断合法性
	// 分别判断左越界，右越界，下越界，有无方块
	// 如果出现冲突，发出固定信号c = 0
	Get(*v += l)
	{
		if (j < 0 || j >= wide || k >= height || map[k * wide + j] != 0)
		{
			// c非法
			// 将c 变为0 ，发出回滚信号
			c = 0;
		}
	}
	if (c)
	{
		c = 1;
	}
	// 如果c等于0 ，坐标违法，数据回滚
	if (!c)
	{
		*v -= l;
		// 如果是下降造成违法
		// 说明降落到某个实体上边
		// 发出固定信号
		// 准备生成下一个方块
		if (v == &y)
		{
			c = -1;
		}
	}
}

void check(int *map, int *c, int *x, int *y, int T[2], int *j, int *k)
{
	int count = 0;
	if (*c == -1)
	{
		// 如果没有失败，生成新的
		if (*y)
		{
			Next(1)
			{
				next[nk * 4 + nj] = 0;
			}
			T[0] = T[1];
			T[1] = rand() % 20;
			num++;
			*x = 0;
			*y = 0;
			i = size - 1;
			while (i)
			{
				for (*j = wide; i;)
				{
					*j -= !!map[i];
					if ((i-- % wide) == 0)
					{
						break;
					}
				}
				if (!*j)
				{
					i += wide;
					int p = i;
					// 消除整列
					while (p > wide)
					{
						map[p] = map[p - wide];
						p--;
					}
					marks += (wide * ++count);
				}
			}
			*k = 0;
			*j = 0;
		}
		// 如果y=0代表游戏失败
		else if (!*y)
		{
			*c = 27;
			return;
		}
	}
}

void game()
{
	c = 1;
	// 初始化
	// 设置随机数起点
	srand((unsigned int)time(NULL));
	// 开辟空间
	map = (int *)calloc(size = wide * height, sizeof(int));
	next = (int *)calloc(16, sizeof(int));
	// 清屏
	// 退出
	// 刷新率
	for (system("cls"); c - 27; Sleep(30))
	{
		CONSOLE_CURSOR_INFO a = {1, 0};
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &a);
		// 获取是否有操作
		// 转换大小写
		Get(c = _kbhit() ? _getch() & 95 : 1)
		{
			// 将对应的区域设置为0
			map[k * wide + j] = 0;
		}
		// 判断c是否为对应操作
		// 如果是对应操作，异或后就是0
		// 是0的话就要继续操作逻辑或
		// 如果不是0就会跳出
		// 左右和加速操作
		//  c ^ 65 || move(&x, -1), c ^ 68 || move(&x, 1), c ^ 83 || move(&y, 1);
		// 换一种写法
		if (!(c ^ 65))
		{
			move(&x, -1);
		}
		if (!(c ^ 68))
		{
			move(&x, 1);
		}
		if (!(c ^ 83))
		{
			move(&y, 1);
		}
		// 旋转
		if (!(c ^ 87))
		{
			if (T[0] > 8)
			{
				i = 3;
			}
			if (T[0] <= 8)
			{
				i = 1;
			}
			if ((T[0] & i ^ i) == 0)
			{
				my = -i;
			}
			if (T[0] & i ^ i)
			{
				my = 1;
			}
			move(&T[0], my);
		}
		// 自动下落
		// 循环十次下落一次
		if (d != 0)
		{
			d--;
			Get(1)
			{
				map[k * wide + j] = T[0] / 4 * 16 + 154;
			}
		}
		else if (d == 0)
		{
			c = 1;
			move(&y, 1);
			Get(d = 15 - flag)
			{
				map[k * wide + j] = T[0] / 4 * 16 + 154;
			}
		}
		// 提高难度
		if (num == 15)
		{
			if (flag != 12)
			{
				flag++;
				num = 0;
			}
		}
		// 检查是否可以消除
		// 并且当上一个块落下去后才可以进行检查
		check(map, &c, &x, &y, T, &j, &k);
		// 注意这里是两个空格,不然比例不对
		// 设置光标位置准备打印
		// 同时隐藏光标
		COORD b = {0, 0};
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), b);
		// 打印一次
		i = 0;
		while (i < size)
		{
			// 设置颜色，如果是实体就会打印出有颜色的空格
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), map[i]);
			// 无阻塞输出
			_cputs("  ");
			i++;
			if (i % wide == 0)
			{
				_cputs("\n");
			}
		}
		COORD c = {30, 12};
		// 打印分数
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 156);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		printf("marks: %d", marks);
		// 打印当前难度
		COORD d = {30, 14};
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 156);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), d);
		printf("rank: %d", flag);
		// 打印下一个方块
		COORD e = {32, 2};
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), e);
		Next(1)
		{
			next[nk * 4 + nj] = T[1] / 4 * 16 + 154;
		}
		int r = 0;
		while (r < 16)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), next[r]);
			_cputs("  ");
			r++;
			if (r % 4 == 0)
			{
				_cputs("\n");
				COORD e = {32, 2 + r / 4};
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), e);
			}
		}
	}
}

// ! 主函数区域

int main()
{
	// 设置背景颜色
	while (1)
	{
		system("color 84");
		welcome();
		int choice = 0;
		scanf_s("%d", &choice);
		switch (choice)
		{
		case 1:
			system("cls");
			game();
			break;
		case 0:
			system("cls");
			printf("Bye~\n");
			exit(0);
		default:
			break;
		}
	}
	return 0;
}
