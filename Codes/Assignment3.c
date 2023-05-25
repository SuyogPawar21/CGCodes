#include <GL/glut.h>
#include <cmath>
#include <unistd.h>


// Global Variables used in Mouse Procedure
bool mousePressedBefore = false;
int x, y;


// Global Variables for storing window dimensions.
int windowWidth, windowHeight;


// Utility function to draw a point
void drawPoint(double x, double y) {
	glPointSize(1);
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}


void drawSolidPoint(int x, int y) {
	glColor3f(1.0, 1.0, 0.0);
	glPointSize(3);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}


// Utility Function used in Bresenham Circle Drawing Algorithm.
// 'Cx' is centre's x-coordinate and 'Cy' is centre's y-coordinate.
void drawPointInAllQuadrants(double x, double y, double Cx, double Cy) {

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


// Works for any centre and radius.
void BresenhamCircleDrawingAlgo(double Cx, double Cy, double radius) {
	
	glColor3f(1.0, 0.0, 0.0);
	
	double pk = 3 - (2 * radius);
	double x = 0, y = radius;
	
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
		usleep(1500);
		drawPointInAllQuadrants(x, y, Cx, Cy);
	}
	
}


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
		usleep(500);
		drawPoint(x, y);
	}
}


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
		usleep(500);
		drawPoint(x, y);		
	}
}



void BresenhamLineDrawingAlgo(int x1, int y1, int x2, int y2) {
	
	glColor3f(1.0, 1.0, 1.0);
	
	int deltaX = abs(x2 - x1), deltaY = abs(y2 - y1);
	
	if (deltaX > deltaY) {
		drawGentleSlopeLine(x1, y1, x2, y2);
	}
	else {
		drawSharpSlopeLine(x1, y1, x2, y2);
	}
	
}


void drawCoordinateAxes() {
	BresenhamLineDrawingAlgo(0, windowHeight/2, 0, -windowHeight/2);
	BresenhamLineDrawingAlgo(windowWidth/2, 0, -windowWidth/2, 0);
	glFlush();
}	


void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawCoordinateAxes();
	glFlush();
}


void Init() {
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-windowWidth/2, windowWidth/2, -windowHeight/2, windowHeight/2);
}


// Utility Functions converting mouse coordinates to window coordinates.
double mouseXToWindowX(double x) {
	return x - windowWidth/2;
}

double mouseYToWindowY(double y) {
	return windowHeight/2 - y;
}


// Utility function for finding distance between two points.
double distBetween2Points(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


// Mouse procedure for drawing a circle by specifying the centre through the first mouse
// click and radius through the second mouse click.
void mouseFunc(int button, int action, int xMouse, int yMouse) {

	// Whenever the left mouse button is pressed do the following.
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		
		// Save the coordinates for centre through the first mouse click.
		if (mousePressedBefore == false) {
			x = xMouse;
			y = yMouse;
			mousePressedBefore = true;
		}
		else {
			// Calculate radius as distance between first and second mouse click point.
			double radius = distBetween2Points(xMouse, yMouse, x , y);
			// Draw Central Point
			drawSolidPoint(mouseXToWindowX(x), mouseYToWindowY(y));
			// Draw the circle.
			BresenhamCircleDrawingAlgo(mouseXToWindowX(x), mouseYToWindowY(y), radius);
			// Reset flag.
			mousePressedBefore = false;
		}
		
		glFlush();
	}
	
}


int main(int argc, char** argv) {	

	windowWidth = 800;
	windowHeight = 800;
		
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Assignment3");
	
	Init();
	glutDisplayFunc(myDisplay);
	glutMouseFunc(mouseFunc);
	
	glutMainLoop();
	
}
