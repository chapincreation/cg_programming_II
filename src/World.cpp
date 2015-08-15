#include "World.h"

//TODO: Add functionality later...
World::World(){
    plane = new Plane[1];
    plane[0].BuildTriangles(1, 1);
	//plane[0].SetScale(vec3(0.15f));
	//plane[0].SetPosition(vec3(-1.0f, -1.0f, 0.0f));

	cube = new Cube[1];
	cube[0].SetScale(vec3(0.5f));
	cube[0].SetPosition(vec3(0.0f, 10.0f, 0.0f));
    cube[0].Rotate(45, vec3(0.0f, 1.0f, 0.0f));
    
    mesh = new Mesh[1];
	//mesh[0].BuildMesh("bunny.obj");
    //mesh[0].BuildMesh("bunny_large.obj");
	mesh[0].BuildMesh("head2.obj");
    mesh[0].SetPosition(vec3(0.0f, -1.5f, 20.0f));
    mesh[0].RotateOverTime(1.0f, vec3(0, 1, 0));

	(void)mesh[0].LoadBMP("dirt.bmp");
    (void)cube[0].LoadBMP("dirt.bmp");
	//GLuint textureID = plane[0].LoadBMP("test.bmp");
	//GLuint textureID = plane[0].LoadBMP("dirt.bmp");
	//GLuint textureID = plane[0].LoadBMP("world.bmp");
}

//TODO: Add functionality later...
World::~World(){
	delete[] plane;
	plane = NULL;

	delete[] cube;
	cube = NULL;
    
    delete[] mesh;
    mesh = NULL;
}

void World::Update(const float& deltaTime){

	//plane[0].Update(deltaTime);
	cube[0].Update(deltaTime);
    mesh[0].Update(deltaTime);
}

void World::Render(const Camera& camera){
	//plane[0].Render(camera);
	glCullFace(GL_FRONT);
	cube[0].Render(camera);

	glCullFace(GL_BACK);
    mesh[0].Render(camera);
}