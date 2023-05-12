#include <GL/glut.h>
#include <cmath>
#include <stdlib.h>

// Global Variables used in Mouse Procedure.
bool mousePressedBefore = false;
int x, y;

// Global Variables for storing window dimensions.
int windowWidth, windowHeight;


// Utility function to draw a point.
void displayPoint(float x, float y) {
	glBegin(GL_POINTS);
	glColor3f(1.0, 1.0, 0.0);
	glVertex2f(x, y);
	glEnd();
}


// Works for all octants.
void DDALineDrawingAlgo(float x1, float y1, float x2, float y2) {
	float dx = x2 - x1, dy = y2 - y1;
	float steps;
	
	if (abs(dx) > abs(dy)) {
		steps = abs(dx);
	}
	else {
		steps = abs(dy);
	}

	float deltaX = dx/steps, deltaY = dy/steps;
	float x = x1, y = y1;
	displayPoint(x, y);
	
	for (int i = 0; i < steps; i++) {
		x += deltaX;
		y += deltaY;
		displayPoint(x, y);
	}
	displayPoint(x2, y2);
}


// Works only for first octant.
void BresenhamLineDrawingAlgo(float x1, float y1, float x2, float y2) {
	float deltaX = x2 - x1, deltaY = y2 - y1;
	float pk = (2 * deltaY) - deltaX;
	float x = x1, y = y1;
	displayPoint(x, y);
	
	for (int i = 0; i < deltaX; i++) {
		if (pk < 0) {
			pk += 2 * deltaY;
			x++;
		}
		else {
			pk += 2 * (deltaY - deltaX);
			x++;
			y++;
		}
		displayPoint(x, y);
	}
}

// Utility Functions converting mouse coordinates to window coordinates.
float mouseXToWindowX(float x) {
	return x - windowWidth/2;
}

float mouseYToWindowY(float y) {
	return windowHeight/2 - y;
}


// Mouse procedure for drawing a line by specifying endpoints through left mouse click.
void mouseFunc(GLint button, GLint action, GLint xMouse, GLint yMouse) {

	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		if (mousePressedBefore == false) {
			x = xMouse;
			y = yMouse;
			mousePressedBefore = true;
		}
		else {
			DDALineDrawingAlgo(mouseXToWindowX(x),mouseYToWindowY(y),
			mouseXToWindowX(xMouse), mouseYToWindowY(yMouse));
			mousePressedBefore = false;
		}
	}
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


void mydisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	drawCoordinateAxes();
	glFlush();
}


int main(int argc, char** argv) {
	
	windowWidth = atoi(argv[1]);
	windowHeight = atoi(argv[2]);
	
	glutInit(&argc, argv);	
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Assignment2");
	
	Init();
	glutDisplayFunc(mydisplay);
	glutMouseFunc(mouseFunc);
	glutMainLoop();
}
