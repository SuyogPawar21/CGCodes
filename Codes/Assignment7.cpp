#include <GL/glut.h>
#include <cmath>
#include <iostream>


using namespace std;

#define RADIAN (M_PI / 180)

int windowWidth, windowHeight;
int iterations = 3;
bool mousePressedBefore = false;
int x, y;

float mouseYToWindowY(float y) {
	return windowHeight - y;
}

void drawLine(int x1, int y1, int x2, int y2) {
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}


double distBetween2Points(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


void drawKochLine(double x1, double y1, double x2, double y2, int n) {
	
	double x3 = (x2 + 2 * x1) / 3.0;
	double y3 = (y2 + 2 * y1) / 3.0;
	
	double x4 = (2 * x2 + x1) / 3.0;
	double y4 = (2 * y2 + y1) / 3.0;
	
	double xh = x4 + ((x3 - x4) * cos(60 * RADIAN)) + ((y3 - y4) * sin(60 * RADIAN));
	double yh = y4 - ((x3 - x4) * sin(60 * RADIAN)) + ((y3 - y4) * cos(60 * RADIAN));
	
	if (n <= 1) {
		drawLine(x1, y1, x3, y3);
		drawLine(x3, y3, xh, yh);
		drawLine(xh, yh, x4, y4);
		drawLine(x4, y4, x2, y2);
		return;
	}
	
	drawKochLine(x1, y1, x3, y3, n - 1);
	drawKochLine(x3, y3, xh, yh, n - 1);
	drawKochLine(xh, yh, x4, y4, n - 1);
	drawKochLine(x4, y4, x2, y2, n - 1);
	
}

void myKeyboard(unsigned char key, int xMouse, int yMouse) {
	if (isdigit(key)) {
		iterations = key - '0';
	}
}

void myMouse(int button, int action, int xMouse, int yMouse) {

	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		if (mousePressedBefore == false) {
			x = xMouse;
			y = yMouse;
			mousePressedBefore = true;
		}
		else {
			drawKochLine(x,mouseYToWindowY(y),
			xMouse, mouseYToWindowY(yMouse), iterations);
			mousePressedBefore = false;
		}
	}
	
	glFlush();
}
	

void Init() {

	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, windowWidth, 0, windowHeight);
	
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glFlush();
}

int main(int argc, char** argv) {

	windowWidth = windowHeight = 800;
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Assignment7");
	
	Init();
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMainLoop();
	
}
