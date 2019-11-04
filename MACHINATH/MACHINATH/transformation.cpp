#include <d3dx9.h>
#include "transformation.h"
#include "mydirect3d.h"
#include "common.h"

D3DXMATRIX matTranslate, matRotation, matScale;
D3DXMATRIX matWorld;

D3DXMATRIX TransformObject(D3DXVECTOR3 translate, D3DXVECTOR3 rotate, D3DXVECTOR3 scale, bool rotateAtPosition)
{
	// get device
	auto device = MyDirect3D_GetDevice();

	// set translation and scaling matrix
	D3DXMatrixTranslation(&matTranslate, translate.x, translate.y, translate.z);
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	// set rotation matrix
	D3DXMATRIX xRot, yRot, zRot;

	D3DXMatrixRotationX(&xRot, D3DXToRadian(rotate.x));
	D3DXMatrixRotationY(&yRot, D3DXToRadian(rotate.y));
	D3DXMatrixRotationZ(&zRot, D3DXToRadian(rotate.z));

	matRotation = xRot * yRot * zRot;

	// calculate world matrix
	if (!rotateAtPosition)
		matWorld = (matScale * matTranslate * matRotation);
	else
		matWorld = (matScale * matRotation * matTranslate);

	// set and return the world matrix
	device->SetTransform(D3DTS_WORLD, &matWorld);

	return matWorld;
}

D3DXMATRIX TransformSprite(D3DXVECTOR3 translate, float rotZ, D3DXVECTOR2 scale, bool rotateAtPosition)
{
	// get device
	auto device = MyDirect3D_GetDevice();

	// set translation and scaling matrix
	D3DXMatrixTranslation(&matTranslate, translate.x, translate.y, translate.z);
	D3DXMatrixScaling(&matScale, scale.x, scale.y, 1.0F);

	// set rotation matrix
	D3DXMatrixRotationZ(&matRotation, D3DXToRadian(rotZ));

	if (!rotateAtPosition)
		matWorld = (matScale * matTranslate * matRotation);
	else
		matWorld = (matScale * matRotation * matTranslate);

	// return the world matrix
	return matWorld;
}
