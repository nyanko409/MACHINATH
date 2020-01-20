#pragma once

#include "collider.h"
#include "mesh.h"

// player class
class Player : public BoneObject
{
public:
	BoxCollider col;
	float moveSpeed;
	float sideSpeed;
	float heightOffset;
	bool isJumping;

	// constructor
	Player(Transform transform, float moveSpeed, float sideSpeed, float heightOffset, ANIMATED_MESH_NAME name, SHADER_TYPE type, float width, float height, float depth, GameObject* parent = nullptr) :
		BoneObject(transform, name, type, parent), col(BoxCollider(this, width, height, depth)),  
		moveSpeed(moveSpeed), sideSpeed(sideSpeed), heightOffset(heightOffset), isJumping(false)
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
