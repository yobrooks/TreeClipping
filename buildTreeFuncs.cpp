

//stores and draws point
void plotPoint(double x, double y)
{
	//store the point
	Vertex vertex;
	vertex.x = x;
	vertex.y = y;
	vertex.z = 1;

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
	plotPoint(xCord + y, yCord - x);
	//plotPoint(xCord - y, yCord - x);			
}

//midpoint circle algorithm taken from pgs 150 - 153 of textbook 
//change all to integers and convert to doubles at the end 
void midpointCircle(double xCord, double yCord, double radius)
{
	double x, y;
	x = 0;
	y = radius;

	double p = (5 / 4) - radius;

	plotPoint(x, y);
	pointSymmetry(xCord, yCord, x, y);


	while (x < y)
	{
		x++;

		if (p < 0)
		{
			p = p + 2 * (x + 1);
		}
		else
		{
			y--;
			p = p + 2 * (x - y) + 1;
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

	vert1.x = x1; vert1.y = y1; vert1.z = 1;
	vert2.x = x2; vert2.y = y2; vert2.z = 1;

	baseTree.myBasePoints.push_back(vert1);
	baseTree.myBasePoints.push_back(vert2);
}


void drawTree()
{

	//define circle info HERE!!!
	double circleCenterX = VIEWPORT_MAX - 100;
	double circleCenterY = VIEWPORT_MAX - 200;
	double circleRadius = 60;

	midpointCircle(circleCenterX, circleCenterY, circleRadius);

	drawTreeLines(circleCenterX + 20, circleCenterY + 15, circleCenterX - 42, circleCenterY + 42);

	drawTreeLines(circleCenterX + 20, circleCenterY - 15, circleCenterX - 42, circleCenterY - 42);

	drawTreeLines(circleCenterX + 20, circleCenterY + 15, VIEWPORT_MIN + 100, VIEWPORT_MAX - 175);

	drawTreeLines(circleCenterX + 20, circleCenterY - 15, VIEWPORT_MIN + 100, VIEWPORT_MIN + 160);

	drawTreeLines(VIEWPORT_MIN + 100, VIEWPORT_MAX - 175, VIEWPORT_MIN + 100, VIEWPORT_MIN + 160);

	//ADD CIRCLE AND BASE TO TREE HERE!!!
	//tree.myCircle = circle.myCircPoints;
	//tree.myBaseTree = baseTree.myBasePoints;

	//add tree center point 
	treeCenterPoint.x = 315;
	treeCenterPoint.y = 298;
	treeCenterPoint.z = 1;

	glPointSize(10.0);
	glBegin(GL_POINTS);
	glVertex2d(treeCenterPoint.x, treeCenterPoint.y);
	glEnd();
	glFlush();
}