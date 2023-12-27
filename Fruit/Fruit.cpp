#include<graphics.h>//easyx��
#include<iostream>
#include<conio.h>//���̴�����getch(),kbhit()
#include<cstdlib>//�������Ҫrand()��srand()
#include<ctime>//ʱ����ʼ�������������ʹ��
#include<vector>//ˮ������δ֪�����ǲ��ö�̬����ķ�ʽ����¼ˮ��
using namespace std;
#pragma comment(lib,"winmm.lib")//���ڲ�����Ƶ�Ŀ�
const int width = 1440;
const int height = 900;
unsigned long lasttime = 0;//���ǳ���ͨ��һ����ʱ����������ˮ������
int fruit_width;
int fruit_height;
int Score = 0;//��Ϸ����
bool End = false;
int r0;//�����
bool pause = false;
IMAGE img_background, img_apple1, img_apple0, img_banana1, img_banana0;//ͼƬ��Դ
IMAGE img_pineapple0, img_pineapple1, img_bomb;

#pragma comment(lib,"MSIMG32.LIB")//windows�Դ���һ������
inline void putimage1(int x, int y, IMAGE* img)//ֱ��ʹ��putimage�����Ļ�
{//ͼƬ�ز���Χ���к�ɫ�����������Լ�д��һ��putimage1�������н���͸���ȴ���
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

enum FruitType { Apple, Banana, Pineapple, Bomb };//���ǿ��Ը�����1234��������
//���岻��������ʹ��enum�Զ������͸���ֱ��




void GamePause()//��Ϸ��ͣ
{
	pause = !pause; // �л���ͣ״̬

	if (pause)
	{
		// ��ʾһ����ͣ���ı���ʾ
		settextcolor(RED);
		settextstyle(40, 0, _T("����"));
		outtextxy(width / 2 - 100, height / 2, _T("��Ϸ��ͣ"));
		FlushBatchDraw(); // ����ˢ�£���ʾ��ͣ��Ϣ
	}
}
class Fruit
{
public:
	FruitType type;
	bool sliced = false;//ˮ��û���п�ʱ
	bool scored = false;//δ�ӷֵ�״̬����ֹ�ظ��ӷ�
	int points;//ˮ����Ӧ�ķ���
	float x, y;//ˮ��������
	float vx, vy, g;//ˮ��ˮƽ����ֱ������ٶȣ����������ļ��ٶ�
	//float vx = 5;
	//float vy = -25;
	//float g = 0.5;
	float t;
	bool out = false;
	Fruit() :x(0), y(0), vx(0), vy(0), g(0), type(Apple), points(0) {};//����ʹ����һ���޲ε����أ�Ŀ�����ȶ������ô��ˮ������
	Fruit(float x, float y, float vx, float vy, float g, FruitType type, int points) :x(x), y(y), vx(vx), vy(vy), g(g), type(type), points(points) {};//���캯�������ҳ�ʼ��
	//����������캯���ĳ�ʼ����ˮ���Ĺ��Բ��÷Ž�ȥ�������ʼ״̬sliced=false�����Ǽ������ˮ���ĸ���




	void drawEnd()
	{
		TCHAR scoreStr[50];
		_stprintf_s(scoreStr, _T("�÷�: %d"), Score);
		MessageBox(GetHWnd(), scoreStr, _T("��Ϸ����"), MB_OK);
	}

	void draw()//��Ⱦͼ��
	{

		if (type == Apple)
		{
			if (sliced == false) putimage1(x, y, &img_apple1);//ƻ���п�֮ǰ
			else if (sliced == true) putimage1(x, y, &img_apple0);//ƻ���п�֮��
		}
		else if (type == Banana)
		{
			if (sliced == false) putimage1(x, y, &img_banana1);//�㽶�п�֮ǰ
			else if (sliced == true) putimage1(x, y, &img_banana0);//�㽶�п�֮��
		}
		else if (type == Pineapple)
		{
			if (sliced == false) putimage1(x, y, &img_pineapple1);//�㽶�п�֮ǰ
			else if (sliced == true) putimage1(x, y, &img_pineapple0);//�㽶�п�֮��
		}
		else if (type == Bomb)
		{
			if (sliced == false) putimage1(x, y, &img_bomb);//�㽶�п�֮ǰ
			if (sliced == true)
			{
				End = true;//�п�ը����Ϸ����
				drawEnd();
			}
		}
		//cout << "Drawing fruit " << type << " at (" << x << ", " << y << "), sliced: " << sliced << endl;
	}
	void update()//����һЩ������Ϣ
	{
		vy += g;
		y += vy;
		x += vx;
		if (sliced == false)//û���п�ǰ��ײǽ�ᷴ��
		{
			if (y >= height)	vy = -0.96 * vy;
			if (y > height) y = height;
			//if (y < 0)y = 0;
			if (x >= width) vx = -0.96 * vx;
			if (x <= 0) vx = -0.96 * vx;
		}
		if (sliced == true) //���п����ٷ���,�����ٶ���΢�б仯
		{
			vx *= 1.01;
			vy *= 0.95;
		}
	}
	/*void out()//ˮ���������˳���̨
	{
		int num = 0;
		while (num <= 2)
		{
			if (y <= 0 || y >= height) num++;
		}
	}*/
};

Fruit type[5];//ˮ������type�Ķ���Ҫ��class����֮��
vector<Fruit> fruit;//ʹ�ö�̬����������ˮ������Ϊ�������ȷ��

void startup()
{
	loadimage(&img_background, _T("D:/game/lbr/Fruit/img/background.jpg"));//����ͼƬ����
	loadimage(&img_apple1, _T("D:/game/lbr/Fruit/img/Apple.png"));//ƻ����δ�п���ͼƬ����
	loadimage(&img_apple0, _T("D:/game/lbr/Fruit/img/AppleSliced.png"));//�п���ƻ��ͼƬ����
	loadimage(&img_banana1, _T("D:/game/lbr/Fruit/img/Banana.png"));//�㽶δ�п�ͼƬ����
	loadimage(&img_banana0, _T("D:/game/lbr/Fruit/img/BananaSliced.png"));//�п����㽶ͼƬ����
	loadimage(&img_pineapple1, _T("D:/game/lbr/Fruit/img/Pineapple.png"));//����δ�п�ͼƬ����
	loadimage(&img_pineapple0, _T("D:/game/lbr/Fruit/img/PineappleSliced.png"));//�п��Ĳ���ͼƬ����
	loadimage(&img_bomb, _T("D:/game/lbr/Fruit/img/FatalBomb.png"));//ը��δ�п�ͼƬ����


	fruit_width = img_apple1.getwidth();//����Fruit��img��С���
	fruit_height = img_apple1.getheight();
	type[1] = Fruit(20, height, 5, -25, 0.5, Apple, 10);//���������͵�ˮ���������ʼ��
	type[2] = Fruit(800, 0, -5, 20, 0.5, Banana, 20);
	//type[3] = Fruit(800, 0, -5, 20, 0.5, Pineapple);
	type[3] = Fruit(800, height, -5, 30, 0.5, Pineapple, 15);
	type[4] = Fruit(20, 0, 3, 15, 0.4, Bomb, 0);
}
void Addfruit()//���ˮ���ĺ����������ʱ��ʹ��
{
	unsigned long currenttime = GetTickCount();//������������lasttimeʹ��
	const unsigned long interval = 5000;//5000���룬5s
	if (currenttime - lasttime > interval)
	{
		r0 = rand() % 4 + 1;//rand()%3�������0��1��2,3��������������ˮ��type���±�
		fruit.push_back(type[r0]);//��vector�����һ�������type
		lasttime = currenttime;
		cout << "Adding fruit: " << r0 << " at time " << currenttime << endl;
	}

}
int main()
{
	startup();

	initgraph(width, height);//w,h����ͼƬ�ĳ���

	mciSendString(L"play D:/game/lbr/Fruit/music/Fruit.mp3 repeat", 0, 0, 0);//��������


	BeginBatchDraw();//˫����
	ExMessage msg;
	srand(static_cast<unsigned int>(time(0)));//����randʹ�ã���ÿ�������һ��

	while (!End)//��Ϸ����ѭ��
	{



		//��¼����������
		while (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN)//������
			{
				for (auto& f : fruit)//vector�ı���
				{
					int mousex = msg.x;//��¼����xy����
					int mousey = msg.y;

					if (msg.x <= f.x + fruit_width && msg.x >= f.x
						&& msg.y <= f.y + fruit_height && msg.y >= f.y)
					{
						f.sliced = true;//�����ж�������������ˮ���������Ƿ��ص�
						if (f.scored == false)
						{
							Score += f.points;//��ͬˮ���ӷ�Ч����ͬ
							f.scored = true;//��ֹ�ظ��ӷ�
						}
					}

				}

			}

			else if (msg.message == WM_KEYDOWN)
			{
				if (msg.vkcode == VK_SPACE)//�����̵Ŀո��
				{
					GamePause();
				}
			}
		}

		if (!pause)
		{
			Addfruit();//��ˮ��
			cleardevice();
			putimage(0, 0, &img_background);//����ͼƬ��Ⱦ
			// ��ʾ�÷�
			settextcolor(WHITE);
			settextstyle(25, 0, _T("����"));
			TCHAR scoreStr[20];
			_stprintf_s(scoreStr, _T("�÷�: %d"), Score);
			outtextxy(10, 10, scoreStr);
			for (auto& f : fruit)//vector�дӵ�һ��Ԫ�ص����һ��
			{
				f.draw();
				f.update();
			}



			FlushBatchDraw();//˫����
		}


		Sleep(20);
	}
	EndBatchDraw();
	closegraph();
	return 0;
}
