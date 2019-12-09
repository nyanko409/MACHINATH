#pragma once

#include "gameObject.h"
#include "mesh.h"

// fog class
class Fog : public MeshObject, public BoxCollider
{
public:
	float moveSpeed;

	// constructor
	Fog(Transform transform, float moveSpeed, MESH_NAME name, SHADER_TYPE type, float width, float height, float depth, GameObject* parent = nullptr) :
		MeshObject(transform, name, type, parent), BoxCollider(this, width, height, depth), moveSpeed(moveSpeed)
	{}

	// destructor
	~Fog() {}

	// draw
	void Draw() override;
};


void InitFog();
void UninitFog();
void UpdateFog();
Fog* GetFog();

