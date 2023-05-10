#include <GL/glut.h>


// Struct to hold r, g, b values together
struct Color {
	float r,g,b;
};


// Class to hold x and y coordinates of a point together
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

// Global Variables in used in mouse procedure.
int verticesCount = 0;
bool objectDrawn = false;


// Array of points which will store the vetices of the polygon.
Point points[20];


// Converting mouse Y to window Y
float mouseYToWindowY(float y) {
	return 800 - y;
}


// Variables for defining fill color, boundary color and interior color of the polygon
Color fillColor, boundaryColor, interiorColor;


// Utility Function for drawing polygon with vertices as stored in the 'points' array. 
void drawPolygon() {
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


void Init() {
		glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 0, 800);
}


void displayPoint(float x, float y) {
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}


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


void boundaryFill4(int x, int y) {
	Color pixelColor;
	glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, &pixelColor);
	if ((pixelColor.r != boundaryColor.r || pixelColor.g != boundaryColor.g || pixelColor.b != boundaryColor.b)
	&& (pixelColor.r!=fillColor.r||pixelColor.g!=fillColor.g||pixelColor.b!=fillColor.b)) {
	
		glBegin(GL_POINTS);
		glColor3f(fillColor.r, fillColor.g, fillColor.b);
		glVertex2i(x, y);
		glEnd();
		glFlush();
		boundaryFill4(x - 1, y);
		boundaryFill4(x + 1, y);
		boundaryFill4(x, y - 1);
		boundaryFill4(x, y + 1);
	}
	glFlush();
}


void floodFill4(int x, int y) {
	Color pixelColor;
	glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, &pixelColor);
	if (pixelColor.r == interiorColor.r && pixelColor.g == interiorColor.g && pixelColor.b == interiorColor.b) {
	
		glBegin(GL_POINTS);
		glColor3f(fillColor.r, fillColor.g, fillColor.b);
		glVertex2i(x, y);
		glEnd();
		glFlush();
		floodFill4(x - 1, y);
		floodFill4(x + 1, y);
		floodFill4(x, y - 1);
		floodFill4(x, y + 1);
	}
	glFlush();
}

void myDisplay() {
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0.0, 0.0, 0.0);
    boundaryColor.r=0.0f;
    boundaryColor.g=0.0f;
    boundaryColor.b=0.0f;
    fillColor.r=1.0f;
    fillColor.g=1.0f;
    fillColor.b=0.0f;
    interiorColor.r = 1.0f;
    interiorColor.g = 1.0f;
    interiorColor.b = 1.0f;
    glFlush();
}


void myMouse(int button, int action, int xMouse, int yMouse) {
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		if (objectDrawn) {
			objectDrawn = false;
			verticesCount = 0;
		}
		points[verticesCount].x = xMouse;
		points[verticesCount++].y = mouseYToWindowY(yMouse);
	}
	else if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) {
		objectDrawn = true;
		myDisplay();
		drawPolygon();
	}
}


void myKeyboard(unsigned char key, int xMouse, int yMouse) {
	if (key == 'b') {
		boundaryFill4(xMouse, mouseYToWindowY(yMouse));
	}
	else if (key == 'f') {
		floodFill4(xMouse, mouseYToWindowY(yMouse));
	}
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Assignment4");
    
    Init();
    glutDisplayFunc(myDisplay);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);
    glutMainLoop();    
}
