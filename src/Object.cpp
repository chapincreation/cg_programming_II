#include "Object.h"

Object::Object(){
	mObjectState = nullptr;
	mRenderMode = GL_TRIANGLES;
	mVertexBufferID = mUvID = mNormID = 0;
	mNumIndices = 6;
    mPosition = vec3(0, 0, 0);
    mScale = vec3(1, 1, 1);
    mRotAxis = vec3(0, 1, 0);
	mNumUVs = mTextureID = mRotSpeed = mRotAngle = (GLuint)0;
}

Object::~Object(){
	
}

void Object::SetPosition(vec3 position){
	mPosition = position;
}

void Object::SetScale(vec3 scale){
	mScale = scale;
}

void Object::Rotate(float rotAngle, vec3 rotAxis){
    mRotAngle = rotAngle;
    mRotAxis = rotAxis;
}

void Object::RotateOverTime(float rotSpeed, vec3 rotAxis){
    mRotSpeed = rotSpeed;
    mRotAxis = rotAxis;
}

vec3 Object::GetPosition(){
	return mPosition;
}

void Object::Update(const float& deltaTime){
	mRotAngle += mRotSpeed * deltaTime;
}

void Object::Render(const Camera& camera){
	mat4 modelMatrix = BeforeRender();
	mat4 MVPMatrix = camera.projectionMatrix * camera.viewMatrix * modelMatrix;

	glUniformMatrix4fv(camera.MVPMatrixID, 1, GL_FALSE, &MVPMatrix[0][0]);
    
    glDrawArrays(mRenderMode, 0, mNumIndices);	//GL_TRIANGLE_STRIP or GL_TRIANGLES
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Object::SaveObjectState(char *message){
	if(!mObjectState)
		mObjectState = (Object*)malloc(sizeof(*this));

	*mObjectState = *this;
	puts(message);
}

void Object::LoadObjectState(char *message){
	*this = *mObjectState;
	puts(message);
}

GLuint Object::LoadBMP(const char * imagepath){
	// Data read from the header of the BMP file
	unsigned char header[54];	// Each BMP file begins by a 54-bytes header
	unsigned int dataPos;		// Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;		// = width*height*3
	unsigned char * data;		// Actual RGB data

	// Open the file
	const char* mode = "rb";
    
    char path[128];
    strcpy (path, RESOURCE_PATH);
    strcat (path, imagepath);
    
    #ifdef _WIN32	//keep windows from complaining…
        FILE * file = nullptr;
        if(fopen_s(&file, path, mode)){
            printf("File could not be opened\n");
            return 0;
        }
    #else
        FILE * file = fopen(path, mode);
    #endif
    
	if (!file){
		printf("Image could not be opened\n");
		return 0;
	}

	// If not 54 bytes read : problem
	if(fread(header, 1, 54, file)!=54 ){	
		printf("Not a correct BMP file\n");
		return false;
	}

	//Checking to see if first two bytes are 'B' and 'M'
	if ( header[0] != 'B' || header[1] != 'M' ){
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if(imageSize==0)    
		imageSize=width*height*3;	// 3 : one byte for each Red, Green and Blue component
	if(dataPos==0)
		dataPos=54;					// The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];
 
	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);
 
	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	glGenTextures(1, &mTextureID);
 
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, mTextureID);
 
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return mTextureID;
}

mat4 Object::BeforeRender(){
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);

	//Setting up vertices...
	glVertexAttribPointer(
		0,			//attribute layout
		3,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		0,			//Stride...
		(void*)0	//Array buffer offset...
	);

	if(mUvID > 0){
		//Setting up uvs...
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, mUvID);

		glVertexAttribPointer(
			1,			//attribute layout
			2,			//Elements in array
			GL_FLOAT,	//Each element is of type float
			GL_FALSE,	//Normalized?
			0,			//Stride...
			(void*)0	//Array buffer offset...
		);
	}

	if(mNormID > 0){
		//Setting up normals...
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, mNormID);

		glVertexAttribPointer(
			2,			//attribute layout
			3,			//Elements in array
			GL_FLOAT,	//Each element is of type float
			GL_FALSE,	//Normalized?
			0,			//Stride...
			(void*)0	//Array buffer offset...
		);
	}

//	glDrawArrays(mRenderMode, 0, mNumIndices);	//GL_TRIANGLE_STRIP or GL_TRIANGLES
//
//	glDisableVertexAttribArray(0);
//	glDisableVertexAttribArray(1);
//	glDisableVertexAttribArray(2);

	//Every object starts off with an identity matrix...
	/*mat4 objectMatrix = mat4(1.0f);
	mat4 identityMatrix = glm::scale(objectMatrix, scale);	
	mat4 modelMatrix = translate(identityMatrix, position);*/

	mat4 identityMatrix = mat4(1.0f);	//model in object space
	mat4 scaleMatrix = glm::scale(identityMatrix, mScale);

	/*mat4 translateMatrix = glm::translate(scaleMatrix, position);
	mat4 modelMatrix = glm::rotate(translateMatrix, rotAngle, vec3(0.0f, 1.0f, 0.0f));
*/
	mat4 translateMatrix = glm::translate(identityMatrix, mPosition);
	mat4 rotationMatrix = glm::rotate(identityMatrix, mRotAngle, mRotAxis);
	mat4 modelMatrix = translateMatrix * rotationMatrix * scaleMatrix * identityMatrix;

	//(model in world space) = translateMatrix * scale * (model in object space)

	return modelMatrix;
}