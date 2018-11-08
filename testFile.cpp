#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

//constants for the graphics and viewport window sizes

const int WINDOW_MAX = 600;
const int WINDOW_MIN = 0;
const int VIEWPORT_MAX =500;
const int VIEWPORT_MIN = 100;


//struct declarations
//vertex struct to hold the x, y position of a point 
//and the z homogenous coordinate of the point
struct Vertex{
	double x, y, z;
};

//circle struct that holds vector of Vertex points
struct Circle{
	vector<Vertex> myCircPoints;
};

//struct that holds the points for the base of the tree
struct BaseTree{
	vector<Vertex> myBasePoints;	
};

//MAYBE TRY TO HAVE A VECTor WITH ALL THE POINTS ---IT MIGHT HELP GRAPHICS A BIT SO it won't be doing the circle then the base

//global variables
//these hold the original points
Circle circle;
BaseTree baseTree;
Vertex treeCenterPoint; 
bool stopAnimation = false; //boolean to determine when the animation of rotation needs to be stopped
bool initialized = false; //boolean to tell whether the tree has been defined initially yet
int SPIN = 0; //angle of rotation
//these hold the points that get transformed
Circle transformationCircle; 
BaseTree transformationBaseTree;


void myglutInit(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB); //implements double buffering 
	glutInitWindowSize(WINDOW_MAX, WINDOW_MAX); //size of window
	glutInitWindowPosition(0,0);
	glutCreateWindow("Animation Tree");

}

void myInit(void)
{
	//set black background
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glPointSize(1.0); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (float) WINDOW_MAX, 0.0, (float) WINDOW_MAX); //set window position
	glMatrixMode(GL_MODELVIEW);	
}

//defines the point for the circle and stores it in the circle struct
void definePoint(double x, double y) 
{
	Vertex vertex;
	vertex.x = x;
	vertex.y = y;
	vertex.z = 1; //homogenous coordinate
	
	circle.myCircPoints.push_back(vertex);
}

//determine where the point will be in all eight octants based on symmetry
//taken from textbook
//eliminates certain octants (done by trial and error) to not be drawn
//thus making more of an arc and opening up the tree to connect to the rest of the tree
void pointSymmetry(double xCord, double yCord, double x, double y)
{
	definePoint(xCord + x, yCord + y); 
	definePoint(xCord - x, yCord + y);
	definePoint(xCord + x, yCord - y);
	definePoint(xCord - x, yCord - y);
	definePoint(xCord + y, yCord + x);
	definePoint(xCord + y, yCord - x);			
}

//midpoint circle algorithm taken from pgs 150 - 153 of textbook 
void midpointCircle(double xCord, double yCord, double radius)
{
	//set initial point
	double x, y;
	x = 0;
	y = radius;

	double p = (5/4) - radius; //make decision variable
	
	definePoint(x, y); //add initial point to circle
	pointSymmetry(xCord, yCord, x, y); //define first points in other octants

	//loop to determine where next points should go
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

//define tree points as vertices and put them in vector for the base of tree 
void defineTreeLines(double x1, double y1, double x2, double y2)
{
	Vertex vert1, vert2;

	vert1.x = x1; vert1.y = y1; vert1.z = 1;
	vert2.x = x2; vert2.y = y2; vert2.z = 1;
	
	baseTree.myBasePoints.push_back(vert1);
	baseTree.myBasePoints.push_back(vert2);
}

//defines the entire tree based on the functions defineTreeLines and midPoint Circle
void defineTree()
{

	//base information about circle defined here 
	double circleCenterX = VIEWPORT_MAX-100;
	double circleCenterY = VIEWPORT_MAX-200;
	double circleRadius = 60;
	
	midpointCircle(circleCenterX, circleCenterY, circleRadius);
	
	defineTreeLines(circleCenterX+20, circleCenterY+15, circleCenterX-42, circleCenterY+42);	
      
	defineTreeLines(circleCenterX+20, circleCenterY-15, circleCenterX-42, circleCenterY-42);
        
	defineTreeLines(circleCenterX+20, circleCenterY+15, VIEWPORT_MIN+100, VIEWPORT_MAX-175);
        
	defineTreeLines(circleCenterX+20, circleCenterY-15, VIEWPORT_MIN+100, VIEWPORT_MIN+160);

        defineTreeLines(VIEWPORT_MIN+100, VIEWPORT_MAX-175, VIEWPORT_MIN+100, VIEWPORT_MIN+160);
 	
	//add tree center point where all transformation are based on 
	treeCenterPoint.x = 315;
	treeCenterPoint.y = 298;
	treeCenterPoint.z = 1;
}

//draws the circle and base of the tree connected on the sceen 
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

//computes and returns the new vertex that comes from translating by the paramaters
Vertex translate(double xTrans, double yTrans, Vertex translationVert)
{
	Vertex returnVert;
	returnVert.x = translationVert.x + (xTrans * translationVert.z);
	returnVert.y = translationVert.y + (yTrans * translationVert.z);
	returnVert.z = translationVert.z;

	return returnVert;
}

//computes and returns the vertex that comes from rotation about the z axis based on angle passed in
Vertex rotate(double theta, Vertex rotateVert)
{
	double angle = (theta*M_PI)/180.0; //convert angle to radians
	Vertex returnVert;

	returnVert.x = (rotateVert.x * cos(angle)) - (rotateVert.y * sin(angle));
	returnVert.y = (rotateVert.x * sin(angle)) + (rotateVert.y * cos(angle));	
	returnVert.z = rotateVert.z;	
	
	return returnVert;
}

//computes and returns the vertex that comes from scaling the vertex by the scale factor passed in
Vertex scale(double increaseScale, Vertex scaleVert)
{
	//make scale into a decimal from percentage
	increaseScale = increaseScale/100;
	Vertex returnVert;
	returnVert.x = (increaseScale * scaleVert.x) + scaleVert.x;
	returnVert.y = (increaseScale * scaleVert.y) + scaleVert.y;
	returnVert.z = scaleVert.z;

	return returnVert;
}

//applies the rotation transformation to the transformation structs
void applyRotation(double theta)
{
	Vertex tempVert;
	vector<Vertex> transformationVector; //temporary vector to hold the new transformed points

	//loops through the transformationCircle struct to rotate each point
	//puts the new point in the temporary vector
	for(int i = 0; i < transformationCircle.myCircPoints.size(); i++)
	{
		tempVert = rotate(theta, transformationCircle.myCircPoints[i]);
		transformationVector.push_back(tempVert);
	}
	
	//clear the transformation circle vector and set the temporary vector just filled as the new one
	//clear the temporary vector and repeat with the transformation baseTree. 
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

//applies scaling the transformation structs
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

//applies translation to the transformation structs
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

//rotates the shape by translating center point of tree to the origin
//applyign the rotation based on angle passed in
//and translates shape back to original position
void rotateShape(double speed)
{
	applyTranslation((treeCenterPoint.x * -1), (treeCenterPoint.y * -1));
	applyRotation(speed);
	applyTranslation(treeCenterPoint.x, treeCenterPoint.y);
}

//scales shape by translating from center point to origin
//applying the scaling by the scale factor
//ans translating back to original position
void scaleShape(double scaleFactor)
{
	applyTranslation((treeCenterPoint.x * -1), (treeCenterPoint.y * -1));
	applyScaling(scaleFactor);
	applyTranslation(treeCenterPoint.x, treeCenterPoint.y);
}

void resetShape()
{
	SPIN = 0;
	transformationCircle.myCircPoints.clear();
	transformationCircle.myCircPoints = circle.myCircPoints;
	transformationBaseTree.myBasePoints.clear();
	transformationBaseTree.myBasePoints = baseTree.myBasePoints;
}

//what is to be displayed on the screen 
void display(void)
{
        //draw viewport rectangle window in white
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glRecti(VIEWPORT_MIN, VIEWPORT_MIN, VIEWPORT_MAX, VIEWPORT_MAX);
       
	 //if the shape has not been initially drawn yet,
	 //define the tree points, add them to the transformation structs as the initial points to start transformations from
	 if(initialized == false)
       	 {
        	 defineTree();
		 transformationCircle.myCircPoints = circle.myCircPoints;
		 transformationBaseTree.myBasePoints = baseTree.myBasePoints;
         	 initialized = true;
         }

	//if the animation has been stopped, and the spin is 0
	//draw tree in the position it was in when it was stopped
	//if(SPIN == 0 && stopAnimation == true)
	if(SPIN == 0)
	{
		drawTree(transformationCircle, transformationBaseTree);
	}

	//if the animation has not started and thus the spin is 0,
	//draw the original tree in its original position
	//also used to return the tree to its original position
	/*if(SPIN == 0 && stopAnimation == false)
	{
		drawTree(transformationCircle, transformationBaseTree);
	}*/

         
	//if the spin is not equal to 0, rotate the shape and draw the new one     
	//keeps updating until animation is stopped     
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
	//change from mouse coordinate of y position to window coordinate of y position
    	y = WINDOW_MAX-y;

	//if the mouse clicks inside the viewport window
    if (x>VIEWPORT_MIN && x<VIEWPORT_MAX && y > VIEWPORT_MIN && y < VIEWPORT_MAX)
    {
	//if left button pressed 
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//increase speed of rotation
		SPIN++;
		glutIdleFunc(SpinDisplay);
	}

	//if right button pressed 
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//slow down rotation speed
		SPIN--;
		glutIdleFunc(SpinDisplay);
	}
    }

	//else if mouse is outside viewport window
        else
    	{
		//if left button pressed
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			//increase scale of tree
			scaleShape(5);
			glutIdleFunc(display); 
		
		}		
		
		//if right button pressed
		if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{	
			//decrease scale of tree
			scaleShape(-5);
			glutIdleFunc(display);
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
		case 'r' : rotateShape(180); glutIdleFunc(display); 
			break; //rotate the shape by 180 degrees to produce a reflection
		case 's' : SPIN = 0; stopAnimation = true; glutIdleFunc(display);
			break; //stop animation where it is 
		case 'i' : resetShape(); glutIdleFunc(display);
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
