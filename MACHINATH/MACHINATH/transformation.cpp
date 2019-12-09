#include <d3dx9.h>
#include "transformation.h"
#include "mydirect3d.h"
#include "common.h"
#include "shader.h"

D3DXMATRIX matTranslate, matRotation, matLocalRotation, matScale, matPivot;
D3DXMATRIX matWorld;

D3DXMATRIX TransformObject(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 rot, D3DXVECTOR3 localRot, D3DXVECTOR3 pivot)
{
	// get device
	auto device = MyDirect3D_GetDevice();
	pos -= pivot;

	// set translation and scaling matrix
	D3DXMatrixTranslation(&matTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);
	
	// set rotation matrix
	D3DXMATRIX xRot, yRot, zRot;

	D3DXMatrixRotationX(&xRot, D3DXToRadian(rot.x));
	D3DXMatrixRotationY(&yRot, D3DXToRadian(rot.y));
	D3DXMatrixRotationZ(&zRot, D3DXToRadian(rot.z));
	//D3DXMatrixRotationYawPitchRoll(&matRotation, D3DXToRadian(rot.y), D3DXToRadian(rot.x), D3DXToRadian(rot.z));

	matRotation = yRot * zRot * xRot;

	// set local rotation matrix
	D3DXMATRIX fxRot, fyRot, fzRot;
	D3DXMatrixRotationX(&fxRot, D3DXToRadian(localRot.x));
	D3DXMatrixRotationY(&fyRot, D3DXToRadian(localRot.y));
	D3DXMatrixRotationZ(&fzRot, D3DXToRadian(localRot.z));
	//D3DXMatrixRotationYawPitchRoll(&matLocalRotation, D3DXToRadian(localRot.y), D3DXToRadian(localRot.x), D3DXToRadian(localRot.z));

	matLocalRotation = fyRot * fzRot * fxRot;

	// rotate local based on pivot
	D3DXMatrixTranslation(&matPivot, pivot.x, pivot.y, pivot.z);
	matPivot *= matLocalRotation;

	// calculate world matrix
	matWorld = (matScale * matPivot * matTranslate * matRotation);

	// return world matrix
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
