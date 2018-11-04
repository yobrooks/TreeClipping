//includes statements

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
	glPointSize(1.0);			
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (float) WINDOW_MAX, 0.0, (float) WINDOW_MAX);
	glMatrixMode(GL_MODELVIEW);							
}

void display(void)
{
	//draw viewport window in white
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glRecti(VIEWPORT_MIN, VIEWPORT_MIN, VIEWPORT_MAX, VIEWPORT_MAX); 
					
	drawTree(); //draw the tree
}
