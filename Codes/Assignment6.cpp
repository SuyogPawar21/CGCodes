#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>

#define RADIAN (M_PI / 180)

using namespace std;

int windowWidth, windowHeight;


int verticesCount = 0;
bool objectDrawn = false;

vector<vector<double>> vertices(20, vector<double>(3));

double screenXToWindowX(double x) {
	return x - windowWidth/2;
}

double screenYToWindowY(double y) {
	return windowHeight/2 - y;
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


void drawPolygon() {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < verticesCount; i++) {
		glVertex2f(vertices[i][0], vertices[i][1]);
	}
	glEnd();
	glFlush();
}


// A = A.B
void matrixMul(vector<vector<double>>& a, vector<vector<double>>& b, int numRowsA) {
	
	vector<vector<double>> result(20, vector<double>(3));
	
	for (int i = 0; i < numRowsA; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j];
		}
	}
	
	for (int i = 0; i < numRowsA; i++) {
		for (int j = 0; j < 3; j++) {
			a[i][j] = result[i][j];
		}
	}
	
}


void translation(double tX, double tY, int numRows) {
	vector<vector<double>> translationMatrix = {{1, 0, 0}, 
															  							{0, 1, 0},
															  							{tX, tY, 1}};
															
	matrixMul(vertices, translationMatrix, numRows);
}


void scalingAboutFixedPoint(double sX, double sY, int xF, int yF, int numRows) {
	vector<vector<double>> scalingMatrix = {{sX, 0, 0}, 
															  					{0, sY, 0},
															  					{xF * (1-sX), yF * (1-sY), 1}};
															
	matrixMul(vertices, scalingMatrix, numRows);
}


void rotationAboutOrigin(double rotationAngle, int numRows) {
	vector<vector<double>> rotationMatrix = {{cos(rotationAngle), sin(rotationAngle), 0}, 
															  					 {-sin(rotationAngle), cos(rotationAngle), 0},
															  					 {0, 0, 1}};
															
	matrixMul(vertices, rotationMatrix, numRows);
}


void reflectionAboutXAxis() {
	vector<vector<double>> reflectionMatrix = {{1, 0, 0}, 
															  						{0, -1, 0},
															  						{0, 0, 1}};
															
	matrixMul(vertices, reflectionMatrix, verticesCount);
}


void reflectionAboutYAxis() {
	vector<vector<double>> reflectionMatrix = {{-1, 0, 0}, 
															  						{0, 1, 0},
															  						{0, 0, 1}};
															
	matrixMul(vertices, reflectionMatrix, verticesCount);
}


void myMouse(int button, int action, int xMouse, int yMouse) {
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		if (objectDrawn == true) {
			objectDrawn = false;
			verticesCount = 0;
		}
		vertices[verticesCount][0] = screenXToWindowX(xMouse);
		vertices[verticesCount][1] = screenYToWindowY(yMouse);
		vertices[verticesCount++][2] = 1;
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
		double tX, tY;
		cout << "Translation Factor X: ";
		cin >> tX;
		cout << "Translation Factor Y: ";
		cin >> tY;
		
		translation(tX, tY, verticesCount);
		drawPolygon();
	}
	
	if (key == 's') {
		double sX, sY;
		cout << "Scaling Factor X: ";
		cin >> sX;
		cout << "Scaling Factor Y: ";
		cin >> sY;
		
		scalingAboutFixedPoint(sX, sY, vertices[0][0], vertices[0][1], verticesCount);
		drawPolygon();
	}
	
	if (key == 'r') {
		double rotationAngle;
		cout << "Rotation Angle: ";
		cin >> rotationAngle;
		rotationAngle *= RADIAN;
		
		rotationAboutOrigin(rotationAngle, verticesCount);
		drawPolygon();
	}
	
	if (key == 'm') {
		char axis;
		cout << "Enter Axis of Reflection('x' or 'y'): ";
		cin >> axis;
		
		if (axis == 'x') {
			reflectionAboutXAxis();
		}
		else if (axis == 'y') {
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

