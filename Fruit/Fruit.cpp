#include<graphics.h>//easyx��
#include<iostream>
#include<conio.h>//���̴�����getch(),kbhit()
#include<cstdlib>//�������Ҫrand()��srand()
#include<ctime>//ʱ����ʼ�������������ʹ��
using namespace std;
const int width = 1280;
const int height = 960;
bool End = false;

IMAGE img_background, img_apple1, img_apple0,img_banana1,img_banana0;//ͼƬ��Դ

#pragma comment(lib,"MSIMG32.LIB")//windows�Դ���һ������
inline void putimage1(int x, int y, IMAGE* img)//ֱ��ʹ��putimage�����Ļ�
{//ͼƬ�ز���Χ���к�ɫ�����������Լ�д��һ��putimage1�������н���͸���ȴ���
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, {AC_SRC_OVER,0,255,AC_SRC_ALPHA});
}

enum FruitType{Apple,Banana,Bomb,Pineapple};//���ǿ��Ը�����1234��������
                                       //���岻��������ʹ��enum�Զ������͸���ֱ��
class Fruit
{
public:
	FruitType type;
	bool sliced = false;//ˮ��û���п�ʱ
	float x, y;//ˮ��������
	float vx, vy,g;//ˮ��ˮƽ����ֱ������ٶȣ����������ļ��ٶ�
	//float vx = 5;
	//float vy = -25;
	//float g = 0.5;
	float t;
	Fruit() :x(0), y(0),vx(0),vy(0),g(0),type(Apple) {};//����ʹ����һ���޲ε����أ�Ŀ�����ȶ������ô��ˮ������
	Fruit(float x, float y,float vx,float vy,float g,FruitType type) :x(x), y(y),vx(vx),vy(vy),g(g),type(type){};//���캯�������ҳ�ʼ��

	void draw()//��Ⱦͼ��
	{
		putimage(0, 0, &img_background);//����ͼƬ��Ⱦ
		if (type == Apple)
		{
			if (sliced == false) putimage1(x, y, &img_apple1);//ƻ���п�֮ǰ
			else if (sliced == true) putimage1(x, y, &img_apple0);//ƻ���п�֮��
		}
		if (type == Banana)
		{
			if (sliced == false) putimage1(x, y, &img_banana1);//�㽶�п�֮ǰ
			else if (sliced == true) putimage1(x, y, &img_banana0);//�㽶�п�֮��
		}


	}
	void update()//����һЩ������Ϣ
	{
		vy += g;
		y += vy;
		x += vx;
		if (sliced == false)//û���п�ǰ��ײǽ�ᷴ��
		{
			if (y >= height)	vy = -0.95 * vy;
			if (y > height) y = height;
			if (x >= width) vx = -0.95 * vx;
			if (x <= 0) vx = -0.95 * vx;
		}
		if (sliced == true) //���п����ٷ���,�����ٶ���΢�б仯
		{
			vx *= 1.01;
			vy *= 0.95;
		}
	}
};

Fruit fruit[5];//ˮ������Ķ���Ҫ��class����֮��

void startup()
{
	loadimage(&img_background, _T("img/background.jpg"));//����ͼƬ����
	loadimage(&img_apple1, _T("img/Apple.png"));//ƻ����δ�п���ͼƬ����
	loadimage(&img_apple0, _T("img/AppleSliced.png"));//�п���ƻ��ͼƬ����
	loadimage(&img_banana1, _T("img/Banana.png"));//�㽶δ�п�ͼƬ����
	loadimage(&img_banana0, _T("img/BananaSliced.png"));//�п����㽶ͼƬ����
	fruit[1] = Fruit(20,height,5,-25,0.5,Apple);
	fruit[2] = Fruit(800, 0, -5, 20, 0.5,Banana);
}
int main()
{
	startup();
	//bool End = false;
	initgraph(width, height);//w,h
	BeginBatchDraw();//˫����
	ExMessage msg;
	srand(static_cast<unsigned int>(time(0)));//����randʹ�ã���ÿ�������һ��
	int r0 = rand() % 2 + 1;//rand()%2�������0��1��������������ˮ�����±�

	while (!End)//��Ϸ����ѭ��
	{
		//��¼����������
		while (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN)//������
			{
				int mousex = msg.x;
				int mousey = msg.y;

				if (msg.x <= fruit[r0].x + img_apple1.getwidth() && msg.x >= fruit[r0].x
					&& msg.y <= fruit[r0].y + img_apple1.getheight() && msg.y >= fruit[r0].y)
					fruit[r0].sliced = true;//�����ж�������������ˮ���������Ƿ��ص�
			}

		}



		cleardevice();
		fruit[r0].draw();
		fruit[r0].update();
		FlushBatchDraw();//˫����
		Sleep(20);
	}
	EndBatchDraw();
	_getch(); // �ȴ�����
	closegraph();
	return 0;
}