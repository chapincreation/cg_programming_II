#include "Input.h"

char gKeyA = 0, gKeyW = 0, gKeyD = 0, gKeyS = 0;

void UpdateKeys(GLFWwindow *window)
{
	gKeyA = gKeyW = gKeyD = gKeyS = 0;

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gKeyA = 1;
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gKeyW = 1;
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gKeyD = 1;
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gKeyS = 1;
}
