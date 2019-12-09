#include "fog.h"
#include "material.h"
#include "transformation.h"
#include "input.h"
#include "camera.h"
#include "playTime.h"
#include "sceneManagement.h"

// globals
#define JumpHeight (10.0F)
#define JumpSpeed (3.0F)

static LPDIRECT3DDEVICE9 device;
static Fog* fog;
static MeshObject* skateboard;

static float finalposy;
static float rotSpeed;
static float rotMax;
static bool jumpFrag;
static int jumpcnt;

void RotateSideways();
void Jump();
void FogCamera();


// override fog draw
void Fog::Draw()
{
	MeshObject::Draw();
}


void InitFog()
{
	// get device
	device = MyDirect3D_GetDevice();

	// create fog
	Transform trans = Transform(D3DXVECTOR3(0.0F, 2.5F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, -90.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	fog = new Fog(trans, 0.3F, MESH_ROBOT, SHADER_DEFAULT, 5, 5, 5);

	// create skateboard and make fog the parent
	trans = Transform(D3DXVECTOR3(-0.2F, -2.5F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	skateboard = new MeshObject(trans, MESH_SKATEBOARD, SHADER_DEFAULT, fog);

	// init fog rotation speed
	rotSpeed = 3.0F;
	rotMax = 10.0F;

	//reset jumpfrag and jumpcnt
	jumpFrag = false;
	jumpcnt = 0;
}

void UninitFog()
{
	// free memory
	delete skateboard;
	delete fog;
}

void UpdateFog()
{
	// rotate locally while moving sideways
	RotateSideways();

	// handle jumping
	Jump();

	// camera movement
	//FogCamera();
}



Fog* GetFog()
{
	return fog;
}

void RotateSideways()
{
	// move fog and rotate in z axis
	if (Keyboard_IsPress(DIK_F))
	{
		fog->transform.localRotation.z += rotSpeed;
	}
	else if (Keyboard_IsPress(DIK_G))
	{
		fog->transform.localRotation.z += -rotSpeed;
	}
	else
	{
		if (fog->transform.localRotation.z > 0)
		{
			fog->transform.localRotation.z -= rotSpeed;
			if (fog->transform.localRotation.z < 0)
				fog->transform.localRotation.z = 0;
		}
		else
		{
			fog->transform.localRotation.z += rotSpeed;
			if (fog->transform.localRotation.z > 0)
				fog->transform.localRotation.z = 0;
		}
	}

	// clip rotation
	if (fog->transform.localRotation.z > rotMax)
		fog->transform.localRotation.z = rotMax;
	if (fog->transform.localRotation.z < -rotMax)
		fog->transform.localRotation.z = -rotMax;
}

void Jump()
{
	if (!jumpFrag && Keyboard_IsPress(DIK_J))
	{
		jumpFrag = true;
		finalposy = fog->transform.position.y;
	}

	//jump
	if (jumpFrag)
	{
		fog->transform.position.y = finalposy + JumpHeight * sin(D3DXToRadian(jumpcnt));
		jumpcnt += JumpSpeed;
		float finalRot = 360.0F / (180.0F / JumpSpeed);

		fog->transform.localRotation.y += finalRot;
		//fog->transform.localRotation.z += finalRot;
		fog->transform.localRotation.x += finalRot;

		if (jumpcnt > 180)
		{
			jumpFrag = false;
			jumpcnt = 0;
			fog->transform.localRotation.y = -90.0F;		//最終着地時点の角度
			//fog->transform.localRotation.z = 0.0F;
			fog->transform.localRotation.x = 0.0F;
		}
	}
}

void FogCamera()
{
	// set camera position
	static float rotX = 0, rotY = 0;
	static float offsetY = 10.0F;

	//if (playTime > 5.0F)
	//{
	//	rotY--;
	//	if (rotY <= -45) rotY = -45;
	//	offsetY -= 0.1F;
	//	if (offsetY < 10) offsetY = 10;
	//}

	SetCameraPos(D3DXVECTOR3(0, fog->transform.position.y, fog->transform.position.z), D3DXVECTOR3(0, offsetY, -10), 0, 0);
}
