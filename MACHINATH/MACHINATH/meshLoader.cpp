#include <d3dx9.h>
#include <vector>
#include "mydirect3d.h"
#include "texture.h"
#include "meshLoader.h"

// unanimated mesh data, add path to x file
std::vector<MESH_DATA> g_mesh
{
	{"asset/mesh/medal_gold.x"},
	{"asset/mesh/skateboard.x" },
	{"asset/mesh/map_straight.x" },
	{"asset/mesh/map_curve_left.x" },
	{"asset/mesh/map_curve_right.x" },
	{"asset/mesh/map_straight_bridge.x" },
	{"asset/mesh/map_straight_up.x" },
	{"asset/mesh/map_straight_tunnel_down.x" }
};

// animated mesh data, add path to x file
std::vector<std::string> g_boneMeshPath
{
	{"asset/mesh/robot.x"}
};

// animated mesh data
std::vector<SkinMeshFile*> g_boneMesh;




void LoadMesh()
{
	// get device
	auto device = MyDirect3D_GetDevice();

	// load every unanimated mesh
	for (int i = 0; i < g_mesh.size(); i++)
	{
		// load mesh from path
		LPD3DXBUFFER matBuffer;
		D3DXLoadMeshFromX(g_mesh[i].meshPath.c_str(), D3DXMESH_SYSTEMMEM, device, NULL, &matBuffer, NULL, &g_mesh[i].numMaterial, &g_mesh[i].mesh);

		// set materials and textures
		D3DXMATERIAL* temp = (D3DXMATERIAL*)matBuffer->GetBufferPointer();
		g_mesh[i].material = new D3DMATERIAL9[g_mesh[i].numMaterial];
		g_mesh[i].texture = new LPDIRECT3DTEXTURE9[g_mesh[i].numMaterial];
		std::string path;

		// loop for every material and set material and texture
		for (DWORD j = 0; j < g_mesh[i].numMaterial; j++)
		{
			g_mesh[i].material[j] = temp[j].MatD3D;
			g_mesh[i].material[j].Ambient = g_mesh[i].material[j].Diffuse;

			if (temp[j].pTextureFilename != NULL)
			{
				path = "asset/texture/";
				path += temp[j].pTextureFilename;
			}
			else
				path = "";

		if (FAILED(D3DXCreateTextureFromFileA(device, path.c_str(), &g_mesh[i].texture[j])))
				g_mesh[i].texture[j] = NULL;
		}
	}

	// load every animated mesh
	g_boneMesh = std::vector<SkinMeshFile*>();
	for (int i = 0; i < g_boneMeshPath.size(); i++)
	{
		g_boneMesh.push_back(new SkinMeshFile);
		g_boneMesh[i]->Load(g_boneMeshPath[i].c_str());
	}
}

MESH_DATA* GetMesh(MESH_NAME name)
{
	return &g_mesh[name];
}

SkinMeshFile* GetAnimatedMesh(ANIMATED_MESH_NAME name)
{
	return g_boneMesh[name];
}
