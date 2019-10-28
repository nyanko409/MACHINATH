#pragma once

#include <string>


// add mesh name to this enum
enum MESH_NAME
{
	MESH_FLOOR,
	MESH_EGG,
	MESH_COIN,
	MESH_SLIME,
	MESH_BLOCK,
	MESH_ROBOT
};

// mesh data struct
struct MESH_DATA
{
	std::string meshPath;
	LPD3DXMESH mesh;
	D3DMATERIAL9* material;
	LPDIRECT3DTEXTURE9* texture;
	DWORD numMaterial;
};

// call once
void LoadMesh();

// get mesh
MESH_DATA* GetMesh(MESH_NAME name);
