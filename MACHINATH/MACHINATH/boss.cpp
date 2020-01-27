#include "boss.h"
#include "map.h"


void Boss::Draw()
{
	auto pDevice = MyDirect3D_GetDevice();
	
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	MeshObject::Draw();
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}



Boss* g_boss;


void InitBoss()
{
	Transform trans = Transform({ 0,50, -200 });
	g_boss = new Boss(trans, MESH_BOSS, SHADER_DEFAULT, GetMap()->back());
}

void UninitBoss()
{
	SAFE_DELETE(g_boss);
}

void UpdateBoss()
{

}
