#pragma once

#include "gameObject.h"

// pickup class - medals, powerups etc
class Pickup : public GameObject, public BoxCollider
{
public:
	// constructor
	Pickup(Transform trans, MESH_NAME mesh, float width, float height, float depth) :
		GameObject(trans, mesh), BoxCollider(&(GameObject::transform), width, height, depth) {}
	
	// destructor
	~Pickup() {}
};


void InitPickup();
void UninitPickup();
void UpdatePickup();
void DrawPickup();
