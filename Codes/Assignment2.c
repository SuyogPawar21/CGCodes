#include <GL/glut.h>
#include <cmath>
#include <stdio.h>


int pointsIntervalDotted = 5;
int pointsIntervalDashed = 20;
bool shouldDraw = true;

int drawingMode = 1;

// Global Variables used in Mouse Procedure.
bool mousePressedBefore = false;
int x, y;


// Global Variables for storing window dimensions.
int windowWidth, windowHeight;


// Utility function to draw a point.
void drawPoint(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}


void drawDottedPoint(int x, int y) {
	if (pointsIntervalDotted != 0) {
		pointsIntervalDotted--;
		return;
	}	
	
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	pointsIntervalDotted = 5;

}


void drawDashPoint(int x, int y) {
	pointsIntervalDashed--;
	if (shouldDraw) {
		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();
	}
	if (pointsIntervalDashed == 0) {
		shouldDraw = !shouldDraw;
		pointsIntervalDashed = 20;
	}
	
}


// Works for all octants.
void DDALineDrawingAlgo(int x1, int y1, int x2, int y2) {

	int dx = x2 - x1, dy = y2 - y1;
	int steps;
	
	if (abs(dx) > abs(dy)) {
		steps = abs(dx);
	}
	else {
		steps = abs(dy);
	}

	double deltaX = ((double) dx) / steps, deltaY = ((double) dy) / steps;
	
	double x = x1, y = y1;
	drawPoint(x, y);
	
	for (int i = 0; i < steps; i++) {
		x += deltaX;
		y += deltaY;
		
		switch(drawingMode) {
			case 1:
				drawPoint(round(x), round(y));
				break;
			case 2:
				drawDottedPoint(round(x), round(y));
				break;
			case 3:
				drawDashPoint(round(x), round(y));
				break;
		}
	}
	
	drawPoint(x2, y2);
	
}


// For lines whose |slope| < 1.
void drawGentleSlopeLine(int x1, int y1, int x2, int y2) {
	
	int deltaX = abs(x2 - x1), deltaY = abs(y2 - y1);
	int pk = (2 * deltaY) - deltaX;
	int xIncrement = (x2 > x1) ? 1 : -1;
	int yIncrement = (y2 > y1) ? 1 : -1;
	
	int x = x1 , y = y1;
	drawPoint(x, y);
	
	for (int i = 0; i < deltaX; i++) {
	
		if (pk < 0) {
			pk += 2 * deltaY;
			x += xIncrement;
		}
		else {
			pk += 2 * (deltaY - deltaX);
			x += xIncrement;
			y += yIncrement;
		}
		
		switch(drawingMode) {
			case 1:
				drawPoint(x, y);
				break;
			case 2:
				drawDottedPoint(x, y);
				break;
			case 3:
				drawDashPoint(x, y);
				break;
		}
	}
}

// For lines whose |slope| >= 1.
void drawSharpSlopeLine(int x1, int y1, int x2, int y2) {
	
	int deltaX = abs(x2 - x1), deltaY = abs(y2 - y1);
	int pk = (2 * deltaX) - deltaY;
	int xIncrement = (x2 > x1) ? 1 : -1;
	int yIncrement = (y2 > y1) ? 1 : -1;
	
	int x = x1 , y = y1;
	drawPoint(x, y);
	
	for (int i = 0; i < deltaY; i++) {
	
		if (pk < 0) {
			pk += 2 * deltaX;
			y += yIncrement;
		}
		else {
			pk += 2 * (deltaX - deltaY);
			x += xIncrement;
			y += yIncrement;
		}
		
		switch(drawingMode) {
			case 1:
				drawPoint(x, y);
				break;
			case 2:
				drawDottedPoint(x, y);
				break;
			case 3:
				drawDashPoint(x, y);
				break;
		}
	}
}


// Works for all octants.
void BresenhamLineDrawingAlgo(int x1, int y1, int x2, int y2) {

	int deltaX = abs(x2 - x1), deltaY = abs(y2 - y1);
	
	if (deltaX > deltaY) {
		drawGentleSlopeLine(x1, y1, x2, y2);
	}
	else {
		drawSharpSlopeLine(x1, y1, x2, y2);
	}
	
}


// Utility Functions converting mouse coordinates to window coordinates.
double mouseXToWindowX(double x) {
	return x - windowWidth/2;
}

double mouseYToWindowY(double y) {
	return windowHeight/2 - y;
}


// Mouse procedure for drawing a line by specifying endpoints through left mouse click.
void myMouse(int button, int action, int xMouse, int yMouse) {
	
	//Whenever the left mouse button is the pressed do the following.
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		
		// Save the coordinates for the first mouse click.
		if (mousePressedBefore == false) {
			x = xMouse;
			y = yMouse;
			mousePressedBefore = true;
		}
		// Draw a line from the previous saved point to the point of second mouse click.
		else {
			BresenhamLineDrawingAlgo(mouseXToWindowX(x),mouseYToWindowY(y),
			mouseXToWindowX(xMouse), mouseYToWindowY(yMouse));
			mousePressedBefore = false;
		}
		
	}
	
	glFlush();
	
}

void myKeyboard(unsigned char key, int x, int y) {
	// Simple
	if (key == '1') {
		drawingMode = 1;
	}
	// Dotted
	else if (key == '2') {
		drawingMode = 2;
	}
	// Dashed
	else if (key == '3') {
		drawingMode = 3;
	}
}

// Draws X and Y axis for the window.
void drawCoordinateAxes() {
	
	//White color
	glColor3f(1.0, 1.0, 1.0);
	
	// Y-axis	
	DDALineDrawingAlgo(0, windowHeight/2, 0, -windowHeight/2);
	
	// X-axis
	DDALineDrawingAlgo(windowWidth/2, 0, -windowWidth/2, 0);
	
	glFlush();
}


// Setting up window properties.
void Init() {

	// Single buffer and RGB color model
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	
	// Black Background with maximum opaqueness
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	glMatrixMode(GL_PROJECTION);
	
	// Setting up the coordinate system for the window such that the middle point of the window
	// is the origin. For example, let's say windowWidth is 1000 and windowHeight is 800 then
	// the x-axis of the window will range from -500 to 500 and y-axis will be -400 to 400.
	gluOrtho2D(-windowWidth/2, windowWidth/2, -windowHeight/2, windowHeight/2);
	
}


void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawCoordinateAxes();
	glColor3f(1.0, 0.0, 0.0);
	glFlush();
}


int main(int argc, char** argv) {
	
	windowWidth = 800;
	windowHeight = 800;

	glutInit(&argc, argv);	
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Assignment2");
	
	Init();
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutMainLoop();
}
