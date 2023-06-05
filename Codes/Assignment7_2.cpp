#include <GL/glut.h>
#include <cmath>
#include <iostream>


using namespace std;

int windowWidth, windowHeight;


struct Point {
	double x, y;
};


Point controlPoints[3] = {{200, 200}, {300, 450}, {500, 150}};


int numControlPoints = 3;


void drawPoint(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}


void drawControlPoints() {
	glColor3f(1.0, 1.0, 0.0);
	glPointSize(3);
	for (int i = 0; i < numControlPoints; i++) {
		drawPoint(controlPoints[i].x, controlPoints[i].y);
	}
}


int Factorial(int n) {
	int ans = 1;
	for (int i = 1; i <= n; i++) {
		ans *= i;
	}
	return ans;
}


int Combination(int n, int k) {
	int ans = 1;
	ans *= (double) Factorial(n) / (Factorial(n - k) * Factorial(k));
	return ans;
}


double bezierBlendingFunc(double t, int n, int k) {
	return Combination(n, k) * pow(t, k) * pow(1 - t, n - k); 
}


void drawBezierCurve() {
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(1);
	
	for (double t = 0.0; t <= 1.0; t += 0.001) {
		double x = 0 ,y = 0;
		for (int i = 0; i < numControlPoints; i++) {
			x += bezierBlendingFunc(t, numControlPoints - 1, i) * controlPoints[i].x;
			y += bezierBlendingFunc(t, numControlPoints - 1, i) * controlPoints[i].y;
		}
		
		drawPoint(x, y);
	}
}


void Init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(0, windowWidth, 0, windowHeight);
}


void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawControlPoints();
	drawBezierCurve();
	glFlush();
}


int pointToBeChanged = 2;


void myMouse(int button, int action, int xMouse, int yMouse) {
	if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) {
		controlPoints[pointToBeChanged].x = xMouse;
		controlPoints[pointToBeChanged].y = 800 - yMouse;
		myDisplay();
	}
	
}


	
void myKeyboard(unsigned char key, int xMouse, int yMouse) {
	if (isdigit(key)) {
		pointToBeChanged = key - '1';
	}
}	
	

int main(int argc, char** argv) {
	windowWidth = windowHeight = 800;

	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Assignment7_2");
	
	Init();
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMainLoop();
	
}
