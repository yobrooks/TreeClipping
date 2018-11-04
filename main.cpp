

int main(int argc, char** argv)
{
	myglutInit(argc, argv);
	myInit();
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
}
