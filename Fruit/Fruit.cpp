#include<graphics.h>
#include<iostream>
#include<conio.h>
using namespace std;
const int width = 1280;
const int height = 960;
bool End = false;

IMAGE img_background, img_apple1, img_apple0,img_banana1,img_banana0;//图片资源

#pragma comment(lib,"MSIMG32.LIB")//windows自带的一个处理
inline void putimage1(int x, int y, IMAGE* img)//直接使用putimage函数的话
{//图片素材周围会有黑色框，于是我们自己写了一个putimage1函数，有进行透明度处理
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, {AC_SRC_OVER,0,255,AC_SRC_ALPHA});
}



class Fruit
{
public:
	bool sliced = false;//水果没被切开时
	float x, y;//一些物理参数的初始化
	float vx = 5;
	float vy = -25;
	float g = 0.5;
	float t;
	Fruit() :x(x), y(y) {};//构造函数
	Fruit(float x, float y) :x(x), y(y){};//构造函数

	void draw()//渲染图像
	{
		putimage(0, 0, &img_background);//背景图片渲染
		if(sliced==false) putimage1(x, y, &img_apple1);//苹果切开之前
		else if (sliced == true) putimage1(x, y, &img_apple0);//苹果切开之后

	}
	void update()//更新一些物理信息
	{
		vy += g;
		y += vy;
		x += vx;
		if (sliced == false)//没被切开前，撞墙会反弹
		{
			if (y >= height)	vy = -0.95 * vy;
			if (y > height) y = height;
			if (x >= width) vx = -0.95 * vx;
			if (x <= 0) vx = -0.95 * vx;
		}
		if (sliced == true) //被切开后不再反弹,受力速度略微有变化
		{
			vx *= 1.01;
			vy *= 0.95;
		}
	}
};

Fruit fruit[5];//水果数组的定义要遭class定义之后

void startup()
{
	loadimage(&img_background, _T("img/background.jpg"));//图片加载
	loadimage(&img_apple1, _T("img/Apple.png"));//图片加载
	loadimage(&img_apple0, _T("img/AppleSliced.png"));//图片加载
	for(int i=1;i<5;i++) fruit[i] = Fruit(20, height);
}
int main()
{
	startup();
	//bool End = false;
	initgraph(width, height);//w,h
	BeginBatchDraw();//双缓冲
	ExMessage msg;


	while (!End)//游戏的主循环
	{
		//记录下鼠标的坐标
		while (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN)//鼠标左键
			{
				int mousex = msg.x;
				int mousey = msg.y;

				if (msg.x <= fruit[1].x + img_apple1.getwidth() && msg.x >= fruit[1].x
					&& msg.y <= fruit[1].y + img_apple1.getheight() && msg.y >= fruit[1].y)
					fruit[1].sliced = true;//我们判断了鼠标的坐标与水果的坐标是否重叠
			}

		}






		cleardevice();
		fruit[1].draw();
		fruit[1].update();
		FlushBatchDraw();//双缓冲
		Sleep(20);
	}
	EndBatchDraw();
	_getch(); // 等待按键
	closegraph();
	return 0;
}