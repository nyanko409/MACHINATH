#include "material.h"
#include "mydirect3d.h"

void SetMaterial()
{
	auto device = MyDirect3D_GetDevice();

	D3DMATERIAL9 mat;
	ZeroMemory(&mat, sizeof(D3DMATERIAL9));

	mat.Diffuse = D3DXCOLOR(1.0F, 1.0F, 1.0F, 1.0F);
	mat.Ambient = D3DXCOLOR(1.0F, 1.0F, 1.0F, 1.0F);
	//mat.Specular = D3DXCOLOR(0.0F, 1.0F, 1.0F, 1.0F);
	//mat.Emissive = D3DXCOLOR(1.0F, 1.0F, 1.0F, 1.0F);

	device->SetMaterial(&mat);
}

void SetMaterial(D3DMATERIAL9* mat)
{
	auto device = MyDirect3D_GetDevice();
	device->SetMaterial(mat);
}
