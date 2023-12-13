#include<graphics.h>//easyx库
#include<iostream>
#include<conio.h>//键盘传感器getch(),kbhit()
#include<cstdlib>//随机数需要rand()和srand()
#include<ctime>//时间库初始化，搭配随机库使用
#include<vector>//水果数量未知，我们采用动态数组的方式来记录水果
using namespace std;
const int width = 1280;
const int height = 960;
unsigned long lasttime = 0;//我们尝试通过一个计时器来逐渐增加水果个数
int fruit_width;
int fruit_height;
bool End = false;
int r0;//随机数
IMAGE img_background, img_apple1, img_apple0,img_banana1,img_banana0;//图片资源

#pragma comment(lib,"MSIMG32.LIB")//windows自带的一个处理
inline void putimage1(int x, int y, IMAGE* img)//直接使用putimage函数的话
{//图片素材周围会有黑色框，于是我们自己写了一个putimage1函数，有进行透明度处理
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, {AC_SRC_OVER,0,255,AC_SRC_ALPHA});
}

enum FruitType{Apple,Banana,Bomb,Pineapple};//我们可以给代号1234，但这样
                                       //意义不明，我们使用enum自定义类型更加直观
class Fruit
{
public:
	FruitType type;
	bool sliced = false;//水果没被切开时
	float x, y;//水果的坐标
	float vx, vy,g;//水果水平，垂直方向的速度，受力产生的加速度
	//float vx = 5;
	//float vy = -25;
	//float g = 0.5;
	float t;
	bool out = false;
	Fruit() :x(0), y(0),vx(0),vy(0),g(0),type(Apple) {};//这里使用了一个无参的重载，目的是先定义出这么个水果数组
	Fruit(float x, float y,float vx,float vy,float g,FruitType type) :x(x), y(y),vx(vx),vy(vy),g(g),type(type){};//构造函数，并且初始化

	void draw()//渲染图像
	{

		if (type == Apple)
		{
			if (sliced == false) putimage1(x, y, &img_apple1);//苹果切开之前
			else if (sliced == true) putimage1(x, y, &img_apple0);//苹果切开之后
		}
		else if (type == Banana)
		{
			if (sliced == false) putimage1(x, y, &img_banana1);//香蕉切开之前
			else if (sliced == true) putimage1(x, y, &img_banana0);//香蕉切开之后
		}

		cout << "Drawing fruit " << type << " at (" << x << ", " << y << "), sliced: " << sliced << endl;
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
	/*void out()//水果弹两次退出舞台
	{
		int num = 0;
		while (num <= 2)
		{
			if (y <= 0 || y >= height) num++;
		}	
	}*/
};

Fruit type[5];//水果数组type的定义要遭class定义之后
vector<Fruit> fruit;//使用动态数组来定义水果，因为其个数不确定

void startup()
{
	loadimage(&img_background, _T("img/background.jpg"));//背景图片加载
	loadimage(&img_apple1, _T("img/Apple.png"));//苹果（未切开）图片加载
	loadimage(&img_apple0, _T("img/AppleSliced.png"));//切开的苹果图片加载
	loadimage(&img_banana1, _T("img/Banana.png"));//香蕉未切开图片加载
	loadimage(&img_banana0, _T("img/BananaSliced.png"));//切开的香蕉图片加载

	fruit_width = img_apple1.getwidth();
	fruit_height = img_apple1.getheight();
	type[1] = Fruit(20,height,5,-25,0.5,Apple);
	type[2] = Fruit(800, 0, -5, 20, 0.5,Banana);
}
void Addfruit()//添加水果的函数，搭配计时器使用
{
	unsigned long currenttime = GetTickCount();//搭配最初定义的lasttime使用
	const unsigned long interval = 5000;//5000毫秒，5s
	if (currenttime - lasttime > interval)
	{
		r0 = rand() % 2 + 1;//rand()%2就是随机0或1，这里用于生成水果的下标
		fruit.push_back(type[r0]);
		lasttime = currenttime;
		cout << "Adding fruit: " << r0 << " at time " << currenttime << endl;
	}

}
int main()
{
	startup();
	//bool End = false;
	initgraph(width, height);//w,h
	BeginBatchDraw();//双缓冲
	ExMessage msg;
	srand(static_cast<unsigned int>(time(0)));//搭配rand使用，让每次随机不一样

	while (!End)//游戏的主循环
	{
		Addfruit();

		//记录下鼠标的坐标
		while (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN)//鼠标左键
			{
				for (auto& f : fruit)
				{
					int mousex = msg.x;
					int mousey = msg.y;

					if (msg.x <= f.x + fruit_width && msg.x >= f.x
						&& msg.y <= f.y + fruit_height && msg.y >= f.y)
						f.sliced = true;//我们判断了鼠标的坐标与水果的坐标是否重叠
				}

			}

		}



		cleardevice();
		putimage(0, 0, &img_background);//背景图片渲染
		for (auto& f : fruit)
		{
			f.draw();
			f.update();
		}

		FlushBatchDraw();//双缓冲
		Sleep(20);
	}
	EndBatchDraw();
	_getch(); // 等待按键
	closegraph();
	return 0;
}