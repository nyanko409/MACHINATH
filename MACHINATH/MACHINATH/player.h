#pragma once

#include "gameObject.h"
#include "mesh.h"

// player class
class Player : public BoneObject, public BoxCollider
{
public:
	float moveSpeed;
	float sideSpeed;

	// constructor
	Player(Transform transform, float moveSpeed, float sideSpeed, ANIMATED_MESH_NAME name, SHADER_TYPE type, float width, float height, float depth, GameObject* parent = nullptr) :
		BoneObject(transform, name, type, parent), BoxCollider(this, width, height, depth),  moveSpeed(moveSpeed), sideSpeed(sideSpeed)
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
