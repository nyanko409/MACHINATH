#pragma once

#include "gameObject.h"
#include "mesh.h"

// player class
class Player : public MeshObject, public BoxCollider
{
public:
	float moveSpeed;

	// constructor
	Player(Transform transform, float moveSpeed, MESH_NAME name, SHADER_TYPE type, float width, float height, float depth, GameObject* parent = nullptr) :
		MeshObject(transform, name, type, parent), BoxCollider(&(GameObject::transform), width, height, depth),  moveSpeed(moveSpeed)
	{}

	// destructor
	~Player() {}

	// draw
	void Draw() override;
};


void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
Player* GetPlayer();
