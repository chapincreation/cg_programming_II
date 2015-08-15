//Standard Headers…
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <string>
#include <functional>
#include <vector>

//If we are on windows we want a single define for it...
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
 #define _WIN32
#endif

#ifdef _WIN32	//read in directory files using DIR
	#include "dirent.h" //Since not available on windows, will need to bring in our own header...
#else
	#include <dirent.h>
#endif

//GLM provides openGL mathematics.
#define GLM_FORCE_RADIANS	//Defining this before glm to ignore warnings we don't care about...
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

//GLEW provides the function pointers we need to access the latest features in openGL.
#include <GL/glew.h>    //Must be included before gl.h and glfw.h.

//GLFW provides window creation and input control.
#include <glfw3.h>

#include "Input.h"

#pragma once

//Namespaces…
using namespace glm; //For openGL mathematics
using namespace std; //If we want to print stuff

//Defines…
#define APP_NAME "cg programming II"
#define EXIT_WITH_ERROR -1
#define EXIT_WITH_SUCCESS 0
#define OPEN_GL_VERSION 3  //Specifies OpenGL 3.3
#define ANTIALIASING 4  //nx antialiasing
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define FIELD_OF_VIEW 45.0f
#define Z_NEAR 0.1f
#define Z_FAR 100.0f
#define GLFW_FAIL 0

#ifdef _WIN32
    #define RESOURCE_PATH "../resource/"
#else
    #define RESOURCE_PATH "resource/"
#endif

//TypedefsÉ
typedef vector<string> stdVecStr;
typedef vector<unsigned int> stdVecUInt;

//External Globals…
extern GLFWwindow* window;

struct Camera{
	Camera(float xPos, float yPos, float zPos, float moveSpeed) :
		xPos(xPos), yPos(yPos), zPos(zPos), moveSpeed(moveSpeed){}

	GLuint MVPMatrixID;
	mat4 projectionMatrix, viewMatrix, MVPMatrix;
	float xPos, yPos, zPos;
	float moveSpeed;
};

namespace setup
{
    void window_refresh_callback(GLFWwindow* window);
    int InitWindowFailed();
    GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
    GLuint *LoadShaders(size_t &numShaders);
    int InitGlewFailed();
    double getDeltaTime();
    stdVecStr ReadFiles(const char* fExt);
}