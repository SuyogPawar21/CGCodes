#include<iostream>
#include<GL/glut.h>
#include<math.h>

using namespace std;

struct Point {
	float x;
	float y;
};

bool objectDrawn = false;
int verticesCount, clipVerticesCount;
int i;

Point clipWindowMin, clipWindowMax, points[20], clipPoints[20];

float mouseYToWindowY(float y) {
	return 800 - y;
}

void leftClipper() {

	clipVerticesCount = 0;
	
	for (i = 0; i < verticesCount; i++) {
	
		//Case-1:	outside to inside
		if (points[i].x < clipWindowMin.x && points[i + 1].x >= clipWindowMin.x)	{
		
			if (points[i + 1].x - points[i].x != 0) {
				clipPoints[clipVerticesCount].y = (points[i+1].y-points[i].y)/(points[i+1].x-points[i].x)*(clipWindowMin.x-points[i].x)+points[i].y;		
			}
			else {
				clipPoints[clipVerticesCount].y=points[i].y;
			}
			clipPoints[clipVerticesCount].x=clipWindowMin.x;
			clipVerticesCount++;
			clipPoints[clipVerticesCount].x=points[i+1].x;				
			clipPoints[clipVerticesCount].y=points[i+1].y;
			clipVerticesCount++;
				
		}
		//Case-2:	inside to inside
		if(points[i].x>=clipWindowMin.x && points[i+1].x>=clipWindowMin.x)	{
			clipPoints[clipVerticesCount].y=points[i+1].y;				 
			clipPoints[clipVerticesCount].x=points[i+1].x;
			clipVerticesCount++;
		}
		
		// Case-3:	inside to outside
		if(points[i].x>=clipWindowMin.x && points[i+1].x<clipWindowMin.x)	{
			if(points[i+1].x-points[i].x!=0) {
				clipPoints[clipVerticesCount].y=(points[i+1].y-points[i].y)/(points[i+1].x-points[i].x)*(clipWindowMin.x-points[i].x)+points[i].y;		
			}
			else {
				clipPoints[clipVerticesCount].y=points[i].y;
			}
			clipPoints[clipVerticesCount].x=clipWindowMin.x;
			clipVerticesCount++;
		}
	}
	
	for (i = 0; i < clipVerticesCount; i++) {
		points[i].x=clipPoints[i].x;
		points[i].y=clipPoints[i].y;
	}
	
	points[i].x=clipPoints[0].x;
	points[i].y=clipPoints[0].y;
	verticesCount=clipVerticesCount;
}



void rightClipper()	{
	i=0;
	clipVerticesCount=0;
	
	for (i = 0; i < verticesCount; i++) {
		//Case-1:	outside to inside
		if(points[i].x > clipWindowMax.x && points[i+1].x <= clipWindowMax.x)	{
			if(points[i+1].x - points[i].x != 0) {
				clipPoints[clipVerticesCount].y=(points[i+1].y-points[i].y)/(points[i+1].x-points[i].x) * (clipWindowMax.x-points[i].x) + points[i].y;			
			}
			else {
				clipPoints[clipVerticesCount].y=points[i].y;
			}
			clipPoints[clipVerticesCount].x=clipWindowMax.x;
			clipVerticesCount++;
			clipPoints[clipVerticesCount].x=points[i+1].x;						
			clipPoints[clipVerticesCount].y=points[i+1].y;
			clipVerticesCount++;
		}
		// Case-2:	inside to inside
		if(points[i].x<=clipWindowMax.x && points[i+1].x<=clipWindowMax.x)		
		{
			clipPoints[clipVerticesCount].y=points[i+1].y;				 
			clipPoints[clipVerticesCount].x=points[i+1].x;
			clipVerticesCount++;
		}
		// Case-3:	inside to outside
		if(points[i].x<=clipWindowMax.x && points[i+1].x>clipWindowMax.x)		
		{
			if(points[i+1].x-points[i].x!=0)
			{
				clipPoints[clipVerticesCount].y=(points[i+1].y-points[i].y)/(points[i+1].x-points[i].x)*(clipWindowMax.x-points[i].x)+points[i].y;			
			}
			else
			{
				clipPoints[clipVerticesCount].y=points[i].y;
			}
			clipPoints[clipVerticesCount].x=clipWindowMax.x;
			clipVerticesCount++;
		}
		
	}
	
	for(i=0;i<clipVerticesCount;i++)
	{
		points[i].x=clipPoints[i].x;
		points[i].y=clipPoints[i].y;
	}
	
	points[i].x=clipPoints[0].x;
	points[i].y=clipPoints[0].y;
	verticesCount=clipVerticesCount;
	
}


void topClipper()	{
	i=0;clipVerticesCount=0;
	
	for(i=0;i<verticesCount;i++) {
		//Case-1:	outside to inside
		if(points[i].y>clipWindowMax.y && points[i+1].y<=clipWindowMax.y)	{
			if(points[i+1].y-points[i].y!=0) {
				clipPoints[clipVerticesCount].x=(points[i+1].x-points[i].x)/(points[i+1].y-points[i].y)*(clipWindowMax.y-points[i].y)+points[i].x;				
			}
			else {
				clipPoints[clipVerticesCount].x=points[i].x;
			}
			clipPoints[clipVerticesCount].y=clipWindowMax.y;
			clipVerticesCount++;
			clipPoints[clipVerticesCount].x=points[i+1].x;					
			clipPoints[clipVerticesCount].y=points[i+1].y;
			clipVerticesCount++;
		}
		// Case-2:	inside to inside
		if(points[i].y<=clipWindowMax.y && points[i+1].y<=clipWindowMax.y) {
			clipPoints[clipVerticesCount].y=points[i+1].y;					
			clipPoints[clipVerticesCount].x=points[i+1].x;
			clipVerticesCount++;
		}
		// Case-3:	inside to outside
		if(points[i].y<=clipWindowMax.y && points[i+1].y>clipWindowMax.y)	{
			if(points[i+1].y-points[i].y!=0) {
				clipPoints[clipVerticesCount].x=(points[i+1].x-points[i].x)/(points[i+1].y-points[i].y)*(clipWindowMax.y-points[i].y)+points[i].x;			
			}
			else {
				clipPoints[clipVerticesCount].x=points[i].x;
			}
			clipPoints[clipVerticesCount].y=clipWindowMax.y;
			clipVerticesCount++;
		}
	}
	
	for(i=0; i<clipVerticesCount; i++) {
		points[i].x=clipPoints[i].x;
		points[i].y=clipPoints[i].y;
	}
	
	points[i].x=clipPoints[0].x;
	points[i].y=clipPoints[0].y;
	verticesCount=clipVerticesCount;
}



void bottomClipper() {
	i=0;clipVerticesCount=0;
	
	for (i=0;i<verticesCount;i++) {
		// Case-1:	outside to inside
		if (points[i].y<clipWindowMin.y && points[i+1].y>=clipWindowMin.y)	{
			if (points[i+1].y-points[i].y!=0) {
				clipPoints[clipVerticesCount].x=(points[i+1].x-points[i].x)/(points[i+1].y-points[i].y)*(clipWindowMin.y-points[i].y)+points[i].x;				
			}
			else {
				clipPoints[clipVerticesCount].x=points[i].x;
			}
			clipPoints[clipVerticesCount].y=clipWindowMin.y;
			clipVerticesCount++;
			clipPoints[clipVerticesCount].x=points[i+1].x;						
			clipPoints[clipVerticesCount].y=points[i+1].y;
			clipVerticesCount++;
		}
		// Case-2:	inside to inside
		if (points[i].y>=clipWindowMin.y && points[i+1].y>=clipWindowMin.y)		
		{
			clipPoints[clipVerticesCount].x=points[i+1].x;					
			clipPoints[clipVerticesCount].y=points[i+1].y;
			clipVerticesCount++;
		}
		// Case-3:	inside to outside
		if(points[i].y>=clipWindowMin.y && points[i+1].y<clipWindowMin.y)	{
			if(points[i+1].y-points[i].y!=0) {
				clipPoints[clipVerticesCount].x=(points[i+1].x-points[i].x)/(points[i+1].y-points[i].y)*(clipWindowMin.y-points[i].y)+points[i].x;			
			}
			else {
				clipPoints[clipVerticesCount].x=points[i].x;
			}
			clipPoints[clipVerticesCount].y=clipWindowMin.y;
			clipVerticesCount++;
		}
	}
	
	for(i = 0;i < clipVerticesCount; i++) {
		points[i].x=clipPoints[i].x; 
		points[i].y=clipPoints[i].y;
	}
	points[i].x=clipPoints[0].x;
	points[i].y=clipPoints[0].y;
	verticesCount=clipVerticesCount;
}


void drawpolygon() {
	glColor3f(1.0,0.0,0.0);
	for(i = 0; i < verticesCount; i++) {
		glBegin(GL_LINES);
		glVertex2d(points[i].x, points[i].y);
		glVertex2d(points[i+1].x, points[i+1].y);
		glEnd();
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(clipWindowMin.x, clipWindowMin.y);
	glVertex2f(clipWindowMax.x,clipWindowMin.y);
	glVertex2f(clipWindowMax.x,clipWindowMax.y);
	glVertex2f(clipWindowMin.x,clipWindowMax.y);
	glEnd();
	drawpolygon();
	glFlush();
}


void myMouse(int button, int action, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		if (objectDrawn) {
			objectDrawn = false;
			verticesCount = 0;
		}
		points[verticesCount].x = x;
		points[verticesCount++].y = mouseYToWindowY(y);
	}
	else if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) {
		objectDrawn = true;
		points[verticesCount].x = points[0].x;					
		points[verticesCount].y = points[0].y;
		display();
		drawpolygon();
	}
}


void myKeyboard(unsigned char key, int xMouse, int yMouse) {
	if (key == 'l') {
		leftClipper();
	}
	if (key == 'r') {
		rightClipper();
	}
	if (key == 't') {
		topClipper();
	}
	if (key == 'b') {
		bottomClipper();
	}
	display();
	glFlush();
}



void Init(void) {
	glClearColor(0.0,0.0,0.0,0.0);			
	gluOrtho2D(0,800,0,800);
}



int main(int argc, char** argv) {
	
	clipWindowMin.x = 300;
	clipWindowMin.y = 300;
	clipWindowMax.x = 600;
	clipWindowMax.y = 600;
	
	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Assignment5");
	Init();
	
	glutDisplayFunc(display);
	glutMouseFunc(myMouse);		
	glutKeyboardFunc(myKeyboard);		
	glFlush();
	glutMainLoop();
	return 0;
}
