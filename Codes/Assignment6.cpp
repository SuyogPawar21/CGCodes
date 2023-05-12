#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <stdlib.h>

using namespace std;

class Point {

	public:
		float x ,y;
		
		Point(float x, float y) {
			this->x = x;
			this->y = y;
		}
		
		Point() {
			this->x = this->y = 0;
		}
};

int windowWidth, windowHeight;

int verticesCount = 0;

bool objectDrawn = false;

Point points[20];

float screenXToWindowX(float x) {
	return x - windowWidth/2;
}

float screenYToWindowY(float y) {
	return windowHeight/2 - y;
}


void drawPolygon() {
	glColor3f(1.0, 1.0, 0.0);
	for (int i = 0; i < verticesCount - 1; i++) {
		glBegin(GL_LINES);
		glVertex2f(points[i].x, points[i].y);
		glVertex2f(points[i + 1].x, points[i + 1].y);
		glEnd();
	}
	glBegin(GL_LINES);
	glVertex2f(points[verticesCount - 1].x, points[verticesCount - 1].y);
	glVertex2f(points[0].x, points[0].y);
	glEnd();
	glFlush();
}

void drawCoordinateAxes() {
	glColor3f(1.0, 1.0, 1.0);
	
	glBegin(GL_LINES);
	glVertex2i(0, windowHeight/2);
	glVertex2i(0, -windowHeight/2);
	glEnd();
	
	glBegin(GL_LINES);
	glVertex2i(windowWidth/2, 0);
	glVertex2i(-windowWidth/2, 0);
	glEnd();
	
	glFlush();
}

void Init() {
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-windowWidth/2, windowWidth/2, -windowHeight/2, windowHeight/2);
}


void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	drawCoordinateAxes();
	glFlush();
}


void translation(int tX, int tY) {
	
	for (int i = 0; i < verticesCount; i++) {
		points[i].x += tX;
		points[i].y += tY;
	}
	
}


void scalingAboutFixedPoint(float sX, float sY) {
	
	Point fixedPoint(points[0].x, points[0].y);
	
	for (int i = 0; i < verticesCount; i++) {
		points[i].x = (points[i].x * sX) + (fixedPoint.x * (1 - sX));
		points[i].y = (points[i].y * sY) + (fixedPoint.y * (1 - sY));
	}
	
}


float calDistFromOrigin(float x, float y) {
	return sqrt(pow(x, 2) + pow(y, 2));
}

float calAngleFromOrigin(float x, float y) {
	return atan(y / x);
}


void rotation(float rotationAngle) {
	
	float distFromOrigin, angleFromOrigin;
	 
	for (int i = 0; i < verticesCount; i++) {
		distFromOrigin = calDistFromOrigin(points[i].x, points[i].y);
		angleFromOrigin = calAngleFromOrigin(points[i].x, points[i].y);
		
		points[i].x = distFromOrigin * cos(rotationAngle + angleFromOrigin);
		points[i].y = distFromOrigin * sin(rotationAngle + angleFromOrigin);
	}
	
}

void xShear(float shX) {
	for (int i = 0; i < verticesCount; i++) {
		points[i].x += shX * points[i].y;
	}
}

void yShear(float shY) {
	for (int i = 0 ; i < verticesCount; i++) {
		points[i].y += shY * points[i].x;
	}
}

void reflectionAboutXAxis() {
	for (int i = 0; i < verticesCount; i++) {
		points[i].y *= -1;
	}
}

void reflectionAboutYAxis() {
	for (int i = 0; i < verticesCount; i++) {
		points[i].x *= -1;
	}
}


void myMouse(int button, int action, int xMouse, int yMouse) {
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		if (objectDrawn) {
			objectDrawn = false;
			verticesCount = 0;
		}
		points[verticesCount].x = screenXToWindowX(xMouse);
		points[verticesCount++].y = screenYToWindowY(yMouse);
	}
	else if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) {
		objectDrawn = true;
		myDisplay();
		drawPolygon();
	}
}


void myKeyboard(unsigned char key, int xMouse , int yMouse) {
	if (key == 't') {
		int tX, tY;
		cout << "Translation Factor X: ";
		cin >> tX;
		cout << "Translation Factor Y: ";
		cin >> tY;
		
		translation(tX, tY);
		myDisplay();
		drawPolygon();
	}
	else if (key == 's') {
		float sX, sY;
		cout << "Scaling Factor X: ";
		cin >> sX;
		cout << "Scaling Factor Y: ";
		cin >> sY;
		
		scalingAboutFixedPoint(sX, sY);
		myDisplay();
		drawPolygon();
	}
	
	else if (key == 'r') {
		float rotationAngle;
		cout << "Rotation Angle: ";
		cin >> rotationAngle;
		
		rotation(rotationAngle);
		myDisplay();
		drawPolygon();
	}
	
	else if (key == 'x') {
		float shearFactor;
		char shearType;
		cout << "X-Shear or Y-Shear?(Type 'x' or 'y'): ";
		cin >> shearType;
		cout << "Shear Factor: ";
		cin >> shearFactor;
		
		if (shearType == 'x') {
			xShear(shearFactor);
		}
		else if (shearType == 'y') {
			yShear(shearFactor);
		}
		
		myDisplay();
		drawPolygon();
	}
	
	else if (key == 'm') {
		char reflectionType;
		cout << "Reflection about X-axis or Y-axis?(Type 'x' or 'y'): ";
		cin >> reflectionType;
		
		if (reflectionType == 'x') {
			reflectionAboutXAxis();
		}
		else if (reflectionType == 'y') {
			reflectionAboutYAxis();
		}
		
		myDisplay();
		drawPolygon();
	}
}


int main(int argc, char** argv) {

	windowWidth = atoi(argv[1]);
	windowHeight = atoi(argv[2]);
	glutInit(&argc, argv);	
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Assignment6");
	
	Init();
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutMainLoop();
}
