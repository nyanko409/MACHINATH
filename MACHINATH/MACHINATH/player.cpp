#include "player.h"
#include "material.h"
#include "transformation.h"
#include "input.h"
#include "camera.h"
#include "playTime.h"
#include "sceneManagement.h"

#define JumpHeight (30.0F)
#define JumpSpeed (3.0F)

static LPDIRECT3DDEVICE9 device;
static Player* player;
static MeshObject* skateboard;

static float rotSpeed;
static float rotMax;
static bool jumpFrag;
static int jumpcnt;

void Jump();

void InitPlayer()
{	
	// get device
	device = MyDirect3D_GetDevice();

	// create player
	Transform trans = Transform(D3DXVECTOR3(0.0F, 1.0F, 0.0F), D3DXVECTOR3(0.0F, 90.0F, 0.0F), D3DXVECTOR3(1.0F, 1.0F, 1.0F));
	player = new Player(trans, 1.0F, MESH_ROBOTA, 5, 5, 5 ,nullptr);
	player->SetAnimationSpeed(0.04F);
	player->PlayAnimation(1);

	// create skateboard and make player the parent
	trans = Transform(D3DXVECTOR3(-0.2F, -1.0F, 0.0F), D3DXVECTOR3(0.0F, -90.0F, 0.0F), D3DXVECTOR3(1.2F, 1.2F, 1.2F));
	skateboard = new MeshObject(trans, MESH_SKATEBOARD, player);

	// init player rotation
	rotSpeed = 3.0F;
	rotMax = 10.0F;

	//reset jumpfrag,jumpcnt
	jumpFrag = false;
	jumpcnt = 0;

	StartTimer();
}

void UninitPlayer()
{
	// free memory
	delete skateboard;
	delete player;
}

void UpdatePlayer()
{
	if (GetScene() != SCENE_GAMESCREEN) return;

	// move player and rotate in z axis
	if (Keyboard_IsPress(DIK_F))
	{
		player->transform.position.x += -0.5F;
		player->transform.rotation.z += rotSpeed;
	}
	else if (Keyboard_IsPress(DIK_G))
	{
		player->transform.position.x += 0.5F;
		player->transform.rotation.z += -rotSpeed;
	}
	else
	{
		if (player->transform.rotation.z > 0)
		{
			player->transform.rotation.z -= rotSpeed;
			if (player->transform.rotation.z < 0)
				player->transform.rotation.z = 0;
		}
		else
		{
			player->transform.rotation.z += rotSpeed;
			if (player->transform.rotation.z > 0)
				player->transform.rotation.z = 0;
		}
	}

	Jump();

	// clip rotation
	if(player->transform.rotation.z > rotMax)
		player->transform.rotation.z = rotMax;
	if (player->transform.rotation.z < -rotMax)
		player->transform.rotation.z = -rotMax;

	// set camera position
	static float rotX = 0, rotY = 0;
	static float offsetY = 30.0F;

	if (playTime > 5.0F) 
	{
		rotY--;
		if (rotY <= -45) rotY = -45;
		offsetY -= 0.1F;
		if (offsetY < 10) offsetY = 10;
	}

	//SetCameraPos(D3DXVECTOR3(0, player->transform.position.y, player->transform.position.z), D3DXVECTOR3(0, offsetY, -25), 0, rotY);
}

void Jump()
{
	if (!jumpFrag && Keyboard_IsPress(DIK_J))
	{
		jumpFrag = true;
		player->PlayAnimation(2);
	}

	//jump
	if (jumpFrag)
	{
		player->transform.position.y = 1.0F + 40.0F*sin(D3DXToRadian(jumpcnt));
		jumpcnt += JumpSpeed;
		player->transform.rotation.y += 360.0F / (180.0F / JumpSpeed);
		//player->transform.rotation.z += 360.0F / (180.0F / JumpSpeed);
		player->transform.rotation.x += 360.0F / (180.0F / JumpSpeed);
		if (jumpcnt > 180)
		{
			jumpFrag = false;
			jumpcnt = 0;
			player->transform.rotation.y = 90.0F;
			//	player->transform.rotation.z = 0.0F;
			player->transform.rotation.x = 0.0F;
			player->PlayAnimation(1);
		}
	}
}

void DrawPlayer()
{
	// draw player
	player->Draw(true, true);

	// draw skateboard
	skateboard->Draw(true, true);
}

Player* GetPlayer()
{
	return player;
}
