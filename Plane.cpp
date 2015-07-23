#include "Plane.h"

Plane::Plane(GLuint width, GLuint height) : Object(){
	BuildTriangles(this->width = width, this->height = height);
	//SetPosition(vec3(0.0f, 0.0f, 0.0f));
}

Plane::~Plane(){
	
}