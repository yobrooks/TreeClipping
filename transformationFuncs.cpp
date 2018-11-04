

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
	double angle = (theta*M_PI) / 180.0; //convert angle to radians
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
	for (int i = 0; i < circle.myCircPoints.size(); i++)
	{
		tempVert = rotate(theta, circle.myCircPoints[i]);
		transformationVector.push_back(tempVert);
	}

	transformationCircle.myCircPoints = transformationVector;
	transformationVector.clear();

	for (int i = 0; i < baseTree.myBasePoints.size(); i++)
	{
		tempVert = rotate(theta, baseTree.myBasePoints[i]);
		transformationVector.push_back(tempVert);
	}

	transformationBaseTree.myBasePoints = transformationVector;
	transformationVector.clear();
}

void applyScaling(double increaseScale)
{
	Vertex tempVert;
	vector<Vertex> transformationVector;

	for (int i = 0; i < circle.myCircPoints.size(); i++)
	{
		tempVert = scale(increaseScale, circle.myCircPoints[i]);
		transformationVector.push_back(tempVert);
	}

	transformationCircle.myCircPoints = transformationVector;
	transformationVector.clear();

	for (int i = 0; i < baseTree.myBasePoints.size(); i++)
	{
		tempVert = scale(increaseScale, baseTree.myBasePoints[i]);
		transformationVector.push_back(tempVert);
	}

	transformationBaseTree.myBasePoints = transformationVector;
	transformationVector.clear();
}

void applyTranslation(double xTrans, double yTrans)
{
	Vertex tempVert;
	vector<Vertex> transformationVector;

	for (int i = 0; i < circle.myCircPoints.size(); i++)
	{
		tempVert = translate(xTrans, yTrans, circle.myCircPoints[i]);
		transformationVector.push_back(tempVert);
	}

	transformationCircle.myCircPoints = transformationVector;
	transformationVector.clear();

	for (int i = 0; i < baseTree.myBasePoints.size(); i++)
	{
		tempVert = translate(xTrans, yTrans, baseTree.myBasePoints[i]);
		transformationVector.push_back(tempVert);
	}

	transformationBaseTree.myBasePoints = transformationVector;
	transformationVector.clear();
}

void rotateShape(double speed)
{
	applyTranslation((treeCenterPoint.x * -1), (treeCenterPoint.y * -1));
	applyRotation(speed);
	applyTranslation(treeCenterPoint.x, treeCenterPoint.y);
}