#pragma once

#include "mesh.h"

// pickup class - medals, powerups etc
class Pickup : public MeshObject, public BoxCollider
{
public:
	// constructor
	Pickup(Transform trans, MESH_NAME mesh, float width, float height, float depth) :
		MeshObject(trans, mesh), BoxCollider(&(MeshObject::transform), width, height, depth) {}
	
	// destructor
	~Pickup() {}
};


void InitPickup();
void UninitPickup();
void UpdatePickup();
void DrawPickup();
