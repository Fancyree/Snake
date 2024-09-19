#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>		//Ϊ��ʹ��POINT
#include<graphics.h>		//EasyX����ͼ�ο�ͷ�ļ�
#include<conio.h>		//���ڼ��������������õĿ�

//�������������
#include<MMSystem.h>		//������ý���豸�ӿڵ�ͷ�ļ�
#pragma comment(lib,"winmm.lib")//���ؾ�̬��

#define SNAKE_MAX 500	//�ߵ���󳤶�
#define N 100
int num = 1;

HWND hwnd = NULL;		//������ʾ��

enum Directions			//��ö�ٱ����г��ߵ��ĸ�����
{
	Up, Down, Left, Right
};
typedef enum Directions direction;

typedef struct Users//����洢�û����ݵĽṹ������
{
	char name[10];//�û���
	int score;//��߷���
}user;
struct Snake			//�ߵĽṹ
{
	int size;		//�ߵĴ�С
	int direction;  //�ߵķ���
	int speed;		//�ߵ��ٶ�
	POINT position[SNAKE_MAX];		//�ߵ���������
}snake;
struct Food			//ʳ��Ľṹ
{
	int x,y;			//ʳ�������
	int radius;		//ʳ��Ĵ�С
	int score,sum;
	bool flag;		//���������������ж�ʳ���Ƿ񱻳Ե�
}food;

void GameSetup();	//��Ϸ���ݳ�ʼ��
void GameDraw();		//��Ϸ���ݻ���
void Music();		//��������
void SnakeMove();	//�ߵ��ƶ�
void KeyboardControl();//ͨ����������ı��ߵķ���
void Menu();			//���˵�����ģ��
int SnakeJudge();	//�ж����Ƿ�ײǽ��ײ���Լ�
void AboutFood();	//ʳ�ﱻ�Ե������ж��Լ������ĵ���
void Show();			//��Ϸ�ڵ�һЩչʾ
void GameRun();		//��Ϸ����
int Rank();			//�����ID�͵÷������ļ���
int ReadRank();		//����������а�

int main()
{
	Menu();
	GameRun();
	Rank();
	ReadRank();

	return 0;
}

void GameSetup()		//��Ϸ���ݳ�ʼ��
{	
	//���ڵĳ�ʼ��
	initgraph(640,640,SHOWCONSOLE);		//init��ʼ��graph����, SHOWCONSOLEʹ����̨�ɼ�, ������ʾ���а�
	//�ߵĳ�ʼ��
	snake.size = 3;
	snake.direction = Right;
	snake.speed = 8;
	for(int i = 0; i < snake.size; i++)
	{
		snake.position[i].x=100 - i*10;
		snake.position[i].y=100;
	}
	//ʳ��ĳ�ʼ��
	srand(time(NULL));		//ʹ��ϵͳʱ����Ϊ���������
	food.x = rand() %601+20;	//ʳ���x�����������
	food.y = rand() %441+20;	//ʳ���y�����������
	food.flag = true;
	food.radius = rand() %16+5;
	food.score = food.radius;
}
void GameDraw()		//��Ϸ���ݻ���
{
	//��Ϸ���ݻ���
	BeginBatchDraw();	//˫�����ͼ���Է����ſ���
	setbkcolor(RGB(252,157,154));	//���ñ�����ɫ
	cleardevice();		//ʹ�õ�ǰ����ɫ��ջ�ͼ�豸
	//������
	setlinecolor(BLACK);
	for(int i = 0; i < snake.size; i++)
	{
		setfillcolor(RGB(rand()%255, rand()%255, rand()%255));
		fillcircle(snake.position[i].x,snake.position[i].y, 5);	
	}
	//���������۾�
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
	//��ʳ��
	setfillcolor(RGB(64,244,205));
	if(food.flag)
		solidcircle(food.x, food.y, food.radius);
	//��ǽ
	setfillcolor(RGB(220,20,60));
	solidrectangle(0,0,640,5);
	solidrectangle(0,0,5,480);
	solidrectangle(640,480,0,475);
	solidrectangle(640,480,635,0);
	EndBatchDraw();
}
void Music()			//��������
{
	mciSendString("open ./res/SnakeBGM.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);
}
void SnakeMove()		//�ߵ��ƶ�
{
	for(int i = snake.size - 1; i > 0; i--)
		snake.position[i] = snake.position[i - 1];	//�ߵĽڽ�֮��������
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
void KeyboardControl()		//ͨ����������ı��ߵķ���
{
	if(_kbhit())				//�жϼ����Ƿ����룬�������뷵��ֵΪ�棬��֮��
	{
		switch(_getch())		//�� �� �� �Ҽ���ֵ�ֱ��� 72 80 75 77 ; �ո��ֵ��32
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
		case 32:			//�ո����ͣ��Ϸ����ͣ��ɰ����������
			system("pause");
			break;
		}
	}
	
}

void Menu()		//���˵�����ģ��
{
	initgraph(500, 500);
	setbkcolor(WHITE);
	cleardevice();
	do{
		BeginBatchDraw();
		setbkmode(TRANSPARENT);
		setfillcolor(WHITE);//���������ɫ
	    solidrectangle(0,0,500,500);
		settextcolor(BLACK);//����������ɫ
	    settextstyle(50,0,"����");//����������ʽ;
	    outtextxy(110,100,"̰������Ϸ");//�������
	    settextcolor(BLACK);
	    settextstyle(30,0,"����");
	    outtextxy(200,200,"START");
	    settextstyle(20,0,"����");
	    outtextxy(150,230,"(���ո������һ��)");
		settextcolor(LIGHTGRAY);
		outtextxy(150,250,"��������   ��������");
		outtextxy(150,270,"��������   ��������");
	    EndBatchDraw();
        getch();//ֹͣ���ȴ�����
	}while(GetKeyState(32)>=0);//�ո��ж�
}

int SnakeJudge()			//�ж����Ƿ�ײǽ��ײ���Լ�
{
	char score[N] = "";
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	if (snake.position[0].x > 640 || snake.position[0].x < 0 ||
		snake.position[0].y > 480 || snake.position[0].y < 0)
	{
		mciSendString(("close BGM"), 0, 0, 0);
		settextstyle(46, 31, ("����"));
		sprintf_s(score, ("���ķ���:%d"), food.sum);
		outtextxy(186, 246, score);
		MessageBox(hwnd, "ײǽ����", "Game Over!", MB_OK);
		return 1;
	}
	for( int i = 1; i < snake.size ; i++)
		if( (snake.position[0].x == snake.position[i].x) &&
			(snake.position[0].y == snake.position[i].y) )
		{
			mciSendString(("close BGM"), 0, 0, 0);
			settextstyle(46, 31, ("����"));
			sprintf_s(score, ("���ķ���:%d"), food.sum);
			outtextxy(186, 246, score);
			MessageBox(hwnd, "��ɰ�ˣ�", "Game Over!", MB_OK);
			return 1;
		}
	return 0;
}

void AboutFood()		//ʳ�ﱻ�Ե������ж��Լ������ĵ���
{
	if( (abs(snake.position[0].x - food.x) <= food.radius) && 
		(abs(snake.position[0].y - food.y) <= food.radius) &&
		food.flag )
	{
		food.flag = false;
		snake.size++;
		food.sum+=food.score;//��������
		printf("\a");//��ʳ�����Ч
	}
	//ʳ���ٴ�����
	if(!food.flag)
	{
		food.x = rand() %601+20;	
		food.y = rand() %441+20;	
		food.flag = true;
		food.radius = rand() %16+5;
		food.score = food.radius;
	}
}
void Show()		//��Ϸ�ڵ�һЩչʾ
{
	BeginBatchDraw();
	char score[100]="";
	char size[100]="";
	sprintf(score, "%d", food.sum);//���÷�������ָ���� score�ַ�����
	sprintf(size, "%d", snake.size);//ͬ��
	setbkmode(TRANSPARENT);		//�����������ʱ�ı���ģʽ���˴�����Ϊ͸��
	settextcolor(WHITE);		//����������ɫ
	outtextxy(40,520,"����:");
	outtextxy(40 + 50,520, score);//�÷�ʵʱ��ʾ
	outtextxy(40,560,"�ߵĽ���:");
	outtextxy(40 + 90,560, size);//�ߵĳ�����ʾ
	outtextxy(40,600,"���¿ո��������ͣ��Ϸ����������������Ϸ");
	EndBatchDraw();
}
void GameRun()	//��Ϸ����
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

int Rank()		//�����ID�͵÷������ļ���
{
	user tmp;
	FILE *fp;
	printf("�������������Ĵ���(���������ֿ�ͷ�ͽ�β):");
	scanf("%s",&tmp.name);
	tmp.score = food.sum;
	fp=fopen("̰���ߵ÷ּ�¼.txt","a");
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
	fp = fopen("̰���ߵ÷ּ�¼.txt","rb");
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
	for(i=0;i<n-1;i++)//����ð�ݷ��������� 
    {  
        k=i;  
		for(j=i+1;j<n;j++)  
	    if(tmp[j].score>tmp[k].score)//����Ƚϴ�С  
        k=j;  
		temp=tmp[k];
		tmp[k]=tmp[i];
		tmp[i]=temp;//����������ʱ�������ݽ���
	} 
	i = 0;
	printf("����\t  ���� \t   �÷�\n");
	while(i <n)
	{
		printf("%3d%10s%10d\n",i+1,tmp[i].name,tmp[i].score);
		i++;
	}
	fclose(fp);
	return 0;
}