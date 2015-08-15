#include "Application.h"

#pragma once

class Object{

	public:
		Object();

		virtual ~Object();
		virtual void Update(const float& deltaTime);
		virtual void Render(const Camera& camera);

		void SetPosition(vec3 position);
		void SetScale(vec3 scale);
		void SetRotation(vec3 rotation);

		vec3 GetPosition();
		vec3 GetRotation();
		float GetLeftX();
		float GetRightX();
		float GetTopY();
		float GetBottomY();
		

		void SaveObjectState(char *message = "Saved Object State");
		void LoadObjectState(char *message = "Loaded Object State");
		GLuint LoadBMP(const char * imagepath);
		GLuint LoadDDS(const char * imagepath);
		void LoadTriangles(const GLuint& perRow, const GLuint& perColumn, const GLenum& renderMode);
		
	protected:
		void BuildCube();
		void BuildTriangles(const GLuint& perRow, const GLuint& perColumn);
		void BuildTriangleStrip(const GLuint& perRow, const GLuint& perColumn);
		vec3 position, scale, rotation;
		Object* objectState;
		

	private:
		
		void LoadTriangles(GLfloat *vertices, GLfloat *uvs);
		float leftX, rightX, topY, bottomY;
		mat4 Render();
		GLuint numIndices, numUVs, textureID, vertexBufferID, renderMode, uvID;
};