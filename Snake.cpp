#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>		//为了使用POINT
#include<graphics.h>		//EasyX引用图形库头文件
#include<conio.h>		//关于键盘输入所需引用的库

//播放音乐所需库
#include<MMSystem.h>		//包含多媒体设备接口的头文件
#pragma comment(lib,"winmm.lib")//加载静态库

#define SNAKE_MAX 500	//蛇的最大长度
#define N 100
int num = 1;

HWND hwnd = NULL;		//死亡提示框

enum Directions			//用枚举变量列出蛇的四个方向
{
	Up, Down, Left, Right
};
typedef enum Directions direction;

typedef struct Users//定义存储用户数据的结构体数组
{
	char name[10];//用户名
	int score;//最高分数
}user;
struct Snake			//蛇的结构
{
	int size;		//蛇的大小
	int direction;  //蛇的方向
	int speed;		//蛇的速度
	POINT position[SNAKE_MAX];		//蛇的坐标数组
}snake;
struct Food			//食物的结构
{
	int x,y;			//食物的坐标
	int radius;		//食物的大小
	int score,sum;
	bool flag;		//布尔变量，用来判断食物是否被吃掉
}food;

void GameSetup();	//游戏数据初始化
void GameDraw();		//游戏内容绘制
void Music();		//播放音乐
void SnakeMove();	//蛇的移动
void KeyboardControl();//通过键盘输入改变蛇的方向
void Menu();			//主菜单界面模块
int SnakeJudge();	//判断蛇是否撞墙或撞到自己
void AboutFood();	//食物被吃掉与否的判断以及分数的叠加
void Show();			//游戏内的一些展示
void GameRun();		//游戏运行
int Rank();			//将玩家ID和得分输入文件中
int ReadRank();		//排序并输出排行榜

int main()
{
	Menu();
	GameRun();
	Rank();
	ReadRank();

	return 0;
}

void GameSetup()		//游戏数据初始化
{	
	//窗口的初始化
	initgraph(640,640,SHOWCONSOLE);		//init初始化graph窗口, SHOWCONSOLE使控制台可见, 用来显示排行榜
	//蛇的初始化
	snake.size = 3;
	snake.direction = Right;
	snake.speed = 8;
	for(int i = 0; i < snake.size; i++)
	{
		snake.position[i].x=100 - i*10;
		snake.position[i].y=100;
	}
	//食物的初始化
	srand(time(NULL));		//使用系统时钟作为随机数种子
	food.x = rand() %601+20;	//食物的x坐标随机生成
	food.y = rand() %441+20;	//食物的y坐标随机生成
	food.flag = true;
	food.radius = rand() %16+5;
	food.score = food.radius;
}
void GameDraw()		//游戏内容绘制
{
	//游戏内容绘制
	BeginBatchDraw();	//双缓冲绘图，以防播放卡顿
	setbkcolor(RGB(252,157,154));	//设置背景颜色
	cleardevice();		//使用当前背景色清空绘图设备
	//绘制蛇
	setlinecolor(BLACK);
	for(int i = 0; i < snake.size; i++)
	{
		setfillcolor(RGB(rand()%255, rand()%255, rand()%255));
		fillcircle(snake.position[i].x,snake.position[i].y, 5);	
	}
	//给蛇添上眼睛
	setfillcolor(BLACK);
	switch(snake.direction)
	{
	case Up:
		solidcircle(snake.position[0].x+3, snake.position[0].y-3, 2);
		solidcircle(snake.position[0].x-3, snake.position[0].y-3, 2);
		break;
	case Down:
		solidcircle(snake.position[0].x+3, snake.position[0].y+3, 2);
		solidcircle(snake.position[0].x-3, snake.position[0].y+3, 2);
		break;
	case Left:
		solidcircle(snake.position[0].x-3, snake.position[0].y+3, 2);
		solidcircle(snake.position[0].x-3, snake.position[0].y-3, 2);
		break;
	case Right:
		solidcircle(snake.position[0].x+3, snake.position[0].y+3, 2);
		solidcircle(snake.position[0].x+3, snake.position[0].y-3, 2);
		break;
	}
	//画食物
	setfillcolor(RGB(64,244,205));
	if(food.flag)
		solidcircle(food.x, food.y, food.radius);
	//画墙
	setfillcolor(RGB(220,20,60));
	solidrectangle(0,0,640,5);
	solidrectangle(0,0,5,480);
	solidrectangle(640,480,0,475);
	solidrectangle(640,480,635,0);
	EndBatchDraw();
}
void Music()			//播放音乐
{
	mciSendString("open ./res/SnakeBGM.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);
}
void SnakeMove()		//蛇的移动
{
	for(int i = snake.size - 1; i > 0; i--)
		snake.position[i] = snake.position[i - 1];	//蛇的节节之间连着走
		switch(snake.direction)
		{
		case	 Up:
			snake.position[0].y-= snake.speed;
			break;
		case Down:
			snake.position[0].y+= snake.speed;
			break;
		case Left:
			snake.position[0].x-= snake.speed;
			break;
		case Right:
			snake.position[0].x+= snake.speed;
			break;
		}
}
void KeyboardControl()		//通过键盘输入改变蛇的方向
{
	if(_kbhit())				//判断键盘是否输入，若有输入返回值为真，反之则反
	{
		switch(_getch())		//上 下 左 右键的值分别是 72 80 75 77 ; 空格的值是32
		{
		case 72:
			if(snake.direction != Down)
				snake.direction = Up;
			break;
		case 80:
			if(snake.direction != Up)
				snake.direction = Down;
			break;
		case 75:
			if(snake.direction != Right)
				snake.direction = Left;
			break;
		case 77:
			if(snake.direction != Left)
				snake.direction = Right;
			break;
		case 32:			//空格键暂停游戏，暂停后可按任意键继续
			system("pause");
			break;
		}
	}
	
}

void Menu()		//主菜单界面模块
{
	initgraph(500, 500);
	setbkcolor(WHITE);
	cleardevice();
	do{
		BeginBatchDraw();
		setbkmode(TRANSPARENT);
		setfillcolor(WHITE);//设置填充颜色
	    solidrectangle(0,0,500,500);
		settextcolor(BLACK);//设置字体颜色
	    settextstyle(50,0,"楷体");//设置字体样式;
	    outtextxy(110,100,"贪吃蛇游戏");//输出字体
	    settextcolor(BLACK);
	    settextstyle(30,0,"楷体");
	    outtextxy(200,200,"START");
	    settextstyle(20,0,"楷体");
	    outtextxy(150,230,"(按空格进入下一步)");
		settextcolor(LIGHTGRAY);
		outtextxy(150,250,"←代表左   →代表右");
		outtextxy(150,270,"↑代表上   ↓代表下");
	    EndBatchDraw();
        getch();//停止，等待操作
	}while(GetKeyState(32)>=0);//空格判断
}

int SnakeJudge()			//判断蛇是否撞墙或撞到自己
{
	char score[N] = "";
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	if (snake.position[0].x > 640 || snake.position[0].x < 0 ||
		snake.position[0].y > 480 || snake.position[0].y < 0)
	{
		mciSendString(("close BGM"), 0, 0, 0);
		settextstyle(46, 31, ("黑体"));
		sprintf_s(score, ("您的分数:%d"), food.sum);
		outtextxy(186, 246, score);
		MessageBox(hwnd, "撞墙啦！", "Game Over!", MB_OK);
		return 1;
	}
	for( int i = 1; i < snake.size ; i++)
		if( (snake.position[0].x == snake.position[i].x) &&
			(snake.position[0].y == snake.position[i].y) )
		{
			mciSendString(("close BGM"), 0, 0, 0);
			settextstyle(46, 31, ("黑体"));
			sprintf_s(score, ("您的分数:%d"), food.sum);
			outtextxy(186, 246, score);
			MessageBox(hwnd, "紫砂了！", "Game Over!", MB_OK);
			return 1;
		}
	return 0;
}

void AboutFood()		//食物被吃掉与否的判断以及分数的叠加
{
	if( (abs(snake.position[0].x - food.x) <= food.radius) && 
		(abs(snake.position[0].y - food.y) <= food.radius) &&
		food.flag )
	{
		food.flag = false;
		snake.size++;
		food.sum+=food.score;//分数计算
		printf("\a");//吃食物的音效
	}
	//食物再次生成
	if(!food.flag)
	{
		food.x = rand() %601+20;	
		food.y = rand() %441+20;	
		food.flag = true;
		food.radius = rand() %16+5;
		food.score = food.radius;
	}
}
void Show()		//游戏内的一些展示
{
	BeginBatchDraw();
	char score[100]="";
	char size[100]="";
	sprintf(score, "%d", food.sum);//将得分输出结果指定到 score字符串中
	sprintf(size, "%d", snake.size);//同上
	setbkmode(TRANSPARENT);		//设置文字输出时的背景模式，此处设置为透明
	settextcolor(WHITE);		//设置字体颜色
	outtextxy(40,520,"分数:");
	outtextxy(40 + 50,520, score);//得分实时显示
	outtextxy(40,560,"蛇的节数:");
	outtextxy(40 + 90,560, size);//蛇的长度显示
	outtextxy(40,600,"按下空格键即可暂停游戏，随后按任意键继续游戏");
	EndBatchDraw();
}
void GameRun()	//游戏运行
{
	GameSetup();
	Music();
	while(1)
	{
		GameDraw();
		SnakeMove();
		KeyboardControl();
		AboutFood();
		Show();
		if(SnakeJudge())
		{
			closegraph();
			break;
		}
		Sleep(30);
	}
}

int Rank()		//将玩家ID和得分输入文件中
{
	user tmp;
	FILE *fp;
	printf("大侠请输入您的大名(请勿以数字开头和结尾):");
	scanf("%s",&tmp.name);
	tmp.score = food.sum;
	fp=fopen("贪吃蛇得分记录.txt","a");
	if(fp==NULL)
	{  
		printf("cannot open the files\n");
		system("pause");
        exit(1);
     }
	fprintf(fp,"%s %d ",tmp.name,tmp.score);
	fclose(fp);
	return 0;
}
int ReadRank()
{
    user tmp[10];
	user temp;
	int n = 1,j = 0,i = 0,k = 0;
	FILE *fp;
	fp = fopen("贪吃蛇得分记录.txt","rb");
	if(fp==NULL)              
	{  
        printf("cannot open the files\n");  
        system("pause");  
        exit(1);  
    }
	fscanf(fp,"%s%d",tmp[n-1].name,&tmp[n-1].score);
	while(!feof(fp))
	{
	n++;
	fscanf(fp,"%s%d",tmp[n-1].name,&tmp[n-1].score);
	}
	for(i=0;i<n-1;i++)//采用冒泡法进行排序 
    {  
        k=i;  
		for(j=i+1;j<n;j++)  
	    if(tmp[j].score>tmp[k].score)//排序比较大小  
        k=j;  
		temp=tmp[k];
		tmp[k]=tmp[i];
		tmp[i]=temp;//当符合条件时进行数据交换
	} 
	i = 0;
	printf("排名\t  名称 \t   得分\n");
	while(i <n)
	{
		printf("%3d%10s%10d\n",i+1,tmp[i].name,tmp[i].score);
		i++;
	}
	fclose(fp);
	return 0;
}