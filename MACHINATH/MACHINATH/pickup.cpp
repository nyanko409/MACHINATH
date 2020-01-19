#include <vector>
#include "common.h"
#include "pickup.h"
#include "customMath.h"
#include "mydirect3d.h"
#include "player.h"
#include "effect.h"
#include "map.h"
#include "score.h"


// globals
static std::vector<Pickup*> g_pickup;
static float g_zRotSpeed = 0;

// set pickups
static std::vector<std::pair<int, D3DXVECTOR3>> g_spawnPos =
{
	{ 1, D3DXVECTOR3(rand()%36-18, 1.3f, 10)},
	//{ 2, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{ 3, D3DXVECTOR3(rand()%36-18, 1.3f, 0)}, 
	//{ 4, D3DXVECTOR3(rand()%36-18, 1.3f, 0)}, 
	//{ 5, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{ 6, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{ 7, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{ 8, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{ 9, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{10, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
								   
	//{11, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{12, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{13, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{14, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{15, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{16, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{17, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{18, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{19, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{20, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//				 			   
	//{21, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{22, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{23, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{24, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{25, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{26, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{27, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{28, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{29, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{30, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//							   
	//{31, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{32, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{33, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{34, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{35, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{36, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{37, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{38, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{39, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{40, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//							  
	//{41, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{42, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{43, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{44, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{45, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{46, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{47, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{48, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{49, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{50, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//				 			 
	//{51, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{52, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{53, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{54, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{55, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{56, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{57, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{58, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{59, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{60, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//				 			   
	//{61, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{62, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{63, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{64, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{65, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{66, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{67, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{68, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{69, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{70, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//							   
	//{71, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{72, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{73, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{74, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{75, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{76, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{77, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{78, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{79, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{80, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//							   
	//{81, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{82, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{83, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{84, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{85, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{86, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{87, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{88, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{89, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{90, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//				 			   
	//{91, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{92, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{93, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},
	//{94, D3DXVECTOR3(rand()%36-18, 1.3f, 0)},

};



void Pickup::Draw()
{
	MeshObject::Draw();
}

void InitPickup()
{
	// init
	g_zRotSpeed = 10;
	g_pickup = std::vector<Pickup*>();
}

void UninitPickup()
{
	// free memory
	for (int i = 0; i < g_pickup.size(); ++i)
		SAFE_DELETE(g_pickup[i]);

	g_pickup.clear();
}

void UpdatePickup()
{
	// loop for every pickup
	for (int i = 0; i < g_pickup.size(); i++)
	{
		// rotate pickup
		g_pickup[i]->transform.localRotation.y += g_zRotSpeed;

		// check for collision with player
		if (g_pickup[i]->col.CheckCollision(GetPlayer()->col))
		{
			// collided, play effect and delete pickup
			//PlayEffect(EFFECT_JUMP, g_pickup[i]->GetCombinedPosition(), { 0, 0, 0 }, { 0, 0, 0 }, {5, 5, 5});
			SAFE_DELETE(g_pickup[i]);
			AddScore(100);
			g_pickup.erase(g_pickup.begin() + i);
		}
	}
}


void ActivatePickup(int mapId)
{
	// activate all pickups with the given mapId
	for (int i = 0; i < g_spawnPos.size(); ++i)
	{
		if (g_spawnPos[i].first > mapId) return;
		if (g_spawnPos[i].first == mapId)
		{
			SpawnPickup(g_spawnPos[i].first, g_spawnPos[i].second, GetMapById(mapId));
		}
	}
}

void CleanPickup(int mapId)
{
	// delete all pickups with the given mapId
	for (int i = 0; i < g_pickup.size(); ++i)
	{
		if (g_pickup[i]->mapId > mapId) return;
		if (g_pickup[i]->mapId == mapId)
		{
			delete g_pickup[i];
			g_pickup.erase(g_pickup.begin() + i);
			--i;
		}
	}
}

void SpawnPickup(int mapId, D3DXVECTOR3 position, GameObject* parent)
{
	SpawnPickup(mapId, position.x, position.y, position.z, parent);
}

void SpawnPickup(int mapId, float posX, float posY, float posZ, GameObject* parent)
{
	Transform trans(D3DXVECTOR3(posX, posY, posZ), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 90, 0), D3DXVECTOR3(0.2F, 0.2F, 0.2F));
	g_pickup.emplace_back(new Pickup(mapId, trans, MESH_COIN, SHADER_DEFAULT, 12, 12, 12, true, parent));
}

std::vector<Pickup*>* GetPickup()
{
	return &g_pickup;
}
