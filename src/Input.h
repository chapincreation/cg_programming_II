//GLFW provides window creation and input control.
#include <glfw3.h>

#pragma once

//Look up keys here: http://www.glfw.org/docs/latest/group__keys.html

extern char gKeyA;
extern char gKeyW;
extern char gKeyD;
extern char gKeyS;

void UpdateKeys(GLFWwindow *window);
