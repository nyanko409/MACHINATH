#include "player.h"
#include "material.h"
#include "transformation.h"
#include "input.h"
#include "camera.h"


static LPDIRECT3DDEVICE9 device;
static Player* player;

void InitPlayer()
{	
	// get device
	device = MyDirect3D_GetDevice();

	// create player
	Transform trans(D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 180.0F, 0.0F), D3DXVECTOR3(1.0F, 1.0F, 1.0F));
	player = new Player(trans, 0.1F, MESH_SLIME, 5, 5, 5 ,nullptr);
}

void UninitPlayer()
{
	// free memory
	delete player;
}

void UpdatePlayer()
{
	// move player
	if (Keyboard_IsPress(DIK_F))
		player->transform.position.x += -0.5F;
	if (Keyboard_IsPress(DIK_G))
		player->transform.position.x += 0.5F;

	// set camera position
	static float rotX = 0, rotY = 0; rotX = 0; rotY++;
	
	//SetCameraPos(D3DXVECTOR3(0, player->transform.position.y, player->transform.position.z), D3DXVECTOR3(0, 20.0F, -20.0F), rotX, rotY);
}

void DrawPlayer()
{
	// draw player
	device->SetTexture(0, NULL);
	TransformObject(player->transform.position, player->transform.rotation, player->transform.scale);
	
	for (int i = 0; i < player->mesh->numMaterial; ++i)
	{
		SetMaterial(&(player->mesh->material[i]));

		if (player->mesh->texture[i] != NULL)
			device->SetTexture(0, player->mesh->texture[i]);
			
		player->mesh->mesh->DrawSubset(i);
	}
}

Player* GetPlayer()
{
	return player;
}
