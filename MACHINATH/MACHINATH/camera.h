#pragma once

#include "common.h"


// camera class
class Camera
{
public:
	D3DXVECTOR3 position;								// position of the camera
	D3DXVECTOR3 lookDirection;							// position where camera should be looking

	D3DXVECTOR3 up;										// current up vector
	D3DXVECTOR3 forward;								// current forward vector
	D3DXVECTOR3 right;									// current right vector

	float nearClip;										// near field clip
	float farClip;										// far field clip

	float aspect;										// aspect ratio
	float fov;											// field of view

	float moveSpeed;									// speed of the camera when moving
	float lookSensivity;								// sensivity of mouse look

	// constructor
	Camera(D3DXVECTOR3 pos, D3DXVECTOR3 lookDir) :
		position(pos), lookDirection(lookDir)
	{
		up = D3DXVECTOR3(0.0F, 1.0F, 0.0F);
		forward = D3DXVECTOR3(0.0F, 0.0F, 1.0F);
		right = D3DXVECTOR3(1.0F, 0.0F, 0.0F);

		aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
		nearClip = 1.0F;
		farClip = 200.0F;
		fov = D3DXToRadian(90);
		moveSpeed = 0.6F;
		lookSensivity = 1.0F;
	}

	// destructor
	~Camera() {}

	void Rotate(float yDeg, float xDeg)
	{
		// inverse xDeg for x rotation if looking into -z
		float xRotYDeg = xDeg;
		if (forward.z < 0) xRotYDeg *= -1;

		// inverse xDeg for z rotation if looking into +x
		float zRotYDeg = xDeg;
		if (forward.x > 0) zRotYDeg *= -1;

		// get rotation angle
		D3DXMATRIX xRot, yRot, zRot;

		D3DXMatrixRotationX(&xRot, D3DXToRadian(xRotYDeg * lookSensivity) * (1 - fabsf(forward.x)));
		D3DXMatrixRotationY(&yRot, D3DXToRadian(yDeg * lookSensivity));
		D3DXMatrixRotationZ(&zRot, D3DXToRadian(zRotYDeg * lookSensivity) * fabsf(forward.x));

		// rotate forward vector
		D3DXVECTOR3 temp;
		D3DXVec3TransformCoord(&temp, &forward, &(xRot * yRot * zRot));

		if (temp.y > 0.95F) return;
		if (temp.y < -0.95F) return;

		forward = temp;

		// calculate right vector from the new forward vector
		D3DXMATRIX nRot;
		D3DXMatrixRotationY(&nRot, D3DXToRadian(90));
		D3DXVec3TransformCoord(&right, &(D3DXVECTOR3(forward.x, 0, forward.z)), &(nRot));
	}

	void LookAt(D3DXVECTOR3 objPos)
	{
		// vector between camera pos and object pos
		D3DXVECTOR3 diff = objPos - position;

		// normalize the vector
		float mag = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
		D3DXVECTOR3 res{ diff.x / mag, diff.y / mag, diff.z / mag };

		// set forward vector
		forward = res;
	}
};


// set camera pos to given value
void SetCameraPos(D3DXVECTOR3 lookAt, D3DXVECTOR3 position, int rotX, int rotY, int rotZ);

// return address of camera
Camera* GetCamera();

// call once in init
void InitCamera();

// call once in uninit
void UninitCamera();

// call every frame
void UpdateCamera();
