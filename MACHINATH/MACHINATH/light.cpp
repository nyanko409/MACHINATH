#include "light.h"
#include "mydirect3d.h"


void AddDirectionalLight(int index, D3DXVECTOR3 direction, D3DXCOLOR diffuse)
{
	// get device and init light
	auto device = MyDirect3D_GetDevice();

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = direction;
	light.Diffuse = diffuse;
	light.Ambient = D3DXCOLOR(0.5F, 0.5F, 0.5F, 1.0F);

	device->SetLight(index, &light);
	device->LightEnable(index, true);
}

void AddSpotLight(int index, D3DXVECTOR3 position, float range, D3DXCOLOR diffuse)
{
	// get device and init light
	auto device = MyDirect3D_GetDevice();

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_SPOT;
	light.Position = position;
	light.Diffuse = diffuse;
	light.Ambient = D3DXCOLOR(0.5F, 0.5F, 0.5F, 1.0F);
	light.Range = range;
	light.Attenuation0 = 0.0F;
	light.Attenuation1 = 0.125F;
	light.Attenuation2 = 0.0F;

	device->SetLight(index, &light);
	device->LightEnable(index, true);
}

void AddPointLight(int index, D3DXVECTOR3 position, float range, D3DXCOLOR diffuse)
{
	// get device and init light
	auto device = MyDirect3D_GetDevice();

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_POINT;
	light.Position = position;
	light.Diffuse = diffuse;
	//light.Ambient = D3DXCOLOR(0.5F, 0.5F, 0.5F, 1.0F);
	light.Ambient = D3DXCOLOR(1.0F, 1.0F, 1.0F, 1.0F);
	light.Range = range;
	light.Attenuation0 = 0.0F;
	light.Attenuation1 = 0.125F;
	light.Attenuation2 = 0.0F;

	device->SetLight(index, &light);
	device->LightEnable(index, true);
}


void MoveLight(int index, float x, float y, float z, bool absolute)
{
	// get device and init light
	auto device = MyDirect3D_GetDevice();

	D3DLIGHT9 light;
	HRESULT hr = device->GetLight(index, &light);

	// return if invalid index is passed
	if (FAILED(hr)) return;

	// move light to the new position
	D3DXVECTOR3 result = D3DXVECTOR3(x, y, z);

	if (!absolute)
	{
		// move the light by given values
		D3DXVECTOR3 start = light.Position;
		D3DXMATRIX matTranslate;

		D3DXMatrixTranslation(&matTranslate, x, y, z);
		D3DXVec3TransformCoord(&result, &start, &matTranslate);
	}

	light.Position = result;

	// set updated light
	device->SetLight(index, &light);
}

void RotateLight(int index, float x, float y, float z, bool absolute)
{
	// get device and init light
	auto device = MyDirect3D_GetDevice();

	D3DLIGHT9 light;
	HRESULT hr = device->GetLight(index, &light);

	// return if invalid index is passed
	if (FAILED(hr)) return;

	// move light to the new position
	D3DXVECTOR3 result = D3DXVECTOR3(x, y, z);

	if (!absolute)
	{
		// move the light by given values
		D3DXVECTOR3 start = light.Position;
		D3DXMATRIX matRotX, matRotY, matRotZ;

		D3DXMatrixRotationX(&matRotX, D3DXToRadian(x));
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(y));
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(z));

		D3DXVec3TransformCoord(&result, &start, &(matRotX * matRotY * matRotZ));
	}

	light.Position = result;

	// set updated light
	device->SetLight(index, &light);
}
