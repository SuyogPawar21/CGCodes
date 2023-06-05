#include <GL/glut.h>
#include <iostream>

using namespace std;


struct Point {
	int x, y;
};


int windowWidth, windowHeight;
Point clipWindowMax, clipWindowMin;


// Utility function to draw a point.
void drawPoint(int x, int y) {
	glPointSize(1);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
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
		drawPoint(x, y);	
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
	  drawPoint(x, y);
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


void Init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(0, windowWidth, 0, windowHeight);
}


void drawClippingWindow() {
	
	glColor3f(0.0, 0.0, 1.0);
	BresenhamLineDrawingAlgo(clipWindowMin.x, clipWindowMin.y, clipWindowMax.x, clipWindowMin.y);
	BresenhamLineDrawingAlgo(clipWindowMax.x, clipWindowMin.y, clipWindowMax.x, clipWindowMax.y);
	BresenhamLineDrawingAlgo(clipWindowMax.x, clipWindowMax.y, clipWindowMin.x, clipWindowMax.y);
	BresenhamLineDrawingAlgo(clipWindowMin.x, clipWindowMax.y, clipWindowMin.x, clipWindowMin.y);
	
}


void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawClippingWindow();
	glFlush();
}


double mouseYToWindowY(double y) {
	return windowHeight - y;
}


double x1, y1, x2, y2;
bool mousePressedBefore = false;

void myMouse(int button, int action, int xMouse, int yMouse) {

	//Whenever the left mouse button is the pressed do the following.
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		
		// Save the coordinates for the first mouse click.
		if (mousePressedBefore == false) {
			x1 = xMouse;
			y1 = mouseYToWindowY(yMouse);
			mousePressedBefore = true;
		}
		// Draw a line from the previous saved point to the point of second mouse click.
		else {
			x2 = xMouse;
			y2 = mouseYToWindowY(yMouse);
			glColor3f(0.0, 1.0, 0.0);
			BresenhamLineDrawingAlgo(x1, y1, x2, y2);
			mousePressedBefore = false;
		}
	}
}


const int leftOutCode = 1, rightOutCode = 2, bottomOutCode = 4, topOutCode = 8;


int getOutCode(int x, int y) {

	int outCode = 0;	
	
	if (x < clipWindowMin.x) {
		outCode = outCode | leftOutCode;
	}
	if (x > clipWindowMax.x) {
		outCode = outCode | rightOutCode;
	}
	if (y < clipWindowMin.y) {
		outCode = outCode | bottomOutCode;
	}
	if (y > clipWindowMax.y) {
		outCode = outCode | topOutCode;
	}
	
	return outCode;
}


void Clip() {

	bool done = false;
	bool shouldPlotLine = false;
	
	while (!done) {
		
		int c1 = getOutCode(x1, y1);
		int c2 = getOutCode(x2, y2);
		
		if ((c1 | c2) == 0) {
			done = true;
			shouldPlotLine = true;
		}
		else if ((c1 & c2) != 0) {
			done = true;
			shouldPlotLine = false;
		}
		else {
			if (c1 == 0) {
				swap(c1, c2);
				swap(x1, x2);
				swap(y1, y2);
			}
			
			double slope = (double) (y2 - y1) / (x2 - x1);
				
			if (c1 & leftOutCode) {
				x1 = clipWindowMin.x;
				y1 += (clipWindowMin.x - x1) * slope;
			}
			else if (c1 & rightOutCode) {
				x1 = clipWindowMax.x;
				y1 += (clipWindowMax.x - x1) * slope;
			}
			else if (c1 & bottomOutCode) {
				y1 = clipWindowMin.y;
				x1 += (clipWindowMin.y - y1) / slope;
			}
			else if (c1 & topOutCode) {
				y1 = clipWindowMax.y;
				x1 += (clipWindowMax.y - y1) / slope;
			}
		}
	}
	
	myDisplay();
	
	if (shouldPlotLine) {
		glColor3f(1.0, 1.0, 0.0);
		BresenhamLineDrawingAlgo(x1, y1, x2 , y2);
	}
}


void myKeyboard(unsigned char key, int xMouse, int yMouse) {
	if (key == 'c') {
		Clip();
	}
}


int main(int argc, char** argv) {
	
	windowWidth = windowHeight = 600;
	clipWindowMin = {200, 200};
	clipWindowMax = {400, 400};

	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Assignment8");
	
	Init();
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutMainLoop();
}
