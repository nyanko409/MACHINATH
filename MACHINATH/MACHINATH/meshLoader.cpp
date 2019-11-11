#include <d3dx9.h>
#include <vector>
#include "mydirect3d.h"
#include "texture.h"
#include "meshLoader.h"

// unanimated mesh data, add path to x file
std::vector<MESH_DATA> mesh
{
	{"asset/mesh/coin.x"},
	{"asset/mesh/skateboard.x" },
	{"asset/mesh/neoshinjuku.x" }
};

// animated mesh data, add path to x file
std::vector<std::string> boneMeshPath
{
	{"asset/mesh/robot.x"},
};

// animated mesh data
std::vector<SkinMeshFile*> boneMesh;




void LoadMesh()
{
	// get device
	auto device = MyDirect3D_GetDevice();

	// loop for every unanimated mesh
	for (int i = 0; i < mesh.size(); i++)
	{
		// load mesh from path
		LPD3DXBUFFER matBuffer;
		D3DXLoadMeshFromX(mesh[i].meshPath.c_str(), D3DXMESH_SYSTEMMEM, device, NULL, &matBuffer, NULL, &mesh[i].numMaterial, &mesh[i].mesh);

		// set materials and textures
		D3DXMATERIAL* temp = (D3DXMATERIAL*)matBuffer->GetBufferPointer();
		mesh[i].material = new D3DMATERIAL9[mesh[i].numMaterial];
		mesh[i].texture = new LPDIRECT3DTEXTURE9[mesh[i].numMaterial];
		std::string path;

		// loop for every material and set material and texture
		for (DWORD j = 0; j < mesh[i].numMaterial; j++)
		{
			mesh[i].material[j] = temp[j].MatD3D;
			mesh[i].material[j].Ambient = mesh[i].material[j].Diffuse;

			if (temp[j].pTextureFilename != NULL)
			{
				path = "asset/texture/";
				path += temp[j].pTextureFilename;
			}
			else
				path = "";

			if (FAILED(D3DXCreateTextureFromFileA(device, path.c_str(), &mesh[i].texture[j])))
				mesh[i].texture[j] = NULL;
		}
	}

	// for animated mesh
	boneMesh = std::vector<SkinMeshFile*>();
	for (int i = 0; i < boneMeshPath.size(); i++)
	{
		boneMesh.push_back(new SkinMeshFile);
		boneMesh[i]->Load(boneMeshPath[i].c_str());
	}
}

MESH_DATA* GetMesh(MESH_NAME name)
{
	return &mesh[name];
}

SkinMeshFile* GetAnimatedMesh(ANIMATED_MESH_NAME name)
{
	return boneMesh[name];
}
