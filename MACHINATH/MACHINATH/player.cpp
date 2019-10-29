#include "player.h"
#include "material.h"
#include "transformation.h"
#include "input.h"
#include "camera.h"


static LPDIRECT3DDEVICE9 device;
static Player* player;
static GameObject* skateboard;

static float rotSpeed;
static float rotMax;

void InitPlayer()
{	
	// get device
	device = MyDirect3D_GetDevice();

	// create player
	Transform trans = Transform(D3DXVECTOR3(0.0F, 1.0F, 0.0F), D3DXVECTOR3(0.0F, 90.0F, 0.0F), D3DXVECTOR3(1.0F, 1.0F, 1.0F));
	player = new Player(trans, 1.0F, MESH_ROBOT, 5, 5, 5 ,nullptr);

	trans = Transform(D3DXVECTOR3(-0.2F, -0.5F, 0.0F), D3DXVECTOR3(0.0F, -90.0F, 0.0F), D3DXVECTOR3(1.2F, 1.2F, 1.2F));
	skateboard = new GameObject(trans, MESH_SKATEBOARD, player);

	rotSpeed = 3.0F;
	rotMax = 10.0F;
}

void UninitPlayer()
{
	// free memory
	delete player;
}

void UpdatePlayer()
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
	if(player->transform.rotation.z > rotMax)
		player->transform.rotation.z = rotMax;
	if (player->transform.rotation.z < -rotMax)
		player->transform.rotation.z = -rotMax;

	// set camera position
	static float rotX = 0, rotY = 0; rotX = 0; rotY--;
	if (rotY <= -90) rotY = -90;
	static float offsetY = 30.0F;
	offsetY -= 0.1F;
	if (offsetY < 10) offsetY = 10;
	
	//SetCameraPos(D3DXVECTOR3(0, player->transform.position.y, player->transform.position.z), D3DXVECTOR3(0, offsetY, -25), 0, rotY);
}

void DrawPlayer()
{
	// draw player
	device->SetTexture(0, NULL);
	TransformObject(player->transform.position, player->transform.rotation, player->transform.scale);
	
	for (int i = 0; i < player->mesh->numMaterial; ++i)
	{
		//SetMaterial(&(player->mesh->material[i]));
		D3DMATERIAL9 mat;
		mat.Diffuse = D3DXCOLOR(1.0F, 0.0F, 0.0F, 1.0F);
		mat.Ambient = D3DXCOLOR(0.5F, 0.5F, 0.5F, 1.0F);
		SetMaterial(&mat);

		if (player->mesh->texture[i] != NULL)
			device->SetTexture(0, player->mesh->texture[i]);
			
		player->mesh->mesh->DrawSubset(i);
	}

	// skateboard
	TransformObject(skateboard->GetWorldPosition(), skateboard->GetWorldRotation(), skateboard->GetWorldScale());
	for (int i = 0; i < skateboard->mesh->numMaterial; ++i)
	{
		SetMaterial(&(skateboard->mesh->material[i]));

		if (skateboard->mesh->texture[i] != NULL)
			device->SetTexture(0, skateboard->mesh->texture[i]);

		skateboard->mesh->mesh->DrawSubset(i);
	}
}

Player* GetPlayer()
{
	return player;
}
