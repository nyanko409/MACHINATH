#include "camera.h"
#include "mydirect3d.h"
#include "input.h"


// globals
POINT g_curPos, g_fixedPos = {500, 500};
Camera* g_pCam;

// function def
void CameraInput();

// operator overload
POINT operator - (POINT o1, POINT o2) { return POINT{ o1.x - o2.x, o1.y - o2.y }; }


void InitCamera()
{
	// init camera
	g_pCam = new Camera({0,0,0});
	g_pCam->LookAt({ 0,0,1 });

	// init cursor pos
	GetCursorPos(&g_curPos);
	g_curPos = g_fixedPos;
}

D3DXVECTOR3 SetCameraForward(D3DXVECTOR3 lookAt)
{
	// look at position by setting forward vector
	return g_pCam->LookAt(lookAt);
}

void SetCameraPos(D3DXVECTOR3 position, float offsetX, float offsetY, float offsetZ, int rotY)
{
	// get rotation matrix
	D3DXMATRIX yRot;
	D3DXMatrixRotationY(&yRot, D3DXToRadian(rotY));

	// get position matrix
	D3DXMATRIX mPosOffset, mPosFinal;
	D3DXMatrixTranslation(&mPosOffset, offsetX, offsetY, offsetZ);
	D3DXMatrixTranslation(&mPosFinal, position.x, position.y, position.z);

	// set camera to new position
	D3DXVec3TransformCoord(&g_pCam->position, &D3DXVECTOR3{0,0,0}, &(mPosOffset * yRot * mPosFinal));
}

void UpdateCamera()
{
#if _DEBUG
	CameraInput();
#endif

	// init matrix and get device
	auto device = MyDirect3D_GetDevice();
	D3DXMATRIX matView, matProjection;

	// set view
	D3DXMatrixLookAtLH(&matView, &g_pCam->position, &(g_pCam->forward + g_pCam->position), &g_pCam->up);
	device->SetTransform(D3DTS_VIEW, &matView);

	// set projection
	D3DXMatrixPerspectiveFovLH(&matProjection, g_pCam->fov, g_pCam->aspect, g_pCam->nearClip, g_pCam->farClip);
	device->SetTransform(D3DTS_PROJECTION, &matProjection);
}

void UninitCamera()
{
	SAFE_DELETE(g_pCam);
}

void CameraInput()
{ 
	// camera mouse look
	GetCursorPos(&g_curPos);
	POINT diffPoint = g_curPos - g_fixedPos;
	g_pCam->Rotate(diffPoint.x, diffPoint.y);

	SetCursorPos(g_fixedPos.x, g_fixedPos.y);
	g_curPos = g_fixedPos;

	// camera movement
	if (Keyboard_IsPress(DIK_W))
	{
		// move cam forward
		g_pCam->position += g_pCam->forward * g_pCam->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_A))
	{
		// move cam left
		g_pCam->position += -g_pCam->right * g_pCam->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_S))
	{
		// move cam backward
		g_pCam->position += -g_pCam->forward * g_pCam->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_D))
	{
		// move cam right
		g_pCam->position += g_pCam->right * g_pCam->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_Q))
	{
		// move cam up
		g_pCam->position.y += g_pCam->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_E))
	{
		// move cam down
		g_pCam->position.y += -g_pCam->moveSpeed;
	}

	// fov
	if (Keyboard_IsPress(DIK_M) && g_pCam->fov < 3)
		g_pCam->fov += 0.01f;

	if (Keyboard_IsPress(DIK_N) && g_pCam->fov > 1)
		g_pCam->fov -= 0.01f;
}

Camera* GetCamera()
{
	return g_pCam;
}
