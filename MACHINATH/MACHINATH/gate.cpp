#include "common.h"
#include "mesh.h"
#include "map.h"
#include "gate.h"


static MeshObject* g_gateLeft;
static MeshObject* g_gateRight;
static bool g_open;


void InitGate()
{
	// get the map to attach the gates
	Map* map = GetMap()->front();
	if (!map->data.name == MESH_NAME::MESH_MAP_START) return;

	g_open = false;

	g_gateLeft = new MeshObject(Transform(), MESH_MAP_START_GATE_LEFT, SHADER_DEFAULT, map);
	g_gateRight = new MeshObject(Transform(), MESH_MAP_START_GATE_RIGHT, SHADER_DEFAULT, map);
}

void UninitGate()
{
	SAFE_DELETE(g_gateLeft);
	SAFE_DELETE(g_gateRight);
}

void UpdateGate()
{
	if (!g_open) return;

	// open the gate
	g_gateLeft->transform.position.x -= 0.2F;
	g_gateRight->transform.position.x += 0.2F;

	// stop the gate
	if (g_gateLeft->transform.position.x <= -50)
	{
		g_open = false;

		g_gateLeft->transform.position.x = -50;
		g_gateRight->transform.position.x = 50;
	}
}


void OpenGate()
{
	g_open = true;
}
