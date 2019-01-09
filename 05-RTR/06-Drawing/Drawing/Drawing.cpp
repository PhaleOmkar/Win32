// Headers
#include <GL/freeglut.h>

// Helper Defines

#define COLOR3F(r,g,b)  glColor3f(r/256.0f, g/256.0f, b/256.0f)
#define POINT(x,y)		glVertex2f(x + (fFactor * i), y - (fFactor * j))

#define YELLOW		COLOR3F(242.0f, 244.0f, 6.0f);
#define WHITE		glColor3f(   1.0f,    1.0f,    1.0f);
#define GREY		glColor3f(0.2555f, 0.2555f, 0.2555f);
#define ORANGE		glColor3f(0.9961f, 0.4453f, 0.0703f);
#define DARK_GREEN  glColor3f(0.3672f, 0.6367f, 0.4531f);
#define DARK_BLUE   glColor3f(0.1211f, 0.1094f, 0.2188f);
#define DARK_RED    glColor3f(0.3008f, 0.0234f, 0.0234f);
#define LIGHT_GREEN glColor3f(0.6758f, 0.9961f, 0.4766f);
#define LIGHT_BLUE  glColor3f(0.4492f, 0.5820f, 0.7031f);
#define LIGHT_RED   glColor3f(0.8867f, 0.6406f, 0.6406f);

#define GREEN

#ifdef BLUE
#define DARK		DARK_BLUE;
#define LIGHT		LIGHT_BLUE;
#endif

#ifdef GREEN
#define DARK		DARK_GREEN;
#define LIGHT		LIGHT_GREEN;
#endif

#ifdef RED
#define DARK		DARK_RED;
#define LIGHT		LIGHT_RED;
#endif



bool bIsFullScreen = false;

int main(int argc, char *argv[])
{
	// function declarations
	void initialize(void);
	void uninitialize(void);
	void reshape(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);

	// code
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Drawing");

	initialize();

	// callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	return(0);
}

void initialize(void)
{
	// code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glutFullScreen();
}

void uninitialize(void)
{
	// code
}

void reshape(int width, int height)
{
	// code
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void display(void)
{
	// variables
	int i, j;
	float xOffset[] = { 0.0f, 0.25f, 0.50f, 0.75f, 1.0f, 1.25f, 1.50f, 1.75f };
	float yOffset[] = { 0.0f, -0.25f, -0.50f, -0.75f, -1.0f, -1.25f, -1.50f, -1.75f };
	float fFactor = 0.0625f;

	// code
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Origin for OpenGL is located at LOWER LEFT corner
	glBegin(GL_TRIANGLES);

	// Flat Triangles
	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 32; j++)
		{
			//// Upper Triangle
			//DARK POINT(-1.0f, 1.0f);
			//LIGHT POINT(0.0f, 1.0f);
			//DARK POINT(-1.0f, 0.0f);

			//// Lower Triangle
			//DARK POINT(-1.0f, 0.0f);
			//DARK POINT(0.0f, 1.0f);
			//LIGHT POINT(0.0f, 0.0f);

			// Upper Triangle
			DARK POINT(-1.0f, 1.0f);
			LIGHT POINT(-0.9375f, 1.0f);
			DARK POINT(-1.0f, 0.9375f);

			// Lower Triangle
			DARK POINT(-1.0f, 0.9375f);
			DARK POINT(-0.9375f, 1.0f);
			LIGHT POINT(-0.9375f, 0.9375f);
		}
	}

	// Slanted Triangles
	//for (i = 0; i < 4; i++)
	//{
	//	// Upper Triangle
	//	LIGHT glVertex2f((-1.0f / fFactor) + xOffset[i], (1.0f / fFactor) + yOffset[i]);

	//	DARK glVertex2f((0.0f / fFactor) + xOffset[i], (0.5f / fFactor) + yOffset[i]);

	//	LIGHT glVertex2f((-1.0f / fFactor) + xOffset[i], (0.0f / fFactor) + yOffset[i]);

	//	// Lower Triangle
	//	DARK glVertex2f((-1.0f / fFactor) + xOffset[i], (0.0f / fFactor) + yOffset[i]);

	//	LIGHT glVertex2f((0.0f / fFactor) + xOffset[i], (0.5f / fFactor) + yOffset[i]);

	//	LIGHT glVertex2f((0.0f / fFactor) + xOffset[i], (-0.5f / fFactor) + yOffset[i]);
	//}

	// Spotted Pattern
	//for (i = 0; i < 4; i++)
	//{
	//	LIGHT POINT(-1.0f, 1.0f);
	//	LIGHT POINT(0.0f, 1.0f);
	//	LIGHT POINT(-0.5f, 0.5f);

	//	DARK POINT(0.0f, 1.0f);
	//	DARK POINT(0.0f, 0.0f);
	//	DARK  POINT(-0.5f, 0.5f);

	//	LIGHT POINT(-1.0f, 0.0f);
	//	LIGHT POINT(-0.5f, 0.5f);
	//	LIGHT POINT(0.0f, 0.0f);

	//	DARK POINT(-1.0f, 1.0f);
	//	DARK  POINT(-0.5f, 0.5f);
	//	DARK POINT(-1.0f, 0.0f);
	//}



	////// SUNSET //////

	for (i = 13; i < 18; i++)
	{
		for (j = 14; j <= 19; j++)
		{
			// Upper Triangle
			YELLOW POINT(-1.0f, 1.0f);
			WHITE POINT(-0.9375f, 1.0f);
			YELLOW POINT(-1.0f, 0.9375f);

			// Lower Triangle
			YELLOW POINT(-1.0f, 0.9375f);
			YELLOW POINT(-0.9375f, 1.0f);
			ORANGE POINT(-0.9375f, 0.9375f);
		}
	}

	////////////////////


	
	glEnd();

	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	// code 
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;

	case 'F':
	case 'f':
		if (bIsFullScreen == false)
		{
			glutFullScreen();
			bIsFullScreen = true;
		}
		else
		{
			glutLeaveFullScreen();
			bIsFullScreen = false;
		}
	}
}

void mouse(int button, int state, int x, int y)
{
	// code
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		break;

	case GLUT_RIGHT_BUTTON:
		//glutLeaveMainLoop();
		break;
	}
}
