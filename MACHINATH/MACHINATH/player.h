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

	// return forward vector
	D3DXVECTOR3 GetForward()
	{
		// create x and y rotation matrix
		D3DXMATRIX rotX, rotY;
		D3DXMatrixRotationX(&rotX, D3DXToRadian(transform.rotation.x));
		D3DXMatrixRotationY(&rotY, D3DXToRadian(transform.rotation.y));

		// apply it to forward vector and return
		D3DXVECTOR3 f = D3DXVECTOR3(-1, 0, 0);
		D3DXVECTOR3 temp;
		D3DXVec3TransformCoord(&temp, &f, &(rotX * rotY));
		return temp;
	}
};


void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
Player* GetPlayer();
