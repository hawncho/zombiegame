/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include <stdlib.h>
#include <stdio.h>
#include "GLUTBackend.h"

ICallbacks* pCallbacks = nullptr;
bool IsFullScreen = false;

int WindowWidth = 0;
int WindowHeight = 0;

// Set of GLUT callback functions that forward the GLUT callback calls to ICallbacks 
void DisplayCB()
{
	pCallbacks->DisplayCB();
}

void IdleCB()
{
	pCallbacks->IdleCB();
}

void KeyboardCB(unsigned char Key, int x, int y)
{
	pCallbacks->KeyboardCB(Key, x, y);
}

void KeyboardUpCB(unsigned char Key, int x, int y)
{
	pCallbacks->KeyboardUpCB(Key, x, y);
}

void MouseCB(int button, int state, int x, int y)
{
	pCallbacks->MouseCB(button, state, x, y);
}

void MovingMouseCB(int x, int y)
{
	pCallbacks->MovingMouseCB(x, y);
}

void PassiveMouseCB(int x, int y)
{
	pCallbacks->PassiveMouseCB(x, y);
}

// Registers local GLUT callbacks
void InitializeGLUTCallbacks()
{
	glutDisplayFunc(DisplayCB);
	glutIdleFunc(IdleCB);
	glutKeyboardFunc(KeyboardCB);
	glutKeyboardUpFunc(KeyboardUpCB);
	glutMouseFunc(MouseCB);
	glutMotionFunc(MovingMouseCB);
	glutPassiveMotionFunc(PassiveMouseCB);
}

// Initializes GLEW and checking for errors
void InitializeGLEW()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error initializing Glew: %s\n", glewGetErrorString(err));
		exit(1);
	}
}

// Initializes a GLUT window with specified dimensions and title
// Centers the window on the screen
void InitializeWindow(int argc, char** argv, int width, int height, const char* windowTitle, ICallbacks* callbacks)
{
	pCallbacks = callbacks;

	WindowWidth = width;
	WindowHeight = height;

	glutInit(&argc, argv);

	glutInitWindowSize(WindowWidth, WindowHeight);

	int ScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int ScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitWindowPosition((ScreenWidth >> 1) - (width >> 1), (ScreenHeight >> 1) - (height >> 1));

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutCreateWindow(windowTitle);

	glutSetCursor(GLUT_CURSOR_NONE);

	InitializeGLUTCallbacks();

	InitializeGLEW();
}

void ToggleFullScreen()
{
	if (IsFullScreen)
	{
		glutReshapeWindow(WindowWidth, WindowHeight);

		IsFullScreen = false;
	}
	else
	{
		glutFullScreen();
		glutWarpPointer(0, 0);

		IsFullScreen = true;
	}
}

void StartGlutLoop()
{
	glutMainLoop();
}