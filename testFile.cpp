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
	double x, y, z;
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
Tree tree;
Vertex treeCenterPoint;
bool stopAnimation = false;
bool initialized = false;
int SPIN = 0;
Circle transformationCircle;
BaseTree transformationBaseTree;
Tree transformationTree;

void myglutInit(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB); //change to double 
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
void definePoint(double x, double y) //***********
{
	//store the point
	Vertex vertex;
	vertex.x = x;
	vertex.y = y;
	vertex.z = 1;
	
	circle.myCircPoints.push_back(vertex);
//	circle.numCircPoints ++;
	//draw the point
/*	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
		glVertex2d(x, y);
	glEnd();*/
}

//everything in octant 5 and 6 (with starting octant being 0) needs to not be drawn...
//determine where the point will be in all eight octants
void pointSymmetry(double xCord, double yCord, double x, double y)
{
	definePoint(xCord + x, yCord + y); //***********
	definePoint(xCord - x, yCord + y);
	definePoint(xCord + x, yCord - y);
	definePoint(xCord - x, yCord - y);
	definePoint(xCord + y, yCord + x);
	//plotPoint(xCord - y, yCord + x);
	definePoint(xCord+ y, yCord - x);
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
	
	definePoint(x, y); //**********
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

void defineTreeLines(double x1, double y1, double x2, double y2)
{
	Vertex vert1, vert2;
	/*glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
		glVertex2d(x1, y1);	
		glVertex2d(x2, y2);
	glEnd();
	glFlush();*/ //**************

	vert1.x = x1; vert1.y = y1; vert1.z = 1;
	vert2.x = x2; vert2.y = y2; vert2.z = 1;
	
	baseTree.myBasePoints.push_back(vert1);
	baseTree.myBasePoints.push_back(vert2);
}


void defineTree() //*********
{

	//define circle info HERE!!!
	double circleCenterX = VIEWPORT_MAX-100;
	double circleCenterY = VIEWPORT_MAX-200;
	double circleRadius = 60;
	
	midpointCircle(circleCenterX, circleCenterY, circleRadius);
	//***********
	defineTreeLines(circleCenterX+20, circleCenterY+15, circleCenterX-42, circleCenterY+42);	
      
	defineTreeLines(circleCenterX+20, circleCenterY-15, circleCenterX-42, circleCenterY-42);
        
	defineTreeLines(circleCenterX+20, circleCenterY+15, VIEWPORT_MIN+100, VIEWPORT_MAX-175);
        
	defineTreeLines(circleCenterX+20, circleCenterY-15, VIEWPORT_MIN+100, VIEWPORT_MIN+160);

        defineTreeLines(VIEWPORT_MIN+100, VIEWPORT_MAX-175, VIEWPORT_MIN+100, VIEWPORT_MIN+160);

	//ADD CIRCLE AND BASE TO TREE HERE!!!
	//tree.myCircle = circle.myCircPoints;
	//tree.myBaseTree = baseTree.myBasePoints;
 	
	//add tree center point 
	treeCenterPoint.x = 315;
	treeCenterPoint.y = 298;
	treeCenterPoint.z = 1;
	
/*	glPointSize(10.0);
	glBegin(GL_POINTS);
		glVertex2d(treeCenterPoint.x, treeCenterPoint.y);
	glEnd();
	glFlush();*/
}

//draws the circle and tree //************
void drawTree(Circle circleParam, BaseTree baseTreeParam)
{
	glColor3f(0.0f, 0.0f, 1.0f);

	for(int i=0; i < circleParam.myCircPoints.size(); i++)
	{
		glBegin(GL_POINTS);
			glVertex2d(circleParam.myCircPoints[i].x, circleParam.myCircPoints[i].y);
		glEnd();
	}

	for(int i=0; i < baseTreeParam.myBasePoints.size()-1; i+=2)
	{
		glBegin(GL_LINES);
			glVertex2d(baseTreeParam.myBasePoints[i].x, baseTreeParam.myBasePoints[i].y);
			glVertex2d(baseTreeParam.myBasePoints[i+1].x, baseTreeParam.myBasePoints[i+1].y);
		glEnd();
	}
	
}

//build translation matrix
Vertex translate(double xTrans, double yTrans, Vertex translationVert)
{
	Vertex returnVert;
	returnVert.x = translationVert.x + (xTrans * translationVert.z);
	returnVert.y = translationVert.y + (yTrans * translationVert.z);
	returnVert.z = translationVert.z;

	return returnVert;
}

//build rotation matrix that does a rotation about the z axis; based on angle passed in
Vertex rotate(double theta, Vertex rotateVert)
{
	double angle = (theta*M_PI)/180.0; //convert angle to radians
	Vertex returnVert;

	returnVert.x = (rotateVert.x * cos(angle)) - (rotateVert.y * sin(angle));
	returnVert.y = (rotateVert.x * sin(angle)) + (rotateVert.y * cos(angle));	
	returnVert.z = rotateVert.z;	
	
	return returnVert;
}

//scale computation
Vertex scale(double increaseScale, Vertex scaleVert)
{
	//make scale into a decimal from percentage
	//increaseScale = increaseScale/100;
	Vertex returnVert;
	returnVert.x = increaseScale * scaleVert.x;
	returnVert.y = increaseScale * scaleVert.y;
	returnVert.z = scaleVert.z;

	return returnVert;
}

void applyRotation(double theta)
{
	Vertex tempVert;
	vector<Vertex> transformationVector;
	for(int i = 0; i < transformationCircle.myCircPoints.size(); i++)
	{
		tempVert = rotate(theta, transformationCircle.myCircPoints[i]);
		transformationVector.push_back(tempVert);
	}
	
	transformationCircle.myCircPoints.clear();
	transformationCircle.myCircPoints = transformationVector;
	transformationVector.clear();
	
	for(int i = 0; i < transformationBaseTree.myBasePoints.size(); i++)
	{
		tempVert = rotate(theta, transformationBaseTree.myBasePoints[i]);
		transformationVector.push_back(tempVert);
	}	
	
	transformationBaseTree.myBasePoints.clear();
	transformationBaseTree.myBasePoints = transformationVector;
	transformationVector.clear();
}

void applyScaling(double increaseScale)
{
	Vertex tempVert;
	vector<Vertex> transformationVector;

	for(int i = 0; i < transformationCircle.myCircPoints.size(); i++)
	{
		tempVert = scale(increaseScale, transformationCircle.myCircPoints[i]);
		transformationVector.push_back(tempVert);
	}

	transformationCircle.myCircPoints.clear();
	transformationCircle.myCircPoints = transformationVector;
	transformationVector.clear();

	for(int i = 0; i < transformationBaseTree.myBasePoints.size(); i++)
	{
		tempVert = scale(increaseScale, transformationBaseTree.myBasePoints[i]);
		transformationVector.push_back(tempVert);
	}

	transformationBaseTree.myBasePoints.clear();
	transformationBaseTree.myBasePoints = transformationVector;
	transformationVector.clear();
}

void applyTranslation(double xTrans, double yTrans)
{
	Vertex tempVert;
	vector<Vertex> transformationVector;

	for(int i = 0; i < transformationCircle.myCircPoints.size(); i++)
	{
		tempVert = translate(xTrans, yTrans, transformationCircle.myCircPoints[i]);
		transformationVector.push_back(tempVert);
	}
	
	transformationCircle.myCircPoints.clear();
	transformationCircle.myCircPoints = transformationVector;
	transformationVector.clear();
	
	for(int i = 0; i < transformationBaseTree.myBasePoints.size(); i++)
	{
		tempVert = translate(xTrans, yTrans, transformationBaseTree.myBasePoints[i]);
		transformationVector.push_back(tempVert);
	}

	transformationBaseTree.myBasePoints.clear();
	transformationBaseTree.myBasePoints = transformationVector;
	transformationVector.clear();
}

void rotateShape(double speed)
{
	applyTranslation((treeCenterPoint.x * -1), (treeCenterPoint.y * -1));
	applyRotation(speed);
	applyTranslation(treeCenterPoint.x, treeCenterPoint.y);
}

void display(void)
{
        //draw viewport window in white
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glRecti(VIEWPORT_MIN, VIEWPORT_MIN, VIEWPORT_MAX, VIEWPORT_MAX);
        if(initialized == false)
       	{
        	 defineTree();
		 transformationCircle.myCircPoints = circle.myCircPoints;
		 transformationBaseTree.myBasePoints = baseTree.myBasePoints;
         	 initialized = true;
        }

        if(SPIN == 0)
        {
       		 drawTree(circle, baseTree);       
	} 
                   
        if(SPIN != 0)
	{     
		   rotateShape((double) SPIN);                                                                                                                drawTree(transformationCircle, transformationBaseTree); 
	}
	                                                                                                                                           glutSwapBuffers();
}
void SpinDisplay(void)
{
	if(SPIN > 360)
	{
		SPIN = SPIN -360;
	}

	glutPostRedisplay();
}
	
//mouse function
void mouse(int button, int state, int x, int y)
{
	//if mouse inside of viewport
    	y = WINDOW_MAX-y;

if(stopAnimation==false){
    if (x>VIEWPORT_MIN && x<VIEWPORT_MAX && y > VIEWPORT_MIN && y < VIEWPORT_MAX)
    {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//increase speed of rotation
		SPIN++;
		glutIdleFunc(SpinDisplay);
	//	cout << "INSIDE VIEWPORT LEFT" << endl;

	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		SPIN--;
		glutIdleFunc(SpinDisplay);
		//slow down rotation
	//	cout << "INSIDE VIEWPORT RIGHT" << endl;

	}
    }
}
	//NOT WORKING!!!
	//else if mouse is outside viewport
    else
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
void keyboard(unsigned char key, int x, int y)
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
		case 's' : stopAnimation = true; glutIdleFunc(display);
			break;
		case 'i' : stopAnimation = true; SPIN = 0; glutIdleFunc(display);
			 //stop animation, return to original position
			break;
	}
}

int main(int argc, char** argv)
{
	myglutInit(argc, argv);
	myInit();
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
}
