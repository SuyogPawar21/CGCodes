#include <GL/glut.h>
#include <cmath>
#include <stdlib.h>

#define ERRORTHRESHOLD 0.000001


// Struct to hold r, g, b values together.
struct Color {
	float r, g, b;
};


// Struct to hold x and y coordinates of a point together.
struct Point {
	float x, y;	
};


// Utility function to check for equality of two floating point values.
bool areValuesEqual(float a, float b) {
	return abs(a - b) < ERRORTHRESHOLD;
}

// Utility function to check for equality of two Colors.
bool areColorsEqual(Color c1, Color c2) {
	return areValuesEqual(c1.r, c2.r) && areValuesEqual(c1.g, c2.g) && areValuesEqual(c1.b, c2.b);
}


// Global Variables in used in mouse procedure.
int verticesCount = 0;
bool objectDrawn = false;

// Global Variables for storing window dimensions.
int windowWidth, windowHeight;

// Array of points which will store the vertices of the polygon.
Point points[20];

// Variables for storing fill color, boundary color and interior color of the polygon.
Color fillColor, boundaryColor, interiorColor;


// Converting mouse Y to window Y.
float mouseYToWindowY(float y) {
	return windowHeight - y;
}


// Utility Function for drawing polygon with vertices as stored in the 'points' array. 
void drawPolygon() {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < verticesCount; i++) {
		glVertex2i(points[i].x, points[i].y);
	}
	glEnd();
	glFlush();
}


void Init() {
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0, windowWidth, 0, windowHeight);
}


// Utility function to draw a colored point.
void drawColoredPoint(float x, float y, Color color) {
	glBegin(GL_POINTS);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(x, y);
	glEnd();
}


// Boundary Fill Algorithm (4 Point Version)
void boundaryFill4(int x, int y) {
	
	// Getting the color for the pixel at (x, y) position and storing that in 'pixelColor'.
	Color pixelColor;
	glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, &pixelColor);
	
	// If the pixel at (x, y) is not a boundary pixel and is not already filled then fill it.
	if (!areColorsEqual(pixelColor, boundaryColor) && !areColorsEqual(pixelColor, fillColor)) {
	
		drawColoredPoint(x, y, fillColor);
		boundaryFill4(x - 1, y);
		boundaryFill4(x + 1, y);
		boundaryFill4(x, y - 1);
		boundaryFill4(x, y + 1);
		
	}
	glFlush();
	
}


// Flood Fill Algorithm (4 Point Version)
void floodFill4(int x, int y) {
	
	// Getting the color for the pixel at (x, y) position and storing that in 'pixelColor'.
	Color pixelColor;
	glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, &pixelColor);
	
	// If the pixel at (x, y) has the default interior color then fill it.
	if (areColorsEqual(pixelColor, interiorColor)) {
	
		drawColoredPoint(x, y, fillColor);
		floodFill4(x - 1, y);
		floodFill4(x + 1, y);
		floodFill4(x, y - 1);
		floodFill4(x, y + 1);
		
	}
	glFlush();
}


void initializeColors() {
	// Black
	boundaryColor.r = 0.0f;
  boundaryColor.g = 0.0f;
  boundaryColor.b = 0.0f;
  
  // Red
  fillColor.r = 1.0f;
  fillColor.g = 0.0f;
  fillColor.b = 0.0f;
  
  // White
  interiorColor.r = 1.0f;
  interiorColor.g = 1.0f;
  interiorColor.b = 1.0f;
}


void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
  glFlush();
}


// Mouse procedure to draw a polygon by specifying points using left click.
void myMouse(int button, int action, int xMouse, int yMouse) {
	
	// Whenever the left mouse button is pressed do the following.
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		
		// If the object is already drawn and reset the state of the 'points' array.
		if (objectDrawn) {
			objectDrawn = false;
			verticesCount = 0;
		}
		// Add point to 'points' array
		points[verticesCount].x = xMouse;
		points[verticesCount++].y = mouseYToWindowY(yMouse);
		
	}
	// Whenever the right mouse button is pressed draw the polygon
	// using points in 'points' array.
	else if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) {
		objectDrawn = true;
		myDisplay();
		drawPolygon();
	}
	
}


// Keyboard procedure for specifying fill algorithm at specified point.
void myKeyboard(unsigned char key, int xMouse, int yMouse) {
	if (key == 'b') {
		boundaryFill4(xMouse, mouseYToWindowY(yMouse));
	}
	else if (key == 'f') {
		floodFill4(xMouse, mouseYToWindowY(yMouse));
	}
}


int main(int argc, char** argv) {
		
	initializeColors();
		
	windowWidth = 800;
	windowHeight = 800;
		
  glutInit(&argc, argv);
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Assignment4");
    
  Init();
  glutDisplayFunc(myDisplay);
  glutMouseFunc(myMouse);
  glutKeyboardFunc(myKeyboard);
  glutMainLoop();    
}
