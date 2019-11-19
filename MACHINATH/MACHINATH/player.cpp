#include "player.h"
#include "material.h"
#include "transformation.h"
#include "input.h"
#include "camera.h"
#include "playTime.h"
#include "sceneManagement.h"

// globals
#define JumpHeight (30.0F)
#define JumpSpeed (3.0F)

static LPDIRECT3DDEVICE9 device;
static Player* player;
static MeshObject* skateboard;

static float rotSpeed;
static float rotMax;
static bool jumpFrag;
static int jumpcnt;

void MoveForward();
bool Curve();
void MoveSideways();
void Jump();
void PlayerCamera();


// override player draw method
void Player::Draw()
{
	BoneObject::Draw();
}


void InitPlayer()
{	
	// get device
	device = MyDirect3D_GetDevice();

	// create player
	Transform trans = Transform(D3DXVECTOR3(0.0F, 1.0F, 0.0F), D3DXVECTOR3(0.0F, 90.0F, 0.0F), D3DXVECTOR3(1.0F, 1.0F, 1.0F));
	player = new Player(trans, 0.4F, ANIM_MESH_ROBOT, SHADER_DEFAULT, 5, 5, 5);
	player->SetAnimationSpeed(0.04F);
	player->PlayAnimation(1);

	// create skateboard and make player the parent
	trans = Transform(D3DXVECTOR3(-0.2F, -1.0F, 0.0F), D3DXVECTOR3(0.0F, -90.0F, 0.0F), D3DXVECTOR3(1.2F, 1.2F, 1.2F));
	skateboard = new MeshObject(trans, MESH_SKATEBOARD, SHADER_DEFAULT, player);

	// init player rotation
	rotSpeed = 3.0F;
	rotMax = 10.0F;

	//reset jumpfrag,jumpcnt
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
	if (GetScene() != SCENE_GAMESCREEN) return;

	// move forward
	MoveForward();

	if (playTime >= 2.5F)
	{
		if (Curve())
		{
			playTime = 0.0F;
		}
	}

	// left and right movement
	MoveSideways();

	// handle jumping
	Jump();

	// camera movement
	PlayerCamera();
}






Player* GetPlayer()
{
	return player;
}

bool initCurve;
bool rotStarted;
float endZ;
int rotZ;
float startY, endY;
float startSpeed;
bool Curve()
{
	if (!initCurve)
	{
		initCurve = true;
		rotStarted = false;
		startY = 0;
		endZ = 45;
		rotZ = 30;
		endY = player->transform.rotation.y + 90;
		startSpeed = player->moveSpeed;
	}

	// decrease movespeed
	if (!rotStarted)
	{
		player->moveSpeed -= 0.02F;

		if (player->moveSpeed <= 0) 
			player->moveSpeed = 0;

		if (player->moveSpeed <= 0.2F)
			rotStarted = true;
	}

	// rotate if player slowed down
	if (rotStarted)
	{
		// increase speed
		player->moveSpeed += 0.02F;
		if (player->moveSpeed >= startSpeed) player->moveSpeed = startSpeed;

		// rotate in z and x
		if (rotZ < endZ)
		{
			//player->transform.rotation.z += 10;
			player->transform.rotation.x = 40.0F * sinf(D3DXToRadian(player->transform.rotation.y*2-180.0f));
			player->transform.rotation.z = 90.0F* cosf(D3DXToRadian(player->transform.rotation.y*2-270.0f ));
			//player->transform.rotation.z = rotZ * player->GetForward().z;
			//player->transform.rotation.x = rotZ * player->GetForward().x;
			//rotZ += 5;
		}

		// rotate in y
		if (startY < 90)
		{
			player->transform.rotation.y += startY;
			startY += 0.01f;

			if (player->transform.rotation.y >= endY)
			{
				player->transform.rotation.y = endY;
			}
		}
		else
		{
			if(player->moveSpeed >= startSpeed)
				return true;
		}
	}

	return false;
}

void MoveForward()
{
	player->transform.position += player->GetForward() * player->moveSpeed;
}

void MoveSideways()
{
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

	// clip rotation
	if (player->transform.rotation.z > rotMax)
		player->transform.rotation.z = rotMax;
	if (player->transform.rotation.z < -rotMax)
		player->transform.rotation.z = -rotMax;
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
		float finalRot = 360.0F / (180.0F / JumpSpeed);

		player->transform.rotation.y += finalRot;
		//player->transform.rotation.z += finalRot;
		player->transform.rotation.x += finalRot;

		if (jumpcnt > 180)
		{
			jumpFrag = false;
			jumpcnt = 0;
			player->transform.rotation.y = 90.0F;
			//player->transform.rotation.z = 0.0F;
			player->transform.rotation.x = 0.0F;
			player->PlayAnimation(1);
		}
	}
}

void PlayerCamera()
{
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

	SetCameraPos(D3DXVECTOR3(0, player->transform.position.y, player->transform.position.z), D3DXVECTOR3(0, offsetY, -25), 0, rotY);
}
