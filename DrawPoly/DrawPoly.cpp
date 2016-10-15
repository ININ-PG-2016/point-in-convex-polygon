// DrawPoly.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#ifndef MACOSX
#include "GL/glut.h"
#else
#include "GLUT/glut.h"
#endif

struct Point2D{
public:
	Point2D(float x, float y) : x(x), y(y) {}

	float x;
	float y;
};

std::vector<Point2D> points;

bool loadFromFile(char *filename)
{
	std::ifstream in(filename);

	//Could not open the stream
	if (!in)
	{
		std::cout << "File read error!" << std::endl;
		return false;
	}

	char line[255];

	while (in.good())
	{
		in.getline(line, 255);

		if (in)
		{
			std::vector<float> values;
			char *token = strtok(line, " ");
			
			while (token != NULL)
			{
				if (strcmp(token, "") == 0)
				{
					token = strtok(NULL, " ");
					continue;
				}

				std::stringstream tokenstream(token);

				float tmp;
				tokenstream >> tmp;
				values.push_back(tmp);

				token = strtok(NULL, " ");
			}

			if (values.size() != 2)
			{
				std::cout << "Invalid data!" << std::endl;
				return false;
			}

			points.push_back(Point2D(values[0], values[1]));
		}
	}

	in.close();
	return true;
}

void resize(int width, int height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	int minDimension = (width < height) ? width : height;


	//force window to be square shape
	glutReshapeWindow(minDimension, minDimension);

	glViewport(0.05 * minDimension, 0.05 * minDimension, 0.9 * minDimension, 0.9 * minDimension);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	gluOrtho2D(0.0, width, 0.0, height);

//	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
//	glLoadIdentity();									// Reset The Modelview Matrix
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	
	glScaled(0.9, 0.9, 0.9);
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(3);

	glBegin(GL_LINE_LOOP);
	
	for (int i = 0; i < points.size(); i++)
	{
		Point2D rendered = points[i];
		glVertex2f(rendered.x, rendered.y);
	}

	glEnd();

	glutSwapBuffers();
}

int main(int argc, char **argv) {
	if (!loadFromFile("out.poly")) {
		exit(1);
	}


	glutInit(&argc, argv);

	// initialize the OpenGL GLUT window
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 1000);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);

	// create display context and initialize GLEW library
	glutCreateWindow("Polygon visualization");

	glutDisplayFunc(render);
	glutReshapeFunc(resize);

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	glutMainLoop();
    return 0;
}

