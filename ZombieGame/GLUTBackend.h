/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef GLUTBACKEND_H_
#define GLUTBACKEND_H_

#include <GL\glew.h>
#include <GL\glut.h>

#include "GLUTCallbacks.h"

void InitializeWindow(int argc, char** argv, int width, int height, const char* windowTitle, ICallbacks* callbacks);

void StartGlutLoop();

void ToggleFullScreen();

#endif /* GLUTBACKEND_h_ */