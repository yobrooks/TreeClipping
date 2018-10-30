#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

//constants--MUST CHANGE BEFORE TURNING IN

const int WINDOW_MAX = 600;
const int WINDOW_MIN = 0;
const int VIEWPORT_MAX =500;
const int VIEWPORT_MIN = 100;

//use the midpoint circle algorithm to find the points for the circle, store them in a structure, then by trial and error decide on which one to cut off at 

//struct declarations
struct Vertex{
	double x, y, z, w;
};

struct Circle{
	//int numCircPoints=0;
	vector<Vertex> myCircPoints;
};

struct BaseTree{
//	int numBasePoints=0;
	vector<Vertex> myBasePoints;	
};

struct Tree{
	Circle myCircle;
	BaseTree myBaseTree;
};



//const double circleCenterX = VIEWPORT_MAX-100;
//const double circleCenterY = VIEWPORT_MAX-200;
//const double circleRadius = 60;

//global variables
Circle circle;
BaseTree baseTree;
Tree myTree;

void myglutInit(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); //change to double 
	glutInitWindowSize(WINDOW_MAX, WINDOW_MAX);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Tree");

}

void myInit(void)
{
	//set black background
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glPointSize(1.0); //IF I MAKE THIS LARGER WILL NUMBER OF POINTS DECREASE?
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (float) WINDOW_MAX, 0.0, (float) WINDOW_MAX);
	glMatrixMode(GL_MODELVIEW);
	
}

//stores and draws point
void plotPoint(double x, double y)
{
	//store the point
	Vertex vertex;
	vertex.x = x;
	vertex.y = y;
	vertex.z = 0;
	vertex.w = 0;
	
	circle.myCircPoints.push_back(vertex);
//	circle.numCircPoints ++;
	//draw the point
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
		glVertex2d(x, y);
	glEnd();
}

//everything in octant 5 and 6 (with starting octant being 0) needs to not be drawn...
//determine where the point will be in all eight octants
void pointSymmetry(double xCord, double yCord, double x, double y)
{
	plotPoint(xCord + x, yCord + y);
	plotPoint(xCord - x, yCord + y);
	plotPoint(xCord + x, yCord - y);
	plotPoint(xCord - x, yCord - y);
	plotPoint(xCord + y, yCord + x);
	//plotPoint(xCord - y, yCord + x);
	plotPoint(xCord+ y, yCord - x);
	//plotPoint(xCord - y, yCord - x);			
}

//midpoint circle algorithm taken from pgs 150 - 153 of textbook 
//change all to integers and convert to doubles at the end 
void midpointCircle(double xCord, double yCord, double radius)
{
	double x, y;
	x = 0;
	y = radius;

	double p = (5/4) - radius;
	
	plotPoint(x, y);
	pointSymmetry(xCord, yCord, x, y);
	
	
	while(x < y)
	{
		x++;

		if(p < 0)
		{
			p = p + 2*(x +1);
		}
		else
		{
			y--;
			p = p + 2*(x - y) +1;
		}
		pointSymmetry(xCord, yCord, x, y);
	}
}

void drawTreeLines(double x1, double y1, double x2, double y2)
{
	Vertex vert1, vert2;
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
		glVertex2d(x1, y1);	
		glVertex2d(x2, y2);
	glEnd();
	glFlush();

	vert1.x = x1; vert1.y = y1; vert1.z = 0; vert1.w = 0;
	vert2.x = x2; vert2.y = y2; vert2.z = 0; vert2.w = 0;
	
	baseTree.myBasePoints.push_back(vert1);
	baseTree.myBasePoints.push_back(vert2);
//	baseTree.numBasePoints+=2;
	//PUSHBACK TO A VECTOR

}
void drawTree()
{

	//define circle info HERE!!!
	double circleCenterX = VIEWPORT_MAX-100;
	double circleCenterY = VIEWPORT_MAX-200;
	double circleRadius = 60;
	
	midpointCircle(circleCenterX, circleCenterY, circleRadius);
	
	drawTreeLines(circleCenterX+20, circleCenterY+15, circleCenterX-42, circleCenterY+42);	
      
	drawTreeLines(circleCenterX+20, circleCenterY-15, circleCenterX-42, circleCenterY-42);
        
	drawTreeLines(circleCenterX+20, circleCenterY+15, VIEWPORT_MIN+100, VIEWPORT_MAX-175);
        
	drawTreeLines(circleCenterX+20, circleCenterY-15, VIEWPORT_MIN+100, VIEWPORT_MIN+160);

        drawTreeLines(VIEWPORT_MIN+100, VIEWPORT_MAX-175, VIEWPORT_MIN+100, VIEWPORT_MIN+160);

	//ADD CIRCLE AND BASE TO TREE HERE!!!
	myTree.myCircle = circle;
	myTree.myBaseTree = baseTree;
 		
}

void display(void)
{
	//draw viewport window in white
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glRecti(VIEWPORT_MIN, VIEWPORT_MIN, VIEWPORT_MAX, VIEWPORT_MAX); 
	
	drawTree();
}

//build translation matrix
void buildTranslateMatrix(vector<double> translateVec, double x, double y, double z)
{
	translateVec.push_back(1.0);	translateVec.push_back(0.0);	translateVec.push_back(0.0); 	translateVec.push_back(x);
	translateVec.push_back(0.0); 	translateVec.push_back(1.0); 	translateVec.push_back(0.0);	translateVec.push_back(y);
	translateVec.push_back(0.0);	translateVec.push_back(0.0);	translateVec.push_back(1.0);	translateVec.push_back(z);	
	translateVec.push_back(0.0);	translateVec.push_back(0.0);	translateVec.push_back(0.0);	translateVec.push_back(1.0);
}

//build rotation matrix that does a rotation about the z axis; based on angle passed in
void buildRotationMatrix(double theta, vector<double> rotateVec)
{
	double angle = (theta*M_PI)/180.0; //convert angle to radians
	
	rotateVec.push_back(cos(angle));   rotateVec.push_back(-sin(angle));	rotateVec.push_back(0.0);	rotateVec.push_back(0.0);
	rotateVec.push_back(sin(angle));   rotateVec.push_back(cos(angle));	rotateVec.push_back(0.0);	rotateVec.push_back(0.0);
	rotateVec.push_back(0.0);	   rotateVec.push_back(0.0);		rotateVec.push_back(1.0);	rotateVec.push_back(0.0);
	rotateVec.push_back(0.0);	   rotateVec.push_back(0.0);		rotateVec.push_back(0.0);	rotateVec.push_back(1.0); 
}

//build Scale Matrix; based on scale factor passed in
void buildScaleMatrix(double increaseScale, vector<double> scaleVec)
{
	//make scale into a decimal from percentage
	//increaseScale = increaseScale/100;
	
	scaleVec.push_back(increaseScale);	scaleVec.push_back(0.0);	scaleVec.push_back(0.0);	scaleVec.push_back(0.0);
	scaleVec.push_back(0.0);	scaleVec.push_back(increaseScale);	scaleVec.push_back(0.0);	scaleVec.push_back(0.0);	   scaleVec.push_back(0.0);		   scaleVec.push_back(0.0);	 scaleVec.push_back(increaseScale); scaleVec.push_back(0.0);
	scaleVec.push_back(0.0);		scaleVec.push_back(0.0);	scaleVec.push_back(0.0);	scaleVec.push_back(1.0);
}

//mouse function
void mouse(int button, int state, int x, int y)
{
	//if mouse inside of viewport
    y = WINDOW_MAX-y;
    if (x>VIEWPORT_MIN && x<VIEWPORT_MAX && y > VIEWPORT_MIN && y < VIEWPORT_MAX)
    {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//increase speed of rotation
		cout << "INSIDE VIEWPORT LEFT" << endl;
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//slow down rotation
		cout << "INSIDE VIEWPORT RIGHT" << endl;
	}
    }
	//NOT WORKING!!!
	//else if mouse is outside viewport
    else if(x > VIEWPORT_MAX &&  x < WINDOW_MAX && y< VIEWPORT_MIN && y > WINDOW_MIN)
    {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//increase scale of tree
		cout << "OUTSIDE VIEWPORT LEFT" << endl;
	}
	
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//decrease scale of tree
		cout << "OUTSIDE VIEWPORT RIGHT" << endl;
	}
    }		
}

//keyboard function 
/*void keyboard(unsigned char key, int x, int y)
{
	switch(tolower(key))
	{
		case 'q' : exit(0);
		case 'f' : //fill polygon
			break;
		case 't' : //tesselation triangles
			break;
		case 'l' : //back to initial outline
			break;
		case 'r' : //reflect
			break;
		case 's' : //stop animation
			break;
		case 'i' : //stop animation, return to original position
			break;
	}
}*/

int main(int argc, char** argv)
{
	myglutInit(argc, argv);
	myInit();
	glutMouseFunc(mouse);
	//glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
}
