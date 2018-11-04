

//mouse function
void mouse(int button, int state, int x, int y)
{
	//if mouse inside of viewport
	y = WINDOW_MAX - y;
	double speed = 0;

	if (stopAnimation == false) {
		if (x>VIEWPORT_MIN && x<VIEWPORT_MAX && y > VIEWPORT_MIN && y < VIEWPORT_MAX)
		{
			if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			{
				//increase speed of rotation
				speed++;
				//	cout << "INSIDE VIEWPORT LEFT" << endl;
				rotateShape(speed);

			}

			if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
			{
				speed--;
				rotateShape(speed);
				//slow down rotation
				//	cout << "INSIDE VIEWPORT RIGHT" << endl;

			}
		}
	}
	//NOT WORKING!!!
	//else if mouse is outside viewport
	else if (x > VIEWPORT_MAX &&  x < WINDOW_MAX && y< VIEWPORT_MIN && y > WINDOW_MIN)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			//increase scale of tree
			cout << "OUTSIDE VIEWPORT LEFT" << endl;
		}

		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{
			//decrease scale of tree
			cout << "OUTSIDE VIEWPORT RIGHT" << endl;
		}
	}
}

//keyboard function 
void keyboard(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'q': exit(0);
	case 'f': //fill polygon
		break;
	case 't': //tesselation triangles
		break;
	case 'l': //back to initial outline
		break;
	case 'r': //reflect
		break;
	case 's': stopAnimation = true;
		break;
	case 'i': //stop animation, return to original position
		break;
	}
}