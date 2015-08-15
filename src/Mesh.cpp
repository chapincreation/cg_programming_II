#include "Mesh.h"

Mesh::Mesh(const char* objName) : Object(){
    BuildMesh(objName);
}

Mesh::~Mesh(void){
    
}

void Mesh::BuildMesh(const char* objName){
    
    static bool built = false;
    
    if(built){
        fprintf(stderr, "Failed to build triangles. Object already built!\n");
        return;
    }
    
    built = true;
    
    // Read our .obj file
    stdVec3 vertices, normals;	//TODO: Use normals later.
    stdVec2 uvs;
    
    char path[128];
    strcpy (path, RESOURCE_PATH);
    strcat (path, objName);
    
    if(loadOBJ(path, vertices, uvs, normals)){
        mRenderMode = GL_TRIANGLES;
        mNumIndices = (GLuint)vertices.size();
        
        glGenBuffers(1, &mVertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		size_t uvSize = uvs.size(), normSize = normals.size();

		if(uvSize > 0){
			glGenBuffers(1, &mUvID);
			glBindBuffer(GL_ARRAY_BUFFER, mUvID);
			glBufferData(GL_ARRAY_BUFFER, uvSize * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		}

		if(normSize > 0){
			glGenBuffers(1, &mNormID);
			glBindBuffer(GL_ARRAY_BUFFER, mNormID);
			glBufferData(GL_ARRAY_BUFFER, normSize * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		}
    }
};

bool Mesh::loadOBJ(cCharPtr path, stdVec3 &out_vertices, stdVec2 &out_uvs, stdVec3 &out_normals){
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    stdVec3 temp_vertices, temp_normals;
    stdVec2 temp_uvs;
    
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }

	char line[256];
	char lineHeader[128];
	bool isQuad = true;
	while(fgets(line, sizeof(line), file)){
		sscanf(line, "%s", lineHeader);

		if(strcmp(lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
			sscanf(line, "%*s %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
			sscanf(line, "%*s %f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
			sscanf(line, "%*s %f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];

			int matches = sscanf(line, "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1],
				&uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
				isQuad = true;

			if(matches != 12){
				isQuad = false;
				matches = sscanf(line, "%*s %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1],
				&uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			
				if(matches != 9){
					matches = sscanf(line, "%*s %d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0],
						&vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);

					if(matches != 6){
						matches = sscanf(line, "%*s %d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);

						if(matches != 3){
							printf("File can't be read by our simple parser : ( Try exporting with other options\n");
							return false;
						}
					}
				}
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[isQuad ? 3 : 2]);
			if(isQuad) {
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[3]);
			}

			if(matches < 6){
				continue;
			}

			uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[isQuad ? 3 : 2]);
			if(isQuad) {
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				uvIndices.push_back(uvIndex[3]);
			}

			if(matches < 9){
				continue;
			}

			normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[isQuad ? 3 : 2]);
			if(isQuad) {
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[3]);
			}
		}
	}
    
    // For each vertex of each triangle
	for(unsigned int i=0, uvSize = (unsigned int)uvIndices.size(),
        normSize = (unsigned int)normalIndices.size(); i<vertexIndices.size(); ++i){
        out_vertices.push_back(temp_vertices[vertexIndices[i] - 1]);

		if(uvSize > 0)
			out_uvs.push_back(temp_uvs[uvIndices[i] - 1]);

		if(normSize > 0)
			out_normals.push_back(temp_normals[normalIndices[i] - 1]);
    }
	
    //TODO: don't hard code this
    return true;
}
