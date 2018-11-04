void myglutInit(int argc, char** argv);

void myInit(void);

void plotPoint(double x, double y);

void pointSymmetry(double xCord, double yCord, double x, double y);

void midpointCircle(double xCord, double yCord, double radius);

void drawTreeLines(double x1, double y1, double x2, double y2);

void drawTree();

void display(void);

Vertex translate(double xTrans, double yTrans, Vertex translationVert);

Vertex rotate(double theta, Vertex rotateVert);

Vertex scale(double increaseScale, Vertex scaleVert);

void applyRotation(double theta);

void applyScaling(double increaseScale);

void applyTranslation(double xTrans, double yTrans);

void rotateShape(double speed);

void mouse(int button, int state, int x, int y);

void keyboard(unsigned char key, int x, int y);

int main(int argc, char** argv);
