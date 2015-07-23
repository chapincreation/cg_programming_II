#include "Application.h"
#include "Object.h"

#pragma once

class Cube : public Object{

	public:
		Cube(GLuint width, GLuint height, GLuint depth);
		virtual ~Cube();

	private:
		GLuint width, height, depth;
};