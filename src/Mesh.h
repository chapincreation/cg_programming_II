#include "Application.h"
#include "Object.h"

#pragma once
typedef std::vector < glm::vec3 > stdVec3;
typedef std::vector < glm::vec2 > stdVec2;
typedef const char* cCharPtr;

class Mesh : public Object{
    
public:
	Mesh(){}
    Mesh(const char* objName);
    virtual ~Mesh();
    
public:
    void BuildMesh(const char* objName);
    
private:
    bool loadOBJ(cCharPtr path, stdVec3 &out_vertices, stdVec2 &out_uvs, stdVec3 &out_normals);
};