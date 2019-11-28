#pragma once

#include "mesh.h"


// exit direction of the map
enum class Direction
{
	NORTH, EAST, SOUTH, WEST
};

// map class
class Map : public MeshObject
{
public:
	Direction exit;

	Map(Transform transform, MESH_NAME name, Direction exit, SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) :
		MeshObject(transform, name, type, parent), exit(exit)
	{
		// disable draw
		enableDraw = false;
	}

	// return forward vector
	D3DXVECTOR3 GetForward()
	{
		// create y rotation matrix
		D3DXMATRIX rotY;
		D3DXMatrixRotationY(&rotY, D3DXToRadian(-transform.rotation.y));

		// apply it to forward vector and return
		D3DXVECTOR3 f = D3DXVECTOR3(0, 0, 1);
		D3DXVECTOR3 temp;
		D3DXVec3TransformCoord(&temp, &f, &rotY);
		return temp;
	}

	~Map() {}
};


std::vector<Map*>* GetMap();

void InitMap();
void UninitMap();
void UpdateMap();
