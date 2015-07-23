#include "Application.h"
#include "Object.h"

#pragma once



class Player : public Object{
	
	public:
		Player(GLuint width, GLuint height);
		virtual ~Player();

		void Update(const float& deltaTime);
		void SetKeys(const int& upKey, const int&downKey, const int& leftKey, const int&rightKey);
		
		

	private:
		GLuint width, height;
		//vec3 position;
		int upKey, downKey, leftKey, rightKey;
};