#pragma once

#include "mesh.h"

// pickup class - medals, powerups etc
class Pickup : public MeshObject, public BoxCollider
{
public:
	// constructor
	Pickup(Transform trans, MESH_NAME mesh, SHADER_TYPE type, float width, float height, float depth) :
		MeshObject(trans, mesh, type), BoxCollider(&(MeshObject::transform), width, height, depth) {}
	
	// destructor
	~Pickup() {}

	// draw
	void Draw() override
	{
		MeshObject::Draw();
	}
};


void InitPickup();
void UninitPickup();
void UpdatePickup();
