#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

//constants--MUST CHANGE BEFORE TURNING IN

const int WINDOW_MAX = 600;
const int WINDOW_MIN = 0;
const int VIEWPORT_MAX =500;
const int VIEWPORT_MIN = 100;

//use the midpoint circle algorithm to find the points for the circle, store them in a structure, then by trial and error decide on which one to cut off at 

//struct declarations
//Point struct
struct Point{
	double x, y;
};

//global variables
vector<Point> myCircle;

void myglutInit(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(WINDOW_MAX, WINDOW_MAX);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Tree");

}

void myInit(void)
{
	//set black background
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glPointSize(1.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (float) WINDOW_MAX, 0.0, (float) WINDOW_MAX);
	glMatrixMode(GL_MODELVIEW);
	
}

//stores and draws point
void plotPoint(double x, double y)
{
	//store the point
	Point circPoint;
	circPoint.x = x;
	circPoint.y = y;
	myCircle.push_back(circPoint);
	
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
void midpointCircle(int xCord, int yCord, int radius)
{
	double xCordD = (double) xCord;
	double yCordD = (double) yCord;
	double x, y;
	x = 0;
	y = (double) radius;

	double p = (5/4) - (double)radius;
	
	plotPoint(x, y);
	pointSymmetry(xCordD, yCordD, x, y);
	
	
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
		pointSymmetry(xCordD, yCordD, x, y);
	}

	cout << "FINISHED" << endl;
	//cout << myCircle.size() << endl;

}
/*
void drawTree()
{

}*/

void display(void)
{
	//draw viewport window in white
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glRecti(VIEWPORT_MIN, VIEWPORT_MIN, VIEWPORT_MAX, VIEWPORT_MAX); 

	glColor3f(0.0, 0.0, 1.0);
	//put circle coordinate into constants 
	midpointCircle(VIEWPORT_MAX-100, VIEWPORT_MAX-200, 50);	
	//draw the tree
//	glColor3f(0.0f, 0.0f, 1.0f);
//	drawTree();
	glFlush();
}

int main(int argc, char** argv)
{
	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); //change to double later
	//glutInitWindowSize(WINDOW_MAX, WINDOW_MAX);
	//glutInitWindowPosition(0,0);
	//glutCreateWindow("Tree");
	myglutInit(argc, argv);
	myInit();
	//add mouse and keyboard functions here
	glutDisplayFunc(display);
	glutMainLoop();
}
