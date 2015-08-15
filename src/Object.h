#include "Application.h"

#pragma once

class Object{

    //Methods…
	public:
		Object();

		virtual ~Object();
		virtual void Update(const float& deltaTime);
		virtual void Render(const Camera& camera);

		void SetPosition(vec3 position);
		void SetScale(vec3 scale);
        void Rotate(float rotAngle, vec3 rotAxis);
        void RotateOverTime(float rotSpeed, vec3 rotAxis);

		vec3 GetPosition();

		void SaveObjectState(char *message = (char*)"Saved Object State");
		void LoadObjectState(char *message = (char*)"Loaded Object State");
		GLuint LoadBMP(const char * imagepath);

	protected:
		void BuildCube();
        mat4 BeforeRender();
    
    //Members…
    protected:
		Object* mObjectState;
		vec3 mPosition, mScale, mRotAxis;
		float mRotSpeed, mRotAngle;
		GLuint mVertexBufferID, mUvID, mNormID;
		GLuint mNumIndices, mNumUVs, mTextureID, mRenderMode;
};