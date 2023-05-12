#include <GL/glut.h>
#include <cmath>
#include <stdlib.h>


// Global Variables used in Mouse Procedure
bool mousePressedBefore = false;
int x, y;


// Global Variables for storing window dimensions.
int windowWidth, windowHeight;


// Utility function to draw a point
void drawPoint(float x, float y) {
	glBegin(GL_POINTS);
		glVertex2f(x, y);
	glEnd();
}


void drawPointInAllQuadrants(float x, float y, float Cx, float Cy) {
	// First Quadrant
	drawPoint(x + Cx, y + Cy);
	drawPoint(y + Cx, x + Cy);
	
	// Second Quadrant
	drawPoint(-x + Cx, y + Cy);
	drawPoint(-y + Cx, x + Cy);
	
	// Third Quadrant
	drawPoint(-x + Cx, -y + Cy);
	drawPoint(-y + Cx, -x + Cy);
	
	// Fourth Quadrant
	drawPoint(x + Cx, -y + Cy);
	drawPoint(y + Cx, -x + Cy);
}


void BresenhamCircleDrawingAlgo(float Cx, float Cy, float radius) {
	float pk = 3 - (2 * radius);
	float x = 0, y = radius;
	
	while (x <= y) {
		if (pk < 0) {
			x++;
			pk += (4 * x) + 6;
		}
		else {
			x++;
			y--;
			pk += 4 * (x - y) + 10;
		}
		drawPointInAllQuadrants(x, y, Cx, Cy);
	}
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


void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawCoordinateAxes();
	glColor3f(1.0, 1.0, 0.0);
	glFlush();
}


void Init() {
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-windowWidth/2, windowWidth/2, -windowHeight/2, windowHeight/2);
}


// Utility Functions converting mouse coordinates to window coordinates.
float mouseXToWindowX(float x) {
	return x - windowWidth/2;
}

float mouseYToWindowY(float y) {
	return windowHeight/2 - y;
}


void mouseFunc(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		if (mousePressedBefore == false) {
			x = xMouse;
			y = yMouse;
			mousePressedBefore = true;
		}
		else {
			float radius = sqrt(pow(xMouse - x, 2) + pow(yMouse - y, 2));
			BresenhamCircleDrawingAlgo(mouseXToWindowX(x), mouseYToWindowY(y), radius);
			mousePressedBefore = false;
		}
		glFlush();
	}
}


int main(int argc, char** argv) {	
	windowWidth = atoi(argv[1]);
	windowHeight = atoi(argv[2]);
		
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Assignment3");
	
	Init();
	glutDisplayFunc(myDisplay);
	glutMouseFunc(mouseFunc);
	
	glutMainLoop();
}
