#include "transformation.h"
#include "mydirect3d.h"
#include "common.h"


D3DXMATRIX matTranslate, matRotation, matLocalRotation, matScale, matPivot;
D3DXMATRIX matWorld;


D3DXMATRIX TransformObject(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 rot,
	D3DXMATRIX& matOrientation, D3DXMATRIX matCombinedOrientation, D3DXVECTOR3 worldRot,
	D3DXVECTOR3& forward, D3DXVECTOR3& up, D3DXVECTOR3& right, D3DXVECTOR3 pivot)
{
	// get device
	auto device = MyDirect3D_GetDevice();
	pos -= pivot;

	// set translation and scaling matrix
	D3DXMatrixTranslation(&matTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	// world space rotation
	D3DXMATRIX xRot, yRot, zRot;
	D3DXMatrixRotationX(&xRot, D3DXToRadian(worldRot.x));
	D3DXMatrixRotationY(&yRot, D3DXToRadian(worldRot.y));
	D3DXMatrixRotationZ(&zRot, D3DXToRadian(worldRot.z));

	matRotation = xRot * yRot * zRot;

	// set local axis rotation matrix
	D3DXMatrixRotationAxis(&yRot, &up, D3DXToRadian(rot.y));
	D3DXVec3TransformCoord(&forward, &forward, &yRot); D3DXVec3TransformCoord(&right, &right, &yRot);

	D3DXMatrixRotationAxis(&xRot, &right, D3DXToRadian(rot.x));
	D3DXVec3TransformCoord(&forward, &forward, &xRot); D3DXVec3TransformCoord(&up, &up, &xRot);

	D3DXMatrixRotationAxis(&zRot, &forward, D3DXToRadian(rot.z));
	D3DXVec3TransformCoord(&right, &right, &zRot); D3DXVec3TransformCoord(&up, &up, &zRot);

	matLocalRotation = xRot * yRot * zRot;

	// update the orientation matrix
	matOrientation = matOrientation * matLocalRotation;
	matCombinedOrientation = matCombinedOrientation * matLocalRotation;

	// rotate locally based on pivot
	D3DXMATRIX matFinalRot;
	D3DXMatrixTranslation(&matPivot, pivot.x, pivot.y, pivot.z);
	matFinalRot = matPivot * matCombinedOrientation * matRotation;

	// calculate world matrix
	matWorld = (matScale * matFinalRot * matTranslate);

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
