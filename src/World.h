#include "Plane.h"
#include "Cube.h"
#include "Mesh.h"

#pragma once

class World{

	public:
		World();
		virtual ~World();

		void Update(const float& deltaTime);
		void Render(const Camera& camera);
		
	private:
		Plane* plane;
		Cube* cube;
        Mesh* mesh;
};