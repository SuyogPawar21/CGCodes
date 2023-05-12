#include <GL/glut.h>
#include <cmath>
#include <stdlib.h>


// Global Variables used in Mouse Procedure.
bool mousePressedBefore = false;
int x, y;


// Global Variables for storing window dimensions.
int windowWidth, windowHeight;


// Utility function to draw a point.
void displayPoint(double x, double y) {
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}


// Works for all octants.
void DDALineDrawingAlgo(double x1, double y1, double x2, double y2) {

	double dx = x2 - x1, dy = y2 - y1;
	double steps;
	
	if (abs(dx) > abs(dy)) {
		steps = abs(dx);
	}
	else {
		steps = abs(dy);
	}

	double deltaX = dx/steps, deltaY = dy/steps;
	double x = x1, y = y1;
	displayPoint(x, y);
	
	for (int i = 0; i < steps; i++) {
		x += deltaX;
		y += deltaY;
		displayPoint(x, y);
	}
	
	displayPoint(x2, y2);
	
}


// Works only for first octant.
void BresenhamLineDrawingAlgo(double x1, double y1, double x2, double y2) {

	double deltaX = x2 - x1, deltaY = y2 - y1;
	double pk = (2 * deltaY) - deltaX;
	double x = x1, y = y1;
	
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
double mouseXToWindowX(double x) {
	return x - windowWidth/2;
}

double mouseYToWindowY(double y) {
	return windowHeight/2 - y;
}


// Mouse procedure for drawing a line by specifying endpoints through left mouse click.
void mouseFunc(int button, int action, int xMouse, int yMouse) {
	
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
			DDALineDrawingAlgo(mouseXToWindowX(x),mouseYToWindowY(y),
			mouseXToWindowX(xMouse), mouseYToWindowY(yMouse));
			mousePressedBefore = false;
		}
		
	}
	
	glFlush();
	
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
	
	 
	//Taking window dimensions as command line arguments.
	//For example - ./Assignment2.exe 1000 800
	//Here windowWidth = 1000 and winodwHeight = 800.
	
	windowWidth = atoi(argv[1]);
	windowHeight = atoi(argv[2]);
	
	glutInit(&argc, argv);	
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Assignment2");
	
	Init();
	glutDisplayFunc(myDisplay);
	glutMouseFunc(mouseFunc);
	glutMainLoop();
}
