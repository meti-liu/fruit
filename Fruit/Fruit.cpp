#include<graphics.h>
#include<iostream>
#include<conio.h>
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



class Fruit
{
public:
	bool sliced = false;//ˮ��û���п�ʱ
	float x, y;//һЩ��������ĳ�ʼ��
	float vx = 5;
	float vy = -25;
	float g = 0.5;
	float t;
	Fruit() :x(x), y(y) {};//���캯��
	Fruit(float x, float y) :x(x), y(y){};//���캯��

	void draw()//��Ⱦͼ��
	{
		putimage(0, 0, &img_background);//����ͼƬ��Ⱦ
		if(sliced==false) putimage1(x, y, &img_apple1);//ƻ���п�֮ǰ
		else if (sliced == true) putimage1(x, y, &img_apple0);//ƻ���п�֮��

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
	loadimage(&img_background, _T("img/background.jpg"));//ͼƬ����
	loadimage(&img_apple1, _T("img/Apple.png"));//ͼƬ����
	loadimage(&img_apple0, _T("img/AppleSliced.png"));//ͼƬ����
	for(int i=1;i<5;i++) fruit[i] = Fruit(20, height);
}
int main()
{
	startup();
	//bool End = false;
	initgraph(width, height);//w,h
	BeginBatchDraw();//˫����
	ExMessage msg;


	while (!End)//��Ϸ����ѭ��
	{
		//��¼����������
		while (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN)//������
			{
				int mousex = msg.x;
				int mousey = msg.y;

				if (msg.x <= fruit[1].x + img_apple1.getwidth() && msg.x >= fruit[1].x
					&& msg.y <= fruit[1].y + img_apple1.getheight() && msg.y >= fruit[1].y)
					fruit[1].sliced = true;//�����ж�������������ˮ���������Ƿ��ص�
			}

		}






		cleardevice();
		fruit[1].draw();
		fruit[1].update();
		FlushBatchDraw();//˫����
		Sleep(20);
	}
	EndBatchDraw();
	_getch(); // �ȴ�����
	closegraph();
	return 0;
}