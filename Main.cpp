#include <iostream>
#include <vector>
#include <windows.h>
#include <queue>
#include <algorithm>
#include <Windows.h>    
#include <gl/GL.h>    
#include <gl/GLU.h>    
#include <glut.h>
#include <string>

using namespace std;

int x;
int len = 7;
int result[7] = { 1000,1000,1000,1000,1000,1000,1000 };
int value[7] = {3, 1, 7, 6, 4, 2, 5};
bool flag[7] = { false,false,false,false,false,false,false };
int MAX_LEVEL = 0;
int LEVEL_HEIGHT = 0;
float WIDTH_EL = 0.0f;
const int NODE_RADIUS = 20;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
vector <vector<int>> matrix = {
{0, 42, 19, 0, 0, 0, 12 },
{42, 0, 0, 23, 19, 0, 0 },
{19, 0, 0, 0, 34, 30, 0 },
{0, 23, 0, 0, 0, 9, 0 },
{0, 19, 34, 0, 0, 0, 47 },
{0, 0, 30, 9, 0, 0, 26 },
{12, 0, 0, 0, 47, 26, 0 }
};

float getNext(int next)
{
	for (int i = 0; i < len; i++)
	{
		if (next + 1 == value[i])
		{
			return i;
		}
	}
}
void drawLine(float sx, float sy, float ex, float ey, int val)
{
	float x = sx < ex ? -1 : 1;
	float y = sy > ey ? 1 : -1;

	float w = abs(sx - ex) + 4;
	float h = abs(sy - ey) + 4;

	glViewport(min(sx, ex), min(sy, ey), w, h);

	glLineWidth(4);
	glColor3ub(100, 100, 100);

	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(-x, -y);
	glEnd();

	float textW = w + 20 * x;
	float textH = h + 20 * y;
	float textX = (ex + ((sx + ex) / 2)) / 2;
	float textY = (ey + ((sy + ey) / 2)) / 2;
	glViewport(textX, textY, abs(sx - (sx + ((sx + ex) / 2)) / 2)-20, 
		abs(sy - (sy + ((sy + ey) / 2)) / 2)-20);
	glColor3ub(255, 255, 255);
	string text = to_string(val);
	glRasterPos2f(0, 0);
	for (const char* p = text.c_str(); *p != '\0'; p++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);
	}
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}
void drawNode(float cx, float cy, double value, int num_segments = 20) 
{
	float th = 2.0f * 3.1415926f / float(num_segments);

	// Set new viewport
	glViewport(cx - NODE_RADIUS, cy - NODE_RADIUS, NODE_RADIUS * 2, NODE_RADIUS * 2);

	// Draw circle
	glColor3ub(255, 255, 255);
	glBegin(GL_TRIANGLE_FAN);
	for (int ii = 0; ii < num_segments; ii++) {
		glVertex2f(cosf(th * ii), sinf(th * ii));
	}
	glEnd();

	// Prepare text 0,1230000 => 0,123
	string text = to_string(value);
	int foundedIndex = text.length();
	for (int i = text.length() - 1; i >= 0; i--) {
		if (text[i] == (char)'0') {
			foundedIndex = i;
		}
		else {
			continue;
		}
	}
	if (text[foundedIndex - 1] == (char)',') {
		foundedIndex++;
	}
	text = text.substr(0, foundedIndex);

	// Draw Value
	glColor3ub(11, 65, 179);

	float xPos = -0.25 * (text.length() - 1.25);
	glRasterPos2f(xPos, -0.31);
	for (const char* p = text.c_str(); *p != '.'; p++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
	}

	// Return viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}
void displayNodeIter() 
{
	float cx = WINDOW_WIDTH / 2;
	float cy = WINDOW_HEIGHT / 2;
	float th = 2.0f * 3.1415926f / len;
	for (int ii = 0; ii < len; ii++) {
		for (int j = value[ii]; j < len; j++)
		{
			if (matrix[value[ii]-1][j] != 0)
			{
				drawLine(
					// ii => 0..len
					cx + cosf(th * ii) * 200,
					cy + sinf(th * ii) * 200, 
					cx + cosf(th * getNext(j)) * 200,
					cy + sinf(th * getNext(j)) * 200,
					matrix[value[ii] - 1][j]);
			}
		}
	}
	for (int ii = 0; ii < len; ii++) {
		drawNode(cx + cosf(th * ii) * 200, cy + sinf(th * ii) * 200, value[ii]);
	}
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT); // очищение
	float y = getNext(1);
	displayNodeIter();
	glFlush();
}
void drawGrafGL(int argc, char** argv) {
	
	//WIDTH_EL = round(WINDOW_WIDTH / (pow(2, MAX_LEVEL - 1) + 1) / 2);
	//LEVEL_HEIGHT = round(WINDOW_HEIGHT / (MAX_LEVEL + 1));

	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); //создание окна
	glutInitWindowPosition(80, 80); // указание позиции окна
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow("Graf");
	glutDisplayFunc(display);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glutMainLoop();
}


bool comp(int a, int b)
{
	return matrix[x][a] < matrix[x][b];
}

int main(int argc, char** argv)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	queue <int> qu;
	cout << "Введите вершину графа(1<=x<=" << len << ")\n";
	cin >> x;
	x--;
	result[x] = 0;
	qu.push(x);
	while (true)
	{
		x = qu.front();
		vector <int> vec;
		for (int i = 0; i < len; i++)
		{
			if (matrix[x][i] != 0 && !flag[i])
			{
				vec.push_back(i);
			}
		}
		if (vec.size() == 0)
		{
			flag[x] = true;
			qu.pop();
			if (qu.size() == 0) break;
			continue;
		}
		sort(vec.begin(), vec.end(), comp);
		for (int i = 0; i < vec.size(); i++)
		{
			result[vec[i]] = min(result[vec[i]], matrix[x][vec[i]] + result[x]);
			qu.push(vec[i]);
		}
		flag[x] = true;
		qu.pop();
		if (qu.size() == 0) break;
	}

	cout << "Кратчайшее расстояние от заданной вершины до остальных:" << endl;
	for (int i = 0; i < len; i++)
	{
		if (result[i] == 1000) cout << i + 1 << " - ";
		else cout << i + 1 << " - " << result[i] << endl;
	}
	drawGrafGL(argc, argv);
	//system("pause");
}