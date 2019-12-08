#pragma once

#include "gameObject.h"
#include "mesh.h"

// player class
class Player : public BoneObject, public BoxCollider
{
public:
	float moveSpeed;

	// constructor
	Player(Transform transform, float moveSpeed, ANIMATED_MESH_NAME name, SHADER_TYPE type, float width, float height, float depth, GameObject* parent = nullptr) :
		BoneObject(transform, name, type, parent), BoxCollider(this, width, height, depth),  moveSpeed(moveSpeed)
	{}

	// destructor
	~Player() {}

	// draw
	void Draw() override;

	// return forward vector
	D3DXVECTOR3 GetForward(int yOffset = 0)
	{
		// create y rotation matrix
		D3DXMATRIX rotY;
		D3DXMatrixRotationY(&rotY, D3DXToRadian(transform.localRotation.y + yOffset));

		// apply it to forward vector and return
		D3DXVECTOR3 f = D3DXVECTOR3(0, 0, 1);
		D3DXVECTOR3 temp;
		D3DXVec3TransformCoord(&temp, &f, &rotY);
		return temp;
	}
};


void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
Player* GetPlayer();
