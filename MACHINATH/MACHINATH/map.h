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

	Map(Transform transform, MESH_NAME name, Direction exit, int localYRot, SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) :
		MeshObject(transform, name, type, parent), exit(exit)
	{
		// init rotation
		rotateAtPos = false;
		fixedRot = D3DXVECTOR3(0, localYRot, 0);
	}

	~Map() {}
};


void InitMap();
void UninitMap();
void UpdateMap();
