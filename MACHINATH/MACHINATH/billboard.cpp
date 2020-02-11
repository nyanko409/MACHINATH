#include "common.h"
#include "billboard.h"
#include "texture.h"


#define PARTICLE_COUNT 30

BillBoard g_particle[PARTICLE_COUNT];
float g_lifeCycleSpeed;
float g_particleSpawnInterval;

float g_curSpawnInterval;


void InitBillboard()
{
	g_particleSpawnInterval = 0.05F;
	g_lifeCycleSpeed = 0.2F;

	g_curSpawnInterval = 0;

	// init
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		g_particle[i] = BillBoard();
	}
}

void UninitBillboard()
{

}

void UpdateBillboard()
{
	// activate particle
	g_curSpawnInterval += TIME_PER_FRAME;
	if (g_curSpawnInterval >= g_particleSpawnInterval)
	{
		g_curSpawnInterval -= g_particleSpawnInterval;
		for (int i = 0; i < PARTICLE_COUNT; ++i)
		{
			if (g_particle[i].isActive) continue;
			else
			{
				g_particle[i] = BillBoard();
				g_particle[i].transform.scale = D3DXVECTOR3(0.4F, 0.4F, 0.4F);
				g_particle[i].xVel = ((rand() % 21) - 10) / 20.0F;
				g_particle[i].zVel = ((rand() % 21) - 10) / 20.0F;
	
				g_particle[i].isActive = true;
				break;
			}
		}
	}

	// loop for every particle
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		// return if particle is not active
		if (!g_particle[i].isActive) continue;

		g_particle[i].lifeCycle -= g_lifeCycleSpeed;
		g_particle[i].transform.position.x += g_particle[i].xVel * g_particle[i].speed;
		g_particle[i].transform.position.y += sinf((g_particle[i].yVel++) / 4) * 2 * g_particle[i].speed;
		g_particle[i].transform.position.z += g_particle[i].zVel * g_particle[i].speed;

		// disable particle if lifecycle <= 0
		if (g_particle[i].lifeCycle <= 0)
			g_particle[i].isActive = false;
	}
}

void DrawBillboard()
{
	auto pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX matScale, matRot, matTranslate;

	// get inverse view matrix
	pDevice->GetTransform(D3DTS_VIEW, &matRot);
	matRot._14 = matRot._24 = matRot._34 = 0;
	matRot._41 = matRot._42 = matRot._43 = 0;
	D3DXMatrixTranspose(&matRot, &matRot);

	D3DXMATRIX yRot, zRot;
	D3DXMatrixRotationY(&yRot, D3DXToRadian(180));
	D3DXMatrixRotationZ(&zRot, D3DXToRadian(-90));
	//matRot *= yRot;
	//matRot *= yRot;

	pDevice->SetTexture(NULL, Texture_GetTexture(TEXTURE_INDEX_FUCK));

	// loop for every particle
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		// return if particle is not active
		if (!g_particle[i].isActive) continue;

		// translation matrix
		D3DXMatrixTranslation(&matTranslate,
			g_particle[i].transform.position.x, g_particle[i].transform.position.y, g_particle[i].transform.position.z);

		// scaling matrix
		D3DXMatrixScaling(&matScale,
			g_particle[i].transform.scale.x, g_particle[i].transform.scale.y, g_particle[i].transform.scale.z);

		CUSTOM_VERTEX vert[] =
		{
			{5, 5, 0, D3DXVECTOR3(0, 0, -1), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR2(0.0F, 1.0F)},
			{5, -5, 0, D3DXVECTOR3(0, 0, -1), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR2(1.0F, 1.0F)},
			{-5, 5, 0, D3DXVECTOR3(0, 0, -1), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR2(0.0F, 0.0F)},
			{-5, -5, 0, D3DXVECTOR3(0, 0, -1), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR2(1.0F, 0.0F)},
		};

		// draw to screen
		pDevice->SetTransform(D3DTS_WORLD, &(matScale * matRot * matTranslate));
		pDevice->SetFVF(CUSTOM_FVF);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vert, sizeof(CUSTOM_VERTEX));
	}
}
