#include "player.h"
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
static Player* player;
static MeshObject* skateboard;

static float finalposy;
static float rotSpeed;
static float rotMax;
static bool jumpFrag;
static int jumpcnt;

void MoveSideways();
void Jump();
void PlayerCamera();


// override player draw
void Player::Draw()
{
	MeshObject::Draw();
}


void InitPlayer()
{	
	// get device
	device = MyDirect3D_GetDevice();

	// create player
	Transform trans = Transform(D3DXVECTOR3(0.0F, 2.5F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, -90.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	player = new Player(trans, 0.3F, MESH_ROBOT, SHADER_DEFAULT, 5, 5, 5);

	// create skateboard and make player the parent
	trans = Transform(D3DXVECTOR3(-0.2F, -2.5F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, -90.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	skateboard = new MeshObject(trans, MESH_SKATEBOARD, SHADER_DEFAULT, player);

	// init player rotation speed
	rotSpeed = 3.0F;
	rotMax = 10.0F;

	//reset jumpfrag and jumpcnt
	jumpFrag = false;
	jumpcnt = 0;
}

void UninitPlayer()
{
	// free memory
	delete skateboard;
	delete player;
}

void UpdatePlayer()
{
	// left and right movement
	MoveSideways();

	// handle jumping
	Jump();

	// camera movement
	//PlayerCamera();
}






Player* GetPlayer()
{
	return player;
}

void MoveSideways()
{
	// move player and rotate in z axis
	if (Keyboard_IsPress(DIK_F))
	{
		player->transform.position.x += -0.5F;
		player->transform.localRotation.z += rotSpeed;
	}
	else if (Keyboard_IsPress(DIK_G))
	{
		player->transform.position.x += 0.5F;
		player->transform.localRotation.z += -rotSpeed;
	}
	else
	{
		if (player->transform.localRotation.z > 0)
		{
			player->transform.localRotation.z -= rotSpeed;
			if (player->transform.localRotation.z < 0)
				player->transform.localRotation.z = 0;
		}
		else
		{
			player->transform.localRotation.z += rotSpeed;
			if (player->transform.localRotation.z > 0)
				player->transform.localRotation.z = 0;
		}
	}

	// clip rotation
	if (player->transform.localRotation.z > rotMax)
		player->transform.localRotation.z = rotMax;
	if (player->transform.localRotation.z < -rotMax)
		player->transform.localRotation.z = -rotMax;
}

void Jump()
{
	if (!jumpFrag && Keyboard_IsPress(DIK_J))
	{
		jumpFrag = true;
		finalposy = player->transform.position.y;
	}

	//jump
	if (jumpFrag)
	{
		player->transform.position.y = finalposy + JumpHeight*sin(D3DXToRadian(jumpcnt));
		jumpcnt += JumpSpeed;
		float finalRot = 360.0F / (180.0F / JumpSpeed);

		player->transform.localRotation.y += finalRot;
		//player->transform.rotation.z += finalRot;
		player->transform.localRotation.x += finalRot;

		if (jumpcnt > 180)
		{
			jumpFrag = false;
			jumpcnt = 0;
			player->transform.localRotation.y = 90.0F;
			//player->transform.rotation.z = 0.0F;
			player->transform.localRotation.x = 0.0F;
		}
	}
}

void PlayerCamera()
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

	SetCameraPos(D3DXVECTOR3(0, player->transform.position.y, player->transform.position.z), D3DXVECTOR3(0, offsetY, -10), 0, 0);
}
