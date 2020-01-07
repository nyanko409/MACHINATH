#include "lighting.h"
#include "light.h"


void InitLighting()
{
	AddDirectionalLight(0, D3DXVECTOR3(0.0F, -1.0F, 0.0F), D3DXCOLOR(0.0F, 1.0F, 1.0F, 1.0F));
}

void UninitLighting()
{

}

void UpdateLighting()
{

}
