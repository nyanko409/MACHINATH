#include "common.h"
#include "mesh.h"
#include "map.h"
#include "gate.h"
#include "transform.h"
#include "texture.h"
#include "sound.h"


class Gate : public MeshObject
{
public:
	float dissolveValue;

	Gate(Transform transform, MESH_NAME name, SHADER_TYPE shader, GameObject* parent) :
		MeshObject(transform, name, shader, parent), dissolveValue(1)
	{}

	// override draw from meshobject
	void Draw() override;
};

void Gate::Draw()
{
	if (enableDraw)
	{
		MyDirect3D_GetDevice()->SetTexture(1, Texture_GetTexture(TEXTURE_INDEX_DISSOLVE));

		dissolveValue -= 0.006F;
		pShader->SetFloat("value", dissolveValue);
	}

	MeshObject::Draw();
}



static Gate* g_gateLeft;
static Gate* g_gateRight;
static bool g_open;


void InitGate()
{
	// get the map to attach the gates
	Map* map = GetMap()->front();
	if (!map->data.name == MESH_NAME::MESH_MAP_START) return;

	g_open = false;

	g_gateLeft = new Gate(Transform(), MESH_MAP_START_GATE_LEFT, SHADER_DISSOLVE, map);
	g_gateRight = new Gate(Transform(), MESH_MAP_START_GATE_RIGHT, SHADER_DISSOLVE, map);
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
		
		StopSound(AUDIO_SE_GATE_OPEN);
	}
}


void OpenGate()
{
	g_open = true;
	PlaySound(AUDIO_SE_GATE_OPEN,3.0f);
}
