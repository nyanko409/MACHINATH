#include "lighting.h"
#include "light.h"


void InitLighting()
{
	AddDirectionalLight(0, D3DXVECTOR3(0.0F, -1.0F, 0.0F), D3DXCOLOR(0.0F, 1.0F, 1.0F, 1.0F));
	AddPointLight(1, { 0, 20, 0 }, 5000, D3DXCOLOR(0, 1, 1, 1));
}

void UninitLighting()
{

}

void UpdateLighting()
{

}
