#pragma once

#include "collider.h"
#include "mesh.h"
#include "map.h"

// player class
class Player : public BoneObject
{
public:
	BoxCollider col;
	float moveSpeed;
	float sideSpeed;
	float heightOffset;
	float jumpSpeed;
	float jumpHeight;
	bool isJumping;
	bool isMoving;
	bool isMovingSideways;
	float sideVelocity;

	// constructor
	Player(Transform transform, float moveSpeed, float sideSpeed, float heightOffset, ANIMATED_MESH_NAME name, SHADER_TYPE type, float width, float height, float depth, GameObject* parent = nullptr) :
		BoneObject(transform, name, type, parent), col(BoxCollider(this, width, height, depth)),  
		moveSpeed(moveSpeed), sideSpeed(sideSpeed), heightOffset(heightOffset), jumpSpeed(2.9F), jumpHeight(10),
		isJumping(false), isMoving(false), isMovingSideways(true), sideVelocity(0)
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
void QueueMapEvent(EventData data);
