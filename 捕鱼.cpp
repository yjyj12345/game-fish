#define _CRT_SECURE_NO_WARNINGS 
#include<stdio.h>
#include<graphics.h>
#include<time.h>
#include<stdlib.h>
#include<conio.h>
#include<easyx.h>
#include<Windows.h>
#include <mmsystem.h>  //多媒体设备接口头文件
#pragma comment(lib,"winmm.lib")  // 加载多媒体设备接口库文件
#include <string.h>

#define width 1000    //一些特定数值
#define height 700    //窗口大小
#define fishmaxnums 11    //鱼的大小 数目
#define timermax 10  //定时器数目
#define board 100    //边界
#define aspect_left 0
#define aspect_right 2  //朝向
#define role 0   //角色
#define number 11

int score1 = 0;   int score2 = 0;//两关计分

struct Fish//第一关鱼结构体
{    int x; 
     int y;  //鱼位置坐标
     int dir;    //鱼的方向
	 int type;    //鱼的种类
	 int w; 
	 int h;   //鱼的高度宽度
};
struct Fish fish[fishmaxnums]; //等效于stryct Fish fish[]
IMAGE img_background;            //背景图
IMAGE fishimg[11][4]; //下标一表示鱼的种类 下标二01表示左边的   23表示右边的

struct biqibao//第二关鱼结构体
{   int x;
	int y;  //位置坐标
	int dir;    //方向
	int type;    //种类
	int w;
	int h;   //高度宽度
};
struct biqibao a[number];
IMAGE img_bk;            //比奇堡背景图
IMAGE bqbimg[11][4];

typedef struct userdata {//用户名密码结构体
	char user_username[20];
	char user_password[8];
}user;

struct button {//按钮结构体
	int x;
	int y;    //按钮坐标
	int w;
	int h;    //按钮宽高
	COLORREF color;  // 颜色
	char* ptext;      //文字
};//函数声明
void Get_NP(user* u);
void UserdataFile_write(user* u);
int UserdataFile_read(user* u);
void Login(user* u);
void Fillout(user* u);
void menu1();
void menu2();
void loadresources();
void initfish(int type);
void drawfish();
void resetfish();
void control();
int eatfish(int type);
int ontimer(int duration, int id);
void movefish();
int gameover();
void loadresourcesbqb();
void initfishbqb(int type);
void drawfishbqb();
void resetfishbqb();
void controlbqb();
int eatfishbqb(int type);
void movefishbqb();
int gameoverbqb();
struct button* createbutton(int x, int y, int w, int h, COLORREF color, const char* ptext);
void drawbutton1(struct button* pb);
void drawbutton2(struct button* pb);
void drawbutton3(struct button* pb);
void drawbutton4(struct button* pb);
void drawbutton5(struct button* pb);
int mouseinbutton(struct button* pb, MOUSEMSG m);
int click(struct button* pb, MOUSEMSG m);
//函数
struct button* createbutton(int x, int y, int w, int h, COLORREF color, const char* ptext) {//初始化按钮属性
	struct button* pb = (struct button*)malloc(sizeof(struct button));
	pb->x = x;
	pb->y = y;
	pb->w = w;
	pb->h = h;
	pb->color = color;
	pb->ptext = (char*)malloc(strlen(ptext) + 1);
	strcpy(pb->ptext, ptext);
	return pb;
}
void drawbutton1(struct button* pb) {//画按钮(第一关
	setfillcolor(pb->color);
	settextstyle(35, 0, L"黑体");
	setlinecolor(BLACK);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	fillrectangle(pb->x, pb->y, pb->x + pb->w, pb->y + pb->h);
	outtextxy(pb->x + 20, pb->y + 10, L"第一关");
}
void drawbutton2(struct button* pb) {//画按钮(第二关
	setfillcolor(pb->color);
	settextstyle(35, 0, L"黑体");
	setlinecolor(BLACK);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	fillrectangle(pb->x, pb->y, pb->x + pb->w, pb->y + pb->h);
	outtextxy(pb->x + 20, pb->y + 10, L"第二关");
}
void drawbutton3(struct button* pb) {//画按钮(玩法说明
	setfillcolor(pb->color);
	settextstyle(35, 0, L"黑体");
	setlinecolor(BLACK);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	fillrectangle(pb->x, pb->y, pb->x + pb->w, pb->y + pb->h);
	outtextxy(pb->x + 20, pb->y + 10, L"玩法说明");
}
void drawbutton4(struct button* pb) {//画按钮(返回菜单
	setfillcolor(pb->color);
	settextstyle(35, 0, L"黑体");
	setlinecolor(BLACK);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	fillrectangle(pb->x, pb->y, pb->x + pb->w, pb->y + pb->h);
	outtextxy(pb->x + 20, pb->y + 10, L"返回菜单");
}
void drawbutton5(struct button* pb) {//画按钮(退出游戏
	setfillcolor(pb->color);
	settextstyle(35, 0, L"黑体");
	setlinecolor(BLACK);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	fillrectangle(pb->x, pb->y, pb->x + pb->w, pb->y + pb->h);
	outtextxy(pb->x + 20, pb->y + 10, L"退出游戏");
}
//判断鼠标与按钮位置
int mouseinbutton(struct button* pb, MOUSEMSG m) {
	if (pb->x <= m.x && m.x <= pb->x + pb->w && pb->y <= m.y && m.y <= pb->y + pb->h) {
		pb->color = RED;//按钮变颜色
		return 1;//在里面返回1
	}
	pb->color = YELLOW;
	return 0;//不在里面返回0
}
//是否点击按钮
int click(struct button* pb, MOUSEMSG m) {
	if (mouseinbutton(pb, m) && m.uMsg == WM_LBUTTONDOWN) {
		return 1;
	}
	return 0;
}
//相关数据初始化
void initfish(int type) {
	if (type == role) {  //角色属性
		fish[type].x = width / 2 ;
		fish[type].y = height / 2 ;
		fish[type].dir = aspect_right;
		fish[type].type = role;
		fish[type].w = 90;
		fish[type].h = 60;
	}
	else {//其他鱼随机产生
		fish[type].type = rand() % (fishmaxnums - 1) + 1;//rand()%6+1    [0,5]加一后刚好是[1,6]避开玩家角色
		int dir = rand() % 10 > 5 ? aspect_left : aspect_right;
		fish[type].dir = dir;

		fish[type].y = rand() % 500 + 50;//让每条鱼间隔50并且不在最边缘
		fish[type].x = dir == aspect_left ? rand() % board + width : -1 * rand() % board;
		if (type == 1){
			fish[type].w = 60;
			fish[type].h = 45;
		}
		if (type == 2) {
			fish[type].w = 50;
			fish[type].h = 40;
		}
		if (type == 3) {
			fish[type].w = 50;
			fish[type].h = 50;
		}
		if (type == 4) {
			fish[type].w = 70;
			fish[type].h = 70;
		}
		if (type == 5) {
			fish[type].w = 90;
			fish[type].h = 90;
		}
		if (type == 6) {
			fish[type].w = 75;
			fish[type].h = 50;
		}
		if (type == 7) {
			fish[type].w = 117;
			fish[type].h = 65;
		}
		if (type == 8) {
			fish[type].w = 110;
			fish[type].h = 110;
		}
		if (type == 9) {
			fish[type].w = 150;
			fish[type].h = 106;
		}
		if (type == 10) {
			fish[type].w = 80;
			fish[type].h = 70;
		}
	}
}
void initfishbqb(int type) {//相关数据初始化2
	if (type == role) {  //角色属性
		a[type].x = width / 2;
		a[type].y = height / 2;
		a[type].dir = aspect_right;
		a[type].type = role;
		a[type].w = 100;
		a[type].h = 80;
	}
	else {//其他鱼随机产生
		a[type].type = rand() % (number - 1) + 1;//rand()%6+1    [0,5]加一后刚好是[1,6]避开玩家角色
		int dir = rand() % 10 > 5 ? aspect_left : aspect_right;
		a[type].dir = dir;

		a[type].y = rand() % 500 + 50;//让每条鱼间隔50并且不在最边缘
		a[type].x = dir == aspect_left ? rand() % board + width : -1 * rand() % board;
		if (type == 1) {
			a[type].w = 200;
			a[type].h = 340;
		}
		if (type == 2) {
			a[type].w = 176;
			a[type].h = 100;
		}
		if (type == 3) {
			a[type].w = 89;
			a[type].h = 89;
		}
		if (type == 4) {
			a[type].w = 89;
			a[type].h = 89;
		}
		if (type == 5) {
			a[type].w = 120;
			a[type].h = 94;
		}
		if (type == 6) {
			a[type].w = 65;
			a[type].h = 105;
		}
		if (type == 7) {
			a[type].w = 65;
			a[type].h = 100;
		}
		if (type == 8) {
			a[type].w = 99;
			a[type].h = 75;
		}
		if (type == 9) {
			a[type].w = 225;
			a[type].h = 150;
		}
		if (type == 10) {
			a[type].w = 180;
			a[type].h = 180;
		}
	}
}
//加载资源
void loadresources(){
	loadimage(&img_background, L"background.jpg",width, height);
	int i; 
	int j;
	for (i = 0; i < fishmaxnums; i++) {
		initfish(i);
		for (j = 0; j < 4; j++) { //fishimg[i][0]左掩码图fishimg[i][1]左背景图fishimg[i][2]右掩码图fishimg[i][3]右背景图
			switch (j) {
			case 0:
				if(i==0){loadimage(&fishimg[i][j], L"0_lefty.jpg", fish[i].w, fish[i].h);}
				else if(i==1){ loadimage(&fishimg[i][j], L"1_lefty.jpg", fish[i].w, fish[i].h); }
				else if (i == 2) { loadimage(&fishimg[i][j], L"2_lefty.jpg", fish[i].w, fish[i].h); }
				else if (i == 3) { loadimage(&fishimg[i][j], L"3_lefty.jpg", fish[i].w, fish[i].h); }
				else if (i == 4) { loadimage(&fishimg[i][j], L"4_lefty.jpg", fish[i].w, fish[i].h); }
				else if (i == 5) { loadimage(&fishimg[i][j], L"5_lefty.jpg", fish[i].w, fish[i].h); }
				else if (i == 6) { loadimage(&fishimg[i][j], L"6_lefty.jpg", fish[i].w, fish[i].h); }
				else if (i == 7) { loadimage(&fishimg[i][j], L"7_lefty.jpg", fish[i].w, fish[i].h); }
				else if (i == 8) { loadimage(&fishimg[i][j], L"8_lefty.jpg", fish[i].w, fish[i].h); }
				else if (i == 9) { loadimage(&fishimg[i][j], L"9_lefty.jpg", fish[i].w, fish[i].h); }
				else if (i == 10) { loadimage(&fishimg[i][j], L"10_lefty.jpg", fish[i].w, fish[i].h); }
				break;
			case 1:
				if(i==0){loadimage(&fishimg[i][j], L"0_left.jpg", fish[i].w, fish[i].h);}
				else if (i == 1) { loadimage(&fishimg[i][j], L"1_left.jpg", fish[i].w, fish[i].h); }
				else if (i == 2) { loadimage(&fishimg[i][j], L"2_left.jpg", fish[i].w, fish[i].h); }
				else if (i == 3) { loadimage(&fishimg[i][j], L"3_left.jpg", fish[i].w, fish[i].h); }
				else if (i == 4) { loadimage(&fishimg[i][j], L"4_left.jpg", fish[i].w, fish[i].h); }
				else if (i == 5) { loadimage(&fishimg[i][j], L"5_left.jpg", fish[i].w, fish[i].h); }
				else if (i == 6) { loadimage(&fishimg[i][j], L"6_left.jpg", fish[i].w, fish[i].h); }
				else if (i == 7) { loadimage(&fishimg[i][j], L"7_left.jpg", fish[i].w, fish[i].h); }
				else if (i == 8) { loadimage(&fishimg[i][j], L"8_left.jpg", fish[i].w, fish[i].h); }
				else if (i == 9) { loadimage(&fishimg[i][j], L"9_left.jpg", fish[i].w, fish[i].h); }
				else if (i == 10) { loadimage(&fishimg[i][j], L"10_left.jpg", fish[i].w, fish[i].h); }
				break;
			case 2:
				if (i == 0) { loadimage(&fishimg[i][j], L"0_righty.jpg", fish[i].w, fish[i].h); }
				else if (i == 1) { loadimage(&fishimg[i][j], L"1_righty.jpg", fish[i].w, fish[i].h); }
				else if (i == 2) { loadimage(&fishimg[i][j], L"2_righty.jpg", fish[i].w, fish[i].h); }
				else if (i == 3) { loadimage(&fishimg[i][j], L"3_righty.jpg", fish[i].w, fish[i].h); }
				else if (i == 4) { loadimage(&fishimg[i][j], L"4_righty.jpg", fish[i].w, fish[i].h); }
				else if (i == 5) { loadimage(&fishimg[i][j], L"5_righty.jpg", fish[i].w, fish[i].h); }
				else if (i == 6) { loadimage(&fishimg[i][j], L"6_righty.jpg", fish[i].w, fish[i].h); }
				else if (i == 7) { loadimage(&fishimg[i][j], L"7_righty.jpg", fish[i].w, fish[i].h); }
				else if (i == 8) { loadimage(&fishimg[i][j], L"8_righty.jpg", fish[i].w, fish[i].h); }
				else if (i == 9) { loadimage(&fishimg[i][j], L"9_righty.jpg", fish[i].w, fish[i].h); }
				else if (i == 10) { loadimage(&fishimg[i][j], L"10_righty.jpg", fish[i].w, fish[i].h); }
				break;
			case 3:
				if (i == 0) { loadimage(&fishimg[i][j], L"0_right.jpg", fish[i].w, fish[i].h); }
				else if (i == 1) { loadimage(&fishimg[i][j], L"1_right.jpg", fish[i].w, fish[i].h); }
				else if (i == 2) { loadimage(&fishimg[i][j], L"2_right.jpg", fish[i].w, fish[i].h); }
				else if (i == 3) { loadimage(&fishimg[i][j], L"3_right.jpg", fish[i].w, fish[i].h); }
				else if (i == 4) { loadimage(&fishimg[i][j], L"4_right.jpg", fish[i].w, fish[i].h); }
				else if (i == 5) { loadimage(&fishimg[i][j], L"5_right.jpg", fish[i].w, fish[i].h); }
				else if (i == 6) { loadimage(&fishimg[i][j], L"6_right.jpg", fish[i].w, fish[i].h); }
				else if (i == 7) { loadimage(&fishimg[i][j], L"7_right.jpg", fish[i].w, fish[i].h); }
				else if (i == 8) { loadimage(&fishimg[i][j], L"8_right.jpg", fish[i].w, fish[i].h); }
				else if (i == 9) { loadimage(&fishimg[i][j], L"9_right.jpg", fish[i].w, fish[i].h); }
				else if (i == 10) { loadimage(&fishimg[i][j], L"10_right.jpg", fish[i].w, fish[i].h); }
				break;
			}
		}
	}
}
void loadresourcesbqb() {//加载资源2
	loadimage(&img_bk, L"bk.jpg", width, height);
	int i;  int j;
	for (i = 0; i < number; i++) {
		initfishbqb(i);
		for (j = 0; j < 4; j++) { //fishimg[i][0]左掩码图fishimg[i][1]左背景图fishimg[i][2]右掩码图fishimg[i][3]右背景图
			switch (j) {
			case 0:
				if (i == 0) { loadimage(&bqbimg[i][j], L"0lefty.jpg", a[i].w, a[i].h); }
				else if (i == 1) { loadimage(&bqbimg[i][j], L"10lefty.jpg", a[i].w, a[i].h); }
				else if (i == 2) { loadimage(&bqbimg[i][j], L"11lefty.jpg", a[i].w, a[i].h); }
				else if (i == 3) { loadimage(&bqbimg[i][j], L"12lefty.jpg", a[i].w, a[i].h); }
				else if (i == 4) { loadimage(&bqbimg[i][j], L"13lefty.jpg", a[i].w, a[i].h); }
				else if (i == 5) { loadimage(&bqbimg[i][j], L"14lefty.jpg", a[i].w, a[i].h); }
				else if (i == 6) { loadimage(&bqbimg[i][j], L"15lefty.jpg", a[i].w, a[i].h); }
				else if (i == 7) { loadimage(&bqbimg[i][j], L"16lefty.jpg", a[i].w, a[i].h); }
				else if (i == 8) { loadimage(&bqbimg[i][j], L"17lefty.jpg", a[i].w, a[i].h); }
				else if (i == 9) { loadimage(&bqbimg[i][j], L"18lefty.jpg", a[i].w, a[i].h); }
				else if (i == 10) { loadimage(&bqbimg[i][j], L"19lefty.jpg", a[i].w, a[i].h); }
				break;
			case 1:
				if (i == 0) { loadimage(&bqbimg[i][j], L"0left.jpg", a[i].w, a[i].h); }
				else if (i == 1) { loadimage(&bqbimg[i][j], L"10left.jpg", a[i].w, a[i].h); }
				else if (i == 2) { loadimage(&bqbimg[i][j], L"11left.jpg", a[i].w, a[i].h); }
				else if (i == 3) { loadimage(&bqbimg[i][j], L"12left.jpg", a[i].w, a[i].h); }
				else if (i == 4) { loadimage(&bqbimg[i][j], L"13left.jpg", a[i].w, a[i].h); }
				else if (i == 5) { loadimage(&bqbimg[i][j], L"14left.jpg", a[i].w, a[i].h); }
				else if (i == 6) { loadimage(&bqbimg[i][j], L"15left.jpg", a[i].w, a[i].h); }
				else if (i == 7) { loadimage(&bqbimg[i][j], L"16left.jpg", a[i].w, a[i].h); }
				else if (i == 8) { loadimage(&bqbimg[i][j], L"17left.jpg", a[i].w, a[i].h); }
				else if (i == 9) { loadimage(&bqbimg[i][j], L"18left.jpg", a[i].w, a[i].h); }
				else if (i == 10) { loadimage(&bqbimg[i][j], L"19left.jpg", a[i].w, a[i].h); }
				break;
			case 2:
				if (i == 0) { loadimage(&bqbimg[i][j], L"0righty.jpg", a[i].w, a[i].h); }
				else if (i == 1) { loadimage(&bqbimg[i][j], L"10righty.jpg", a[i].w, a[i].h); }
				else if (i == 2) { loadimage(&bqbimg[i][j], L"11righty.jpg", a[i].w, a[i].h); }
				else if (i == 3) { loadimage(&bqbimg[i][j], L"12righty.jpg", a[i].w, a[i].h); }
				else if (i == 4) { loadimage(&bqbimg[i][j], L"13righty.jpg", a[i].w, a[i].h); }
				else if (i == 5) { loadimage(&bqbimg[i][j], L"14righty.jpg", a[i].w, a[i].h); }
				else if (i == 6) { loadimage(&bqbimg[i][j], L"15righty.jpg", a[i].w, a[i].h); }
				else if (i == 7) { loadimage(&bqbimg[i][j], L"16righty.jpg", a[i].w, a[i].h); }
				else if (i == 8) { loadimage(&bqbimg[i][j], L"17righty.jpg", a[i].w, a[i].h); }
				else if (i == 9) { loadimage(&bqbimg[i][j], L"18righty.jpg", a[i].w, a[i].h); }
				else if (i == 10) { loadimage(&bqbimg[i][j], L"19righty.jpg", a[i].w, a[i].h); }
				break;
			case 3:
				if (i == 0) { loadimage(&bqbimg[i][j], L"0right.jpg", a[i].w, a[i].h); }
				else if (i == 1) { loadimage(&bqbimg[i][j], L"10right.jpg", a[i].w, a[i].h); }
				else if (i == 2) { loadimage(&bqbimg[i][j], L"11right.jpg", a[i].w, a[i].h); }
				else if (i == 3) { loadimage(&bqbimg[i][j], L"12right.jpg", a[i].w, a[i].h); }
				else if (i == 4) { loadimage(&bqbimg[i][j], L"13right.jpg", a[i].w, a[i].h); }
				else if (i == 5) { loadimage(&bqbimg[i][j], L"14right.jpg", a[i].w, a[i].h); }
				else if (i == 6) { loadimage(&bqbimg[i][j], L"15right.jpg", a[i].w, a[i].h); }
				else if (i == 7) { loadimage(&bqbimg[i][j], L"16right.jpg", a[i].w, a[i].h); }
				else if (i == 8) { loadimage(&bqbimg[i][j], L"17right.jpg", a[i].w, a[i].h); }
				else if (i == 9) { loadimage(&bqbimg[i][j], L"18right.jpg", a[i].w, a[i].h); }
				else if (i == 10) { loadimage(&bqbimg[i][j], L"19right.jpg", a[i].w, a[i].h); }
				break;
			}
		}
	}
}
//绘制
void drawfish() {
	int i;
	for (i = 0; i < fishmaxnums; i++) {
		putimage(fish[i].x, fish[i].y, &fishimg[i][fish[i].dir], SRCAND);
		putimage(fish[i].x, fish[i].y, &fishimg[i][fish[i].dir + 1], SRCPAINT);
	}
}
void drawfishbqb() {//绘制2
	putimage(a[0].x, a[0].y, &bqbimg[0][a[0].dir], SRCAND);
	putimage(a[0].x, a[0].y, &bqbimg[0][a[0].dir + 1], SRCPAINT);
	int i;
	for (i = 0; i < number; i++) {
		putimage(a[i].x, a[i].y, &bqbimg[i][a[i].dir], SRCAND);
		putimage(a[i].x, a[i].y, &bqbimg[i][a[i].dir + 1], SRCPAINT);
	}
}
//重新生成自动鱼
void resetfish() {
	int i;
	for (i = 1; i < fishmaxnums; i++) {
		switch (fish[i].dir) {
		case aspect_left:
			if (fish[i].x < -board) {
				initfish(i);
			}
			break;
		case aspect_right:
			if (fish[i].x > width+board) {
				initfish(i);
			}
			break;
		}
	}
}
void resetfishbqb() {//重新生成自动鱼2
	int i;
	for (i = 1; i < number; i++) {
		switch (a[i].dir) {
		case aspect_left:
			if (a[i].x < -board) {
				initfishbqb(i);
			}
			break;
		case aspect_right:
			if (a[i].x > width + board) {
				initfishbqb(i);
			}
			break;
		}
	}
}
//手动控制角色鱼
void control(){
	if (GetAsyncKeyState(VK_LEFT)&&fish[role].x>0) { fish[role].x = fish[role].x - 1;fish[role].dir = aspect_left; }
    if (GetAsyncKeyState(VK_RIGHT)&& fish[role].x<width- fish[role].w) { fish[role].x++; fish[role].dir = aspect_right;}
    if (GetAsyncKeyState(VK_UP)&& fish[role].y>0) { fish[role].y--; }
    if (GetAsyncKeyState(VK_DOWN)&& fish[role].y<height- fish[role].h) { fish[role].y++; }
}
void controlbqb() {//手动控制过程2
	if (GetAsyncKeyState(VK_LEFT) && a[role].x > 0) { a[role].x = a[role].x - 1; a[role].dir = aspect_left; }
	if (GetAsyncKeyState(VK_RIGHT) && a[role].x < width - a[role].w) { a[role].x++; a[role].dir = aspect_right; }
	if (GetAsyncKeyState(VK_UP) && a[role].y > 0) { a[role].y--; }
	if (GetAsyncKeyState(VK_DOWN) && a[role].y < height - a[role].h) { a[role].y++; }
}
//假设吃的是type鱼
int eatfish(int type) {
	int minx = max(fish[role].x, fish[type].x);
	int miny = max(fish[role].y, fish[type].y);
	int maxx = min(fish[role].x + fish[role].w/2, fish[type].x + fish[type].w/2);
	int maxy = min(fish[role].y + fish[role].h/2, fish[type].y + fish[type].h/2);
	if (minx > maxx || miny > maxy) {//判断相交
		return 0;//不相交
	}
	else {
		if (((fish[role].w-10) * (fish[role].h-5)) >= ((fish[type].w-10) * (fish[type].h-5))) {//相交即吃到该鱼
			initfish(type); 
			score1=score1+1;
			if (score1 == 10) {
				mciSendString(L"close BGM.mp3", NULL, NULL, NULL);
				IMAGE win;
				loadimage(&win, L"win.jpg", width-500, height-150);
				struct button* back = createbutton(700, 600, 180, 50, YELLOW, "返回菜单");
				while (1) {
					BeginBatchDraw();
					putimage(100, 100, &win);
					drawbutton4(back);
					MOUSEMSG n = GetMouseMsg();
					if (click(back, n)) { menu2(); }
					EndBatchDraw();
				}//MessageBox(GetHWnd(), TEXT("Congratulations!\n Your score:100!"), TEXT("you win"), MB_OK);menu2();
			}
			return 0;
		}
	}
	return 1;
}
int eatfishbqb(int type) {//假设吃的是type鱼2
	int minx = max(a[role].x, a[type].x);
	int miny = max(a[role].y, a[type].y);
	int maxx = min(a[role].x + a[role].w / 2, a[type].x + a[type].w / 2);
	int maxy = min(a[role].y + a[role].h / 2, a[type].y + a[type].h / 2);//判断相交
	if (minx > maxx || miny > maxy) {
		return 0;//不相交
	}
	else {
		if (((a[role].w - 10) * (a[role].h - 5)) >= ((a[type].w - 10) * (a[type].h - 5))) {//相交即吃到该鱼
			initfishbqb(type);
			score2 = score2 + 1;
			if (score2 == 10) {
				MessageBox(GetHWnd(), TEXT("Congratulations!\n Your score:100!"), TEXT("you win"), MB_OK);
				mciSendString(L"close BGM.mp3", NULL, NULL, NULL);
				menu2();
			}
			return 0;
		}
	}
	return 1;
}
//游戏结束
int gameover() {
	int i;
	for (i = 1; i < fishmaxnums; i++) {
		if (eatfish(i) == 1)
			return 1;
	}
	return 0;
}
int gameoverbqb() {//游戏结束2
	int i;
	for (i = 1; i < number; i++) {
		if (eatfishbqb(i) == 1)
			return 1;
	}
	return 0;
}
//移动自动鱼
void movefish() {
	int i;
	for (i = 1; i < fishmaxnums; i++) {
		switch (fish[i].dir) {
		case aspect_left:
			fish[i].x--;
			break;
		case aspect_right:
			fish[i].x++;
			break;
		}
	}
}
void movefishbqb() {//移动自动鱼2
	int i;
	for (i = 1; i < number; i++) {
		switch (a[i].dir) {
		case aspect_left:
			a[i].x--;
			break;
		case aspect_right:
			a[i].x++;
			break;
		}
	}
}
//定时器（控制自动鱼移动速度）
int ontimer(int duration, int id) {
	static int startime[timermax];
	struct node {
		int endtime;
		struct node* next;
	};
	node *head=NULL; node *tail = NULL;node *p;
	p = (node*)malloc(sizeof(node));
	p->endtime = clock();
	p->next = NULL; 
	if (head == NULL) { head = p; tail = p; }
	else {  tail->next = p;tail = p; }
	if (p->endtime - startime[id] >= duration) 
	{
		startime[id] = p->endtime;
        free(head);
		return 1;
	}
	return 0;
}
//输入用户名和密码
void Get_NP(user* u)
{   int i = 0, j = 0;
	printf("Enter Username:");
	while ((u->user_username[i] = getchar()) != '\n')
	{i++;}
	u->user_username[i] = '\0';
	printf("Enter PassWord:");
	while ((u->user_password[j] = getchar()) != '\n')
	{j++;}
	u->user_password[j] = '\0';
}
//登录
void Login(user* u)
{   Get_NP(u);
	if (UserdataFile_read(u))
	{printf("Login successful!");}
	else
	{   printf("Login fail!");
		printf("\nplease login again\n");
		Login(u);
	}
}
//注册
void Fillout(user* u)
{   Get_NP(u);
	UserdataFile_write(u);
	if (UserdataFile_read(u))
	{
		printf("Fillout successful!");
	}
	else
	{   
		printf("Fillout fail!");
		printf("\nplease fillout again\n");
		Fillout(u);
	}
}
//文件读取
int UserdataFile_read(user* u)
{   FILE* fp;
	char read_result[30];
	int check = 0;
	char protem_userdata[30];
	char protem_username[20];
	char protem_password[8];

	strcpy(protem_username, u->user_username);
	strcpy(protem_password, u->user_password);
	strcpy(protem_userdata, protem_username);

	strcat(protem_userdata, " ");
	strcat(protem_userdata, protem_password);
	strcat(protem_userdata, "\n");
	if ((fp = fopen("user_database.txt", "r")) == NULL)//r打开只读文件
	{   printf("UserdataFile open error!\n");
		exit(0);
	}
	while (!feof(fp))//判断是否为文件末尾，没有到达末尾时
	{
		fgets(read_result, 30, fp);//从f中读取不超过30的字符串存入read_result
		if (strcmp(read_result, protem_userdata) == 0)//比较输入的和已存的
		{   
			check = 1;//内容相同则check的值为1
			break;
		}
	}
	if (fclose(fp))//若关闭文件则提示
	{printf("UserdataFile close error!");}
	return check;
}
//文件写入
void UserdataFile_write(user* u)
{   FILE* fp;
	char protem_userdata[30];
	char protem_username[20];
	char protem_password[8];

	strcpy(protem_username, u->user_username);
	strcpy(protem_password, u->user_password);
	strcpy(protem_userdata, protem_username);

	strcat(protem_userdata, " ");//连接
	strcat(protem_userdata, protem_password);
	strcat(protem_userdata, "\n");
	if ((fp = fopen("user_database.txt", "a")) == NULL)//以附加的方式a打开只写文件
	{   printf("UserdataFile open error!\n");
		exit(0);//正常退出  exit(1);则为非正常退出
	}
	fputs(protem_userdata, fp);//向fp文件写入
	if (fclose(fp))//若关闭文件则提示
	{   printf("UserdataFile close error!");
		exit(0);
	}
}
//菜单
void menu2() {
	IMAGE startbk;
	loadimage(&startbk, L"startbk.jpg", width,height);
	struct button* guide = createbutton(400, 335, 180, 50, YELLOW, "玩法说明");
	struct button* one = createbutton(400, 135, 150, 50, YELLOW, "第一关");
	struct button* two = createbutton(400, 235, 150, 50, YELLOW, "第二关");
	struct button* back = createbutton(700,600, 180, 50, YELLOW, "返回菜单");
	struct button* quit = createbutton(400, 435, 180, 50, YELLOW, "退出游戏");
	while (1) {
		BeginBatchDraw();
		putimage(0, 0, &startbk);
		drawbutton1(one);
		drawbutton2(two);
		drawbutton3(guide);
		drawbutton5(quit);
		MOUSEMSG m = GetMouseMsg();//定义鼠标变量 获取鼠标消息
		if (click(guide, m)) {
			IMAGE introduction;
			loadimage(&introduction,L"introduction.jpg", width, height); 
			while (1) {
				BeginBatchDraw();
                putimage(0,0,&introduction);
			    drawbutton4(back);
			    MOUSEMSG n = GetMouseMsg(); 
				if (click(back, n)) { menu2(); }
			    EndBatchDraw();
			}
		}
		if (click(one, m)) {score1 = 0;
			loadresources();
			BeginBatchDraw();
			while (1) {
				putimage(0, 0, &img_background);
				mciSendString(L"open BGM.mp3", NULL, NULL, NULL);
				mciSendString(L"play BGM.mp3 repeat", NULL, NULL, NULL);
				drawfish();
				FlushBatchDraw();
				control();
				if (ontimer(10, 0)) { movefish(); }
				resetfish();
				if (gameover() == 1) {
					MessageBox(GetHWnd(), TEXT("Game Over"), TEXT("you lose"), MB_OK);
					mciSendString(L"close BGM.mp3", NULL, NULL, NULL);
					menu2();
				}
			}
		}
		if (click(two, m)) {score2 = 0;
			loadresourcesbqb();
			BeginBatchDraw();
			while (1) {
				putimage(0, 0, &img_bk);
				mciSendString(L"open BGM.mp3", NULL, NULL, NULL);
				mciSendString(L"play BGM.mp3 repeat", NULL, NULL, NULL);
				drawfishbqb();
				FlushBatchDraw();
				controlbqb();
				if (ontimer(5, 0)) { movefishbqb(); }
				resetfishbqb();
				if (gameoverbqb() == 1) {
					MessageBox(GetHWnd(), TEXT("Game Over"), TEXT("you lose"), MB_OK);
					mciSendString(L"close BGM.mp3", NULL, NULL, NULL);
					menu2();
				}
			}
		}
		if (click(quit, m)) { closegraph(); }
		EndBatchDraw();
	}
}
void menu1() {
	IMAGE startbk;
	loadimage(&startbk, L"startbk.jpg", width, height);
	struct button* guide = createbutton(400, 335, 180, 50, YELLOW, "玩法说明");
	struct button* one = createbutton(400, 135, 150, 50, YELLOW, "第一关");
	struct button* two = createbutton(400, 235, 150, 50, YELLOW, "第二关");
	struct button* back = createbutton(700, 600, 180, 50, YELLOW, "返回菜单");
	struct button* quit = createbutton(400, 435, 180, 50, YELLOW, "退出游戏");
	while (1) {
		BeginBatchDraw();
		putimage(0, 0, &startbk);
		drawbutton1(one);drawbutton2(two);drawbutton3(guide);drawbutton5(quit);
		MOUSEMSG m = GetMouseMsg();//定义鼠标变量 获取鼠标消息
		if (click(guide, m)) {
			IMAGE introduction;
			loadimage(&introduction, L"introduction.jpg", width, height);
			while (1) {
				BeginBatchDraw();
				putimage(0, 0, &introduction);
				drawbutton4(back);
				MOUSEMSG n = GetMouseMsg();
				if (click(back, n)) { menu1(); }
				EndBatchDraw();
			}
		}
		if (click(one, m)) {score1 = 0;
			loadresources();
			BeginBatchDraw();
			while (1) {
				putimage(0, 0, &img_background);
				mciSendString(L"open BGM.mp3", NULL, NULL, NULL);
				mciSendString(L"play BGM.mp3 repeat", NULL, NULL, NULL);
				drawfish();
				FlushBatchDraw();
				control();
				if (ontimer(10, 0)) { movefish(); }
				resetfish();
				if (gameover() == 1) {
					mciSendString(L"close BGM.mp3", NULL, NULL, NULL);
					MessageBox(GetHWnd(), TEXT("Game Over"), TEXT("you lose"), MB_OK);
					menu1();
				}
			}
		}
		if (click(two, m)) {
			IMAGE key;
			loadimage(&key, L"key.jpg", width, height);
			while (1) {
				BeginBatchDraw();
				putimage(0, 0, &key);
				drawbutton4(back);
				MOUSEMSG n = GetMouseMsg();
				if (click(back, n)) { menu1(); }
				EndBatchDraw();
			}
		}
		if (click(quit, m)) { closegraph(); }
		EndBatchDraw();
	}
	 
}
int main() {//主函数
	user* u = (user*)malloc(sizeof(struct userdata));
	char choice[10];
	int i = 0;
	printf("login or fillout:");
	while ((choice[i] = getchar()) != '\n')
	{i++;}
	choice[i] = '\0';
	if (!strcmp("login", choice))
	{Login(u);}
	else if (!strcmp("fillout", choice)){
		Fillout(u);
		printf("\nplease login\n");
		Login(u);
	}
	else{
		printf("Error!\nplease enter again\n");
		Get_NP(u);
		UserdataFile_write(u);
	}
	srand((unsigned)time(NULL));
	HWND hwnd=initgraph(width, height);
	menu1();
	EndBatchDraw();
	closegraph();
}	





























/*srand((unsigned)time(NULL));
	HWND hwnd1 = initgraph(width, height);
	loadresourcesbqb();
	BeginBatchDraw();
	while (1) {
		putimage(0, 0, &img_bk);
		drawfishbqb();
		FlushBatchDraw();
		controlbqb();
		if (ontimer(10, 0)) { movefishbqb(); }
		resetfishbqb();
		if (gameoverbqb() == 1) {
			MessageBox(GetHWnd(), TEXT("Game Over"), TEXT("you lose"), MB_OK);
			break;
		}
	}*/

/*int ontimer(int duration, int id) {
	static int startime[timermax];
	int endtime = clock();
	if (endtime - startime[id] >= duration) {
		startime[id] = endtime;
		return 1;
	}
	return 0;
}*/

/*IMAGE startbk;
	loadimage(&startbk, L"startbk.jpg", width,height);
	struct button* one = createbutton(300, 150, 150, 50, YELLOW, "第一关");
	struct button* two = createbutton(300, 235, 150, 50, YELLOW, "第二关");
	while (1) {
		BeginBatchDraw();
		putimage(0, 0, &startbk);
		drawbutton1(one);
		drawbutton2(two);
		MOUSEMSG m = GetMouseMsg();//定义鼠标变量 获取鼠标消息
		if (click(one, m)) {
			loadresources();
	        BeginBatchDraw();
		    while (1) {
		        putimage(0, 0, &img_background);
		        mciSendString(L"open BGM.mp3", NULL, NULL, NULL);
	            mciSendString(L"play BGM.mp3 repeat", NULL, NULL, NULL);
		        drawfish();
		        FlushBatchDraw();
		        control();
		        if (ontimer(10, 0)) { movefish(); }
		        resetfish();
		        if (gameover() == 1) {
			         MessageBox(GetHWnd(), TEXT("Game Over"), TEXT("you lose"), MB_OK);
			         mciSendString(L"close BGM.mp3", NULL, NULL, NULL);
					 break;
					 menu();
		        }
	        }
		}
		if (click(two, m)) {
		    loadresourcesbqb();
		    BeginBatchDraw();
			while (1) {
				putimage(0, 0, &img_bk);
				mciSendString(L"open BGM.mp3", NULL, NULL, NULL);
				mciSendString(L"play BGM.mp3 repeat", NULL, NULL, NULL);
				//mciSendString(L"open 第二关bgm.mp3", NULL, NULL, NULL);
				//mciSendString(L"play 第二关bgm.mp3 repeat", NULL, NULL, NULL);
				drawfishbqb();
				FlushBatchDraw();
				controlbqb();
				if (ontimer(5, 0)) { movefishbqb(); }
				resetfishbqb();
				if (gameoverbqb() == 1) {
					MessageBox(GetHWnd(), TEXT("Game Over"), TEXT("you lose"), MB_OK);
					mciSendString(L"close 第二关bgm.mp3", NULL, NULL, NULL);

					break;
				}
			}
		}
		EndBatchDraw();
	}*/