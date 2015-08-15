#include "Plane.h"

Plane::Plane(GLuint width, GLuint height) : Object(){
    mLeftX = mRightX = mTopY = mBottomY = 0;
	BuildTriangles(mWidth = width, mHeight = height);
}

Plane::~Plane(){
	
}

void Plane::Update(const float& deltaTime){
    
    mLeftX = mPosition.x - mScale.x;
    mRightX = mPosition.x + mScale.x;
    mTopY = mPosition.y + mScale.y;
    mBottomY = mPosition.y - mScale.y;
    
    Object::Update(deltaTime);
}

void Plane::BuildTriangles(const GLuint& perRow, const GLuint& perColumn){
    
    static bool built = false;
    
    if(built){
        fprintf(stderr, "Failed to build triangles. Object already built!\n");
        return;
    }
    
    built = true;
    
	int numValuesPerRow = 18 * perRow;
	int numValues = 18 * perRow * perColumn;

	GLfloat *vertices = new GLfloat[numValues];
	for(int i = 0, x = 0, y = 0; i < numValues; ++x){
		vertices[i] = x + 0.0f;		vertices[++i] = y + 1.0f;	vertices[++i] = 0.0f;
		vertices[++i] = x + 0.0f;	vertices[++i] = y + 0.0f;	vertices[++i] = 0.0f;
		vertices[++i] = x + 1.0f;	vertices[++i] = y + 1.0f;	vertices[++i] = 0.0f;
		
		vertices[++i] = x + 1.0f;	vertices[++i] = y + 0.0f;	vertices[++i] = 0.0f;
		vertices[++i] = x + 1.0f;	vertices[++i] = y + 1.0f;	vertices[++i] = 0.0f;
		vertices[++i] = x + 0.0f;	vertices[++i] = y + 0.0f;	vertices[++i] = 0.0f;

		if(++i % numValuesPerRow == 0){
			x = -1; --y;
		}
	}

	int numUvValuesPerRow = 12 * perRow;
	int numUvValues = 12 * perRow * perColumn;

	GLfloat *uvs = new GLfloat[numUvValues];
	for(int i = 0, u = 0, v = 0; i < numUvValues; ++u){
		uvs[i] = u + 0.625f;		uvs[++i] = -v - 0.9166f;
		uvs[++i] = u + 0.625f;		uvs[++i] = -v - 1.0f;
		uvs[++i] = u + 0.6875f;		uvs[++i] = -v - 0.9166f;
		
		uvs[++i] = u + 0.6875f;		uvs[++i] = -v - 1.0f;
		uvs[++i] = u + 0.6875f;		uvs[++i] = -v - 0.9166f;
		uvs[++i] = u + 0.625f;		uvs[++i] = -v - 1.0f;

		if(++i % numUvValuesPerRow == 0){
			u = -1; ++v;
		}
	}	

	mNumUVs = numUvValues/2;
	mNumIndices = numValues/3;
	mRenderMode = GL_TRIANGLES;
	LoadTriangles(vertices, uvs);
}

void Plane::LoadTriangles(GLfloat *vertices, GLfloat *uvs){
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, mNumIndices * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &mUvID);
	glBindBuffer(GL_ARRAY_BUFFER, mUvID);
	glBufferData(GL_ARRAY_BUFFER, mNumUVs * 2 * sizeof(GLfloat), uvs, GL_STATIC_DRAW);
}