#include <GL/glut.h>
#include <cmath>
#include <iostream>

#define RADIAN (M_PI / 180)

using namespace std;


struct Point {
		double x , y;
};


int windowWidth, windowHeight;


int verticesCount = 0;
bool objectDrawn = false;

Point points[20];

double screenXToWindowX(double x) {
	return x - windowWidth/2;
}

double screenYToWindowY(double y) {
	return windowHeight/2 - y;
}


void drawPolygon() {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < verticesCount; i++) {
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
	glFlush();
}


void drawCoordinateAxes() {
	
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


void scaling(double sX, double sY) {
	
	for (int i = 0; i < verticesCount; i++) {
		points[i].x *= sX;
		points[i].y *= sY;
	}
	
}

void scalingAboutFixedPoint(double sX, double sY) {
	
	Point fixedPoint;
	fixedPoint.x = points[0].x;
	fixedPoint.y =  points[0].y;
	
	for (int i = 0; i < verticesCount; i++) {
		points[i].x = (points[i].x * sX) + (fixedPoint.x * (1 - sX));
		points[i].y = (points[i].y * sY) + (fixedPoint.y * (1 - sY));
	}
	
}


double calDistFromOrigin(double x, double y) {
	return sqrt(pow(x, 2) + pow(y, 2));
}

double calAngleFromOrigin(double x, double y) {
	return atan(y / x);
}


void rotation(double rotationAngle) {
	
	double distFromOrigin, angleFromOrigin;
	 
	for (int i = 0; i < verticesCount; i++) {
	
		distFromOrigin = calDistFromOrigin(points[i].x, points[i].y);
		angleFromOrigin = calAngleFromOrigin(points[i].x, points[i].y);
		
		points[i].x = distFromOrigin * cos(rotationAngle + angleFromOrigin);
		points[i].y = distFromOrigin * sin(rotationAngle + angleFromOrigin);
		
	}
	
}

void xShear(double shX) {
	for (int i = 0; i < verticesCount; i++) {
		points[i].x += shX * points[i].y;
	}
}

void yShear(double shY) {
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
		if (objectDrawn == true) {
			objectDrawn = false;
			verticesCount = 0;
		}
		points[verticesCount].x = screenXToWindowX(xMouse);
		points[verticesCount++].y = screenYToWindowY(yMouse);
	}
	else if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) {
		objectDrawn = true;
		myDisplay();
		glColor3f(1.0, 1.0, 0.0);
		drawPolygon();
	}
}


void myKeyboard(unsigned char key, int xMouse , int yMouse) {
	glColor3f(1.0, 0.0, 0.0);
	
	if (key == 't') {
		int tX, tY;
		cout << "Translation Factor X: ";
		cin >> tX;
		cout << "Translation Factor Y: ";
		cin >> tY;
		
		translation(tX, tY);
		drawPolygon();
	}
	else if (key == 's') {
		double sX, sY;
		cout << "Scaling Factor X: ";
		cin >> sX;
		cout << "Scaling Factor Y: ";
		cin >> sY;
		
		scaling(sX, sY);
		drawPolygon();
	}
	
	else if (key == 'r') {
		double rotationAngle;
		cout << "Rotation Angle: ";
		cin >> rotationAngle;
		rotationAngle *= RADIAN;
		rotation(rotationAngle);
		drawPolygon();
	}
	
	else if (key == 'x') {
		double shearFactor;
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
		
		drawPolygon();
	}
}


int main(int argc, char** argv) {

	windowWidth = 800;
	windowHeight = 800;
	
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
