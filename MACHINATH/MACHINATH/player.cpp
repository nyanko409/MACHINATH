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
static GameObject* container;
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
	MeshObject::Draw();
}


void InitPlayer()
{	
	// get device
	device = MyDirect3D_GetDevice();

	// create container
	Transform trans = Transform(D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1.0F, 1.0F, 1.0F));
	container = new GameObject(trans);

	// create player
	trans = Transform(D3DXVECTOR3(0.0F, 2.5F, 0.0F), D3DXVECTOR3(0.0F, -90.0F, 0.0F), D3DXVECTOR3(1.0F, 1.0F, 1.0F));
	player = new Player(trans, 0.5F, MESH_ROBOT, SHADER_DEFAULT, 5, 5, 5, container);

	// create skateboard and make player the parent
	trans = Transform(D3DXVECTOR3(-0.2F, -2.5F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1.0F, 1.0F, 1.0F));
	skateboard = new MeshObject(trans, MESH_SKATEBOARD, SHADER_DEFAULT, player);

	// init player rotation speed
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
	//player->transform.rotation.y++;
	if (GetScene() != SCENE_GAMESCREEN) return;

	// move forward
	MoveForward();

	if (playTime >= 2.0F)
	{
		if (Curve())
		{
			//playTime = 0.0F;
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
float endZX;
int rotZX;
float startY, endY;
float startSpeed;
bool Curve()
{
	if (!initCurve)
	{
		initCurve = true;
		rotStarted = false;
		endZX = 30;
		rotZX = 10;
		startY = player->transform.rotation.y;
		endY = player->transform.rotation.y + 90;
		startSpeed = player->moveSpeed;
	}

	// decrease movespeed
	if (!rotStarted)
	{
		player->moveSpeed -= 0.01F;

		if (player->moveSpeed <= 0) 
			player->moveSpeed = 0;

		if (player->moveSpeed <= 0.2F)
			rotStarted = true;
	}

	// rotate if player slowed down
	if (rotStarted)
	{
		// increase speed
		player->moveSpeed += 0.01F;
		if (player->moveSpeed >= startSpeed) player->moveSpeed = startSpeed;

		// rotate skateboard in z and x
		skateboard->transform.rotation.z = rotZX * fabsf(player->GetForward().x);
		skateboard->transform.rotation.x = rotZX * fabsf(player->GetForward().z);

		//container->transform.rotation.z = (rotZX * fabsf(player->GetForward().x) / 2);
		//container->transform.rotation.x = (rotZX * fabsf(player->GetForward().z) / 2);
		
		if (rotZX > endZX) rotZX = endZX;
		if (endY - startY <= 10) rotZX -= 1.5F;
		else rotZX += 1.5F;

		if (skateboard->transform.rotation.z > endZX) skateboard->transform.rotation.z = endZX;
		if (skateboard->transform.rotation.x > endZX) skateboard->transform.rotation.x = endZX;
		if (skateboard->transform.rotation.z < 0) skateboard->transform.rotation.z = 0;
		if (skateboard->transform.rotation.x < 0) skateboard->transform.rotation.x = 0;

		// rotate in y
		if (startY < endY)
		{
			player->transform.rotation.y = startY;
			startY += 0.7F;

			if (player->transform.rotation.y >= endY)
				player->transform.rotation.y = endY;
		}
		else
		{
			if (player->moveSpeed >= startSpeed && rotZX <= 0)
			{
				skateboard->transform.rotation.z = 0;
				skateboard->transform.rotation.x = 0;
				container->transform.rotation.z = 0;
				container->transform.rotation.x = 0;
				return true;
			}
		}
	}

	return false;
}

void MoveForward()
{
	D3DXMATRIX rotOffset;
	D3DXVECTOR3 dir = player->GetForward();
	D3DXMatrixRotationY(&rotOffset, D3DXToRadian(90));
	D3DXVec3TransformCoord(&dir, &dir, &rotOffset);
	player->transform.position += dir * player->moveSpeed;
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

	SetCameraPos(D3DXVECTOR3(player->transform.position.x, player->transform.position.y, player->transform.position.z), D3DXVECTOR3(0, offsetY, -10), 0, rotY);
}
