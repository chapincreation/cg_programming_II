#include "Object.h"

Object::Object(){
	objectState = NULL;
	renderMode = GL_TRIANGLES;
	vertexBufferID = 0;
	position = vec3(0);
	numIndices = 6;
	numUVs = 0;
	textureID = NULL;
	SetPosition(vec3(0));
	SetScale(vec3(1));
	SetRotation(vec3(1, 0, 0));
}

Object::~Object(){
	
}

void Object::SetPosition(vec3 position){
	this->position = position;
}

void Object::SetScale(vec3 scale){
	this->scale = scale;
}

void Object::SetRotation(vec3 rotation){
	this->rotation = rotation;
}

vec3 Object::GetPosition(){
	return this->position;
}

vec3 Object::GetRotation(){
	return this->rotation;
}

float Object::GetLeftX(){
	return leftX;
}

float Object::GetRightX(){
	return rightX;
}

float Object::GetTopY(){
	return topY;
}

float Object::GetBottomY(){
	return bottomY;
}

void Object::Update(const float& deltaTime){
	leftX = this->position.x - scale.x;
	rightX = this->position.x + scale.x;
	topY = this->position.y + scale.y;
	bottomY = this->position.y - scale.y;
}

void Object::Render(const Camera& camera){
	mat4 modelMatrix = Render();
	mat4 MVPMatrix = camera.projectionMatrix * camera.viewMatrix * modelMatrix;

	glUniformMatrix4fv(camera.MVPMatrixID, 1, GL_FALSE, &MVPMatrix[0][0]);
}

void Object::BuildTriangles(const GLuint& perRow, const GLuint& perColumn){
	int numValuesPerRow = 18 * perRow;
	int numValues = 18 * perRow * perColumn;

	GLfloat *vertices = new GLfloat[numValues];
	for(int i = 0, x = 0, y = 0; i < numValues; ++x){
		vertices[i] = x;		vertices[++i] = y + 1;	vertices[++i] = 0;
		vertices[++i] = x;		vertices[++i] = y;		vertices[++i] = 0;
		vertices[++i] = x + 1;	vertices[++i] = y + 1;	vertices[++i] = 0;
		
		vertices[++i] = x + 1;	vertices[++i] = y;		vertices[++i] = 0;
		vertices[++i] = x + 1;	vertices[++i] = y + 1;	vertices[++i] = 0;
		vertices[++i] = x;		vertices[++i] = y;		vertices[++i] = 0;

		if(++i % numValuesPerRow == 0){
			x = -1; --y;
		}
	}

	int numUvValuesPerRow = 12 * perRow;
	int numUvValues = 12 * perRow * perColumn;

	GLfloat *uvs = new GLfloat[numUvValues];
	for(int i = 0, u = 0, v = 0; i < numUvValues; ++u){
		uvs[i] = u;			uvs[++i] = -v;
		uvs[++i] = u;		uvs[++i] = -v - 1;
		uvs[++i] = u + 1;	uvs[++i] = -v;
		
		uvs[++i] = u + 1;	uvs[++i] = -v - 1;
		uvs[++i] = u + 1;	uvs[++i] = -v;
		uvs[++i] = u;		uvs[++i] = -v - 1;

		if(++i % numUvValuesPerRow == 0){
			u = -1; ++v;
		}
	}

	numUVs = numUvValues/2;
	numIndices = numValues/3;
	this->renderMode = GL_TRIANGLES;
	LoadTriangles(vertices, uvs);
}

void Object::BuildTriangleStrip(const GLuint& perRow, const GLuint& perColumn){
	/*
		0, 1	1, 1	|	2, 1	3, 1
		0, 0	1, 0	|	2, 0	3, 0
						|
		0, 0	1, 0	|	2, 0 	3, 0
		0,-1	1,-1	|	2,-1	3,-1
	*/

	int numValuesPerRow = (2 + (2 * perRow)) *3, numDegenValues = 2 * 3;
	uint numValues = (numValuesPerRow + numDegenValues) * perColumn;

	GLfloat *vertices = new GLfloat[numValues];
	for(int i = 0, x = 0, y = 0; i < numValues; ++x){
		vertices[i] = x;		vertices[++i] = y + 1;	vertices[++i] = 0;
		vertices[++i] = x;		vertices[++i] = y;		vertices[++i] = 0;

		if((++i) % numValuesPerRow == 0){
			vertices[i] = x;	vertices[++i] = y;	vertices[++i] = 0;
			vertices[++i] = x;		vertices[++i] = y;	vertices[++i] = 0;
			numValuesPerRow += numDegenValues;
			i +=1;
			x = -1; --y;
		}
	}

	/*GLfloat *vertices = new GLfloat[numVertices];
	vertices[0] = 0.0f; vertices[1] = 1.0f; vertices[2] = 0.0f;
	vertices[3] = 0.0f; vertices[4] = 0.0f; vertices[5] = 0.0f;
	vertices[6] = 1.0f; vertices[7] = 1.0f; vertices[8] = 0.0f;
	vertices[9] = 1.0f; vertices[10] = 0.0f; vertices[11] = 0.0f;

	vertices[12] = 1.0f; vertices[13] = 0.0f; vertices[14] = 0.0f;
	vertices[15] = 1.0f; vertices[16] = 0.0f; vertices[17] = 0.0f;

	vertices[18] = 0.0f; vertices[19] = 0.0f; vertices[20] = 0.0f;
	vertices[21] = 0.0f; vertices[22] = -1.0f; vertices[23] = 0.0f;
	vertices[24] = 1.0f; vertices[25] = 0.0f; vertices[26] = 0.0f;
	vertices[27] = 1.0f; vertices[28] = -1.0f; vertices[29] = 0.0f;*/

	numIndices = numValues/3;
	this->renderMode = GL_TRIANGLE_STRIP;
	LoadTriangles(vertices, vertices);
}


void Object::BuildCube() {
// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	
	static const GLfloat vertices[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};
	
	this->renderMode = GL_TRIANGLES;
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares
}

void Object::LoadTriangles(GLfloat *vertices, GLfloat *uvs){
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, numIndices * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &uvID);
	glBindBuffer(GL_ARRAY_BUFFER, uvID);
	glBufferData(GL_ARRAY_BUFFER, numUVs * 2 * sizeof(GLfloat), uvs, GL_STATIC_DRAW);
}

void Object::SaveObjectState(char *message){
	if(objectState == NULL)
		objectState = (Object*)malloc(sizeof(*this));

	*objectState = *this;
	puts(message);
}

void Object::LoadObjectState(char *message){
	*this = *objectState;
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
	FILE * file = fopen(imagepath, mode);
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
	glGenTextures(1, &textureID);
 
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
 
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureID;
}


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint Object::LoadDDS(const char * imagepath){
 
    unsigned char header[124];
 
    FILE *fp;
 
    // try to open the file 
    fp = fopen(imagepath, "rb");
    if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); 
		return 0;
	}
    // verify the type of file 
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        return 0;
    }
 
    // get the surface desc //
    fread(&header, 124, 1, fp); 
 
    unsigned int height      = *(unsigned int*)&(header[8 ]);
    unsigned int width         = *(unsigned int*)&(header[12]);
    unsigned int linearSize     = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC      = *(unsigned int*)&(header[80]);
	unsigned char * buffer;
    unsigned int bufsize;
    // how big is it going to be including all mipmaps? 
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    // close the file pointer 
    fclose(fp);
	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch(fourCC)
    {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    default:
        free(buffer);
        return 0;
    }
	// Create one OpenGL texture
   
    glGenTextures(1, &textureID);
 
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;
 
    // load the mipmaps 
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 
            0, size, buffer + offset);
 
        offset += size;
        width  /= 2;
        height /= 2;
    }
    free(buffer); 

	// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
	//if(width < 1) width = 1;
	//if(height < 1) height = 1;

    return textureID;
}


mat4 Object::Render(){
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glVertexAttribPointer(
		0,			//attribute layout
		3,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		0,			//Stride...
		(void*)0	//Array buffer offset...
	);

	//Rendering UVs...
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvID);

	glVertexAttribPointer(
		1,			//attribute layout
		2,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		0,			//Stride...
		(void*)0	//Array buffer offset...
	);

	glDrawArrays(renderMode, 0, numIndices);	//GL_TRIANGLE_STRIP or GL_TRIANGLES

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//Every object starts off with an identity matrix...
	/*mat4 objectMatrix = mat4(1.0f);
	mat4 identityMatrix = glm::scale(objectMatrix, scale);	
	mat4 modelMatrix = translate(identityMatrix, position);
	mat4 transform = mat2 * mat1;
	vec4 out_vec = transform * in_vec;*/
	float angle = 10.0f;
	glm::mat4 myIdentityMatrix = glm::mat4(1.0f);
	//glm::mat4 myScalingMatrix = glm::scale(2.0f, 2.0f ,2.0f);
	//glm::vec3 myRotationAxis( ??, ??, ??);
	//glm::rotate( angle_in_degrees, myRotationAxis );

	mat4 identityMatrix = mat4(1.0f);
	mat4 translateMatrix = translate(identityMatrix, position);
	
	mat4 modelMatrix = glm::scale(translateMatrix, scale);
	mat4 rotationMatrix = glm::rotate(modelMatrix, angle, rotation);
	   //rotate object angle degree around X-axis
	return rotationMatrix;
}






//void Object::LoadTriangles(const GLuint& perRow, const GLuint& perColumn, const GLenum& renderMode){

	/*
		0,1   1,1
		0,0   1,0

		0,0   1,0
		0,-1  1,-1
	
	static const GLfloat vertexBuffer[] = {
		0.0f, 1.0f,  0.0f,	//maps to index 0
		0.0f, 0.0f, 0.0f,	//maps to index 1
		1.0f, 1.0f,  0.0f,	//maps to index 2
		1.0f, 0.0f, 0.0f,	//maps to index 3

		1.0f, 0.0f, 0.0f,	//maps to index 4
		0.0f, 0.0f, 0.0f,	//maps to index 5

		0.0f, 0.0f, 0.0f,	//maps to index 6
		0.0f, -1.0f, 0.0f,	//maps to index 7
		1.0f, 0.0f, 0.0f,	//maps to index 8
		1.0f, -1.0f, 0.0f	//maps to index 9
	};

	numIndices = 10;		//For a total of 10 indices
	
	this->renderMode = renderMode;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);

	// UV attempt
	static const GLfloat UV_Buffer[] = {
		0.0f, 1.0f,	//maps to index 0
		0.0f, 0.0f,	//maps to index 1
		1.0f, 1.0f,	//maps to index 2
		1.0f, 0.0f,	//maps to index 3

		1.0f, 0.0f,	//maps to index 4
		0.0f, 0.0f,	//maps to index 5

		0.0f, 0.0f,	//maps to index 6
		0.0f, -1.0f,	//maps to index 7
		1.0f, 0.0f,	//maps to index 8
		1.0f, -1.0f	//maps to index 9
	};
	glGenBuffers(1, &UV_BufferID);
	glBindBuffer(GL_ARRAY_BUFFER, UV_BufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UV_Buffer), UV_Buffer, GL_STATIC_DRAW);

}

void Object::SaveObjectState(char *message){
	if(objectState == NULL)
		objectState = (Object*)malloc(sizeof(*this));

	*objectState = *this;
	puts(message);
}

void Object::LoadObjectState(char *message){
	*this = *objectState;
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
	FILE * file = fopen(imagepath, mode);
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
	glGenTextures(1, &textureID);
 
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
 
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureID;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint Object::LoadDDS(const char * imagepath){
 
    unsigned char header[124];
 
    FILE *fp;
 
    // try to open the file 
    fp = fopen(imagepath, "rb");
    if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); 
		return 0;
	}
    // verify the type of file 
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        return 0;
    }
 
    // get the surface desc //
    fread(&header, 124, 1, fp); 
 
    unsigned int height      = *(unsigned int*)&(header[8 ]);
    unsigned int width         = *(unsigned int*)&(header[12]);
    unsigned int linearSize     = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC      = *(unsigned int*)&(header[80]);
	unsigned char * buffer;
    unsigned int bufsize;
    // how big is it going to be including all mipmaps? 
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    // close the file pointer 
    fclose(fp);
	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch(fourCC)
    {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    default:
        free(buffer);
        return 0;
    }
	// Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
 
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;
 
    // load the mipmaps 
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 
            0, size, buffer + offset);
 
        offset += size;
        width  /= 2;
        height /= 2;
    }
    free(buffer); 

	// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
	//if(width < 1) width = 1;
	//if(height < 1) height = 1;

    return textureID;
}

mat4 Object::Render(){
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glVertexAttribPointer(
		0,			//attribute layout
		3,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		0,			//Stride...
		(void*)0	//Array buffer offset...
	);

	glDrawArrays(renderMode, 0, numIndices);	//GL_TRIANGLE_STRIP or GL_TRIANGLES
	glDisableVertexAttribArray(0);

	//Every object starts off with an identity matrix...
	
	mat4 identityMatrix = mat4(1.0f);
	mat4 translateMatrix = translate(identityMatrix, position);
	mat4 modelMatrix = glm::scale(translateMatrix, scale);

	return modelMatrix;
}
*/
//static mat4 Render(GLuint vertexBuffer, const vec3& position, const vec3& scale);