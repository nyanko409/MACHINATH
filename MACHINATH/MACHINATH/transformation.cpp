#include <d3dx9.h>
#include "transformation.h"
#include "mydirect3d.h"
#include "common.h"

D3DXMATRIX matTranslate, matRotation, matScale;

void TransformObject(D3DXVECTOR3 translate, D3DXVECTOR3 rotate, D3DXVECTOR3 scale, bool rotateAtPosition)
{
	// set translation and scaling matrix
	D3DXMatrixTranslation(&matTranslate, translate.x, translate.y, translate.z);
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	// set rotation matrix
	D3DXMATRIX xRot, yRot, zRot;

	D3DXMatrixRotationX(&xRot, D3DXToRadian(rotate.x));
	D3DXMatrixRotationY(&yRot, D3DXToRadian(rotate.y));
	D3DXMatrixRotationZ(&zRot, D3DXToRadian(rotate.z));

	matRotation = xRot * yRot * zRot;

	// get device and set world transformation
	auto device = MyDirect3D_GetDevice();

	if(!rotateAtPosition)
		device->SetTransform(D3DTS_WORLD, &(matScale * matTranslate * matRotation));
	else
		device->SetTransform(D3DTS_WORLD, &(matScale * matRotation * matTranslate));
}
