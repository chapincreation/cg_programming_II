#include "Cube.h"

Cube::Cube(GLuint width, GLuint height, GLuint depth) : Object(){
	BuildTriangles(this->width = width, this->height = height);
	//SetPosition(vec3(0.0f, 0.0f, 0.0f));
}

Cube::~Cube(){
	
}