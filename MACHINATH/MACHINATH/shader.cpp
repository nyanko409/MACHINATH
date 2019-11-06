#include "shader.h"
#include "mydirect3d.h"


static LPD3DXEFFECT effect;
static D3DXHANDLE technique;


void InitShader()
{
	auto pDevice = MyDirect3D_GetDevice();
	static LPD3DXBUFFER errorlog;
	
	// load shader into memory
	D3DXCreateEffectFromFile(pDevice, "asset/shader/custom/vertex.fx", 0, 0, 
		D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, &effect, &errorlog);
	effect->FindNextValidTechnique(NULL, &technique);
}

void EffectStart()
{
	effect->Begin(NULL, NULL);
	effect->BeginPass(0);
}

void EffectEnd()
{
	effect->EndPass();
	effect->End();
}

void UninitShader()
{
	// free memory
	if(effect)
		effect->Release();
}

LPD3DXEFFECT GetEffect()
{
	return effect;
}
