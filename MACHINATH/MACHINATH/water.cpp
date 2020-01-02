#include "water.h"
#include "texture.h"
#include "playTime.h"


void Water::Draw()
{
	auto device = MyDirect3D_GetDevice();

	device->SetTexture(1, Texture_GetTexture(TEXTURE_INDEX_WATER));
	pShader->SetFloat("time", playTime * 0.1F);

	MeshObject::Draw();
}
