#include "camera.h"
#include "common.h"
#include "mydirect3d.h"
#include "input.h"
#include "font.h"
#include "shader.h"


// operator overload
POINT operator - (POINT o1, POINT o2) { return POINT{ o1.x - o2.x, o1.y - o2.y }; }

// function def
void CameraInput();

// variables
POINT last; POINT current;
Camera* cam;

void InitCamera()
{
	// init camera
	cam = new Camera(D3DXVECTOR3(0, 0, -10), D3DXVECTOR3(0, 0, 1));

	// init cursor pos
	GetCursorPos(&current);
	last = current;
}

void SetCameraPos(D3DXVECTOR3 position, D3DXVECTOR3 offset, float rotX, float rotY)
{
	// look at position by setting forward vector
	cam->LookAt(position);

	// rotate offset pos
	D3DXMATRIX xRot, yRot;

	D3DXMatrixRotationX(&xRot, D3DXToRadian(rotX));
	D3DXMatrixRotationY(&yRot, D3DXToRadian(rotY));

	D3DXVECTOR3 temp;
	D3DXVec3TransformCoord(&temp, &offset, &(xRot * yRot));

	// set camera position
	cam->position = position + temp;
}

void UpdateCamera()
{
	// move camera with WASD and mouse
	CameraInput();

	// init matrix and get device
	auto device = MyDirect3D_GetDevice();
	D3DXMATRIX matView, matProjection;

	// set view
	D3DXMatrixLookAtLH(&matView, &cam->position, &(cam->forward + cam->position), &cam->up);
	device->SetTransform(D3DTS_VIEW, &matView);

	// set projection
	D3DXMatrixPerspectiveFovLH(&matProjection, cam->fov, cam->aspect, cam->nearClip, cam->farClip);
	device->SetTransform(D3DTS_PROJECTION, &matProjection);
}

void UninitCamera()
{
	if (cam) delete cam;
}

void CameraInput()
{ 
	// camera mouse look
	last = current;
	
	GetCursorPos(&current);
	POINT diffPoint = current - last;
	cam->Rotate(diffPoint.x, diffPoint.y);

	// camera pos
	if (Keyboard_IsPress(DIK_W))
	{
		// move cam forward
		cam->position += cam->forward * cam->moveSpeed;
		cam->lookDirection += cam->forward * cam->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_A))
	{
		// move cam left
		cam->position += -cam->right * cam->moveSpeed;
		cam->lookDirection += -cam->right * cam->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_S))
	{
		// move cam backward
		cam->position += -cam->forward * cam->moveSpeed;
		cam->lookDirection += -cam->forward * cam->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_D))
	{
		// move cam right
		cam->position += cam->right * cam->moveSpeed;
		cam->lookDirection += cam->right * cam->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_Q))
	{
		// move cam up
		cam->position.y += cam->moveSpeed;
		cam->lookDirection.y += cam->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_E))
	{
		// move cam down
		cam->position.y += -cam->moveSpeed;
		cam->lookDirection.y += -cam->moveSpeed;
	}
}

Camera* GetCamera()
{
	return cam;
}
