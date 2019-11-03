#pragma once

#include "gameObject.h"
#include "mesh.h"

// player class
class Player : public BoneObject, public BoxCollider
{
public:
	float moveSpeed;

	// constructor
	Player(Transform transform, float moveSpeed, ANIMATED_MESH_NAME name, float width, float height, float depth, GameObject* parent = nullptr) :
		BoneObject(transform, name, parent), BoxCollider(&(GameObject::transform), width, height, depth),  moveSpeed(moveSpeed)
	{}

	// destructor
	~Player() {}
};


void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
void DrawPlayer();
Player* GetPlayer();
