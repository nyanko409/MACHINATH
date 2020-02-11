#include "boss.h"
#include "player.h"
#include "effect.h"
#include "map.h"
#include "cameraevent.h"
#include "camera.h"
#include "customMath.h"
#include "sound.h"


void Boss::Draw()
{
	if (!enableDraw) return;

#ifdef _DEBUG
	BoxCollider::DrawCollider(collider, D3DCOLOR(D3DCOLOR_RGBA(255, 0, 0, 255)));
#endif

	// draw both faces for boss
	auto pDevice = MyDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	MeshObject::Draw();
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}



static Boss* g_boss;

static bool g_collided;
static float g_curTime, g_interval;
static int g_curExplosionCount, g_explosionCount;

void MovePlayerToFinalPosition();



void InitBoss()
{
	// get last map
	Map* map = GetMap()->back();

	// only init boss if there is a metropolitan map
	if (map->data.name != MESH_NAME::MESH_MAP_METROPOLITAN) return;

	// init boss
	Transform trans;

	if(map->exit == Direction::NORTH)
		trans = Transform({ 0 ,50, -200 });
	else if (map->exit == Direction::SOUTH)
		trans = Transform({ 0 ,50, 200 });
	else if(map->exit == Direction::WEST)
		trans = Transform({ 200 ,50, 0 });
	else
		trans = Transform({ -200 ,50, 0 });

	g_boss = new Boss(trans, MESH_BOSS, SHADER_DEFAULT, map);
	g_boss->enableDraw = map->enableDraw;

	g_collided = false;
	g_interval = 0.5F;
	g_curTime = g_interval;
	g_curExplosionCount = 0;
	g_explosionCount = 15;
}

void UninitBoss()
{
	SAFE_DELETE(g_boss);
}

void UpdateBoss()
{
	// return if boss is already dead
	if (!g_boss) return;

	// move player back to center
	if (g_boss->enableDraw)
	{
		GetPlayer()->isMovingSideways = false;
		GetPlayer()->transform.localRotation = { 0,0,0 };
		for (auto child : GetPlayer()->child)
		{
			child->transform.localRotation = { 0,0,0 };
		}

		Direction dir = GetMap()->back()->exit;
		float px, mx;
		if (dir == Direction::NORTH || dir == Direction::SOUTH)
		{
			px = GetPlayer()->parent->transform.position.x;
			mx = GetMap()->back()->GetCombinedPosition().x;
			GetPlayer()->parent->transform.position.x = Lerp(px, mx, 0.02F);
		}
		else
		{
			px = GetPlayer()->parent->transform.position.z;
			mx = GetMap()->back()->GetCombinedPosition().z;
			GetPlayer()->parent->transform.position.z = Lerp(px, mx, 0.02F);
		}
	}

	// if collided with player, delete object and play effects
	if (!g_collided && g_boss->enableDraw && g_boss->collider.CheckCollision(GetPlayer()->col))
	{
		g_collided = true;

		SetLerpSpeed(0.005F);
		GetCamera()->target = g_boss;
		GetCamera()->forwardOverride = g_boss;

		GetPlayer()->enableDraw = false;
		GetPlayer()->isMoving = false;
		for (auto child : GetPlayer()->child)
		{
			child->enableDraw = false;
		}

		CameraEventData ced = {360 + 45, 1, -50, -0.6, 2, 0.1F};
		AddCameraEvent(ced);
	}
	
	if (g_collided)
	{
		// play explosion per interval
		if (g_curExplosionCount < g_explosionCount)
		{
			g_curTime += TIME_PER_FRAME;
			if (g_curTime >= g_interval)
			{
				g_curTime -= g_interval;
				g_curExplosionCount++;

				// explode at random offset from boss position
				D3DXVECTOR3 expPos = g_boss->GetCombinedPosition();

				int randX = rand() % 30;
				int randY = rand() % 30;
				int randZ = rand() % 30;

				if (g_curExplosionCount % 2)
				{
					randX = -randX;
					randY = -randY;
					randZ = -randZ;
				}

				expPos.x += randX;
				expPos.y += randY;
				expPos.z += randZ;

				// play the effect
				PlayEffect(EFFECT_EXPLOSION_RED, expPos, { 0, 0, 0 }, { 4, 4, 4 }, 0.7F);
				PlaySound(AUDIO_SE_EXPLOSION);
			}
		}
		else if (g_curExplosionCount == g_explosionCount)
		{
			// set camera target back to player
			MovePlayerToFinalPosition();
			GetCamera()->target = GetPlayer();
			GetCamera()->forwardOverride = GetPlayer();
			g_curExplosionCount++;
		}
	}
}

void MovePlayerToFinalPosition()
{
	Player* player = GetPlayer();

	player->enableDraw = true;
	player->transform.position = { 0,1,0 };
	player->transform.rotation = { 0,0,0 };
	player->transform.localRotation = { 0,0,0 };

	player->parent->transform.rotation = { 0,0,0 };
	player->parent->transform.localRotation = { 0,0,0 };

	for (auto child : player->child)
	{
		child->enableDraw = true;
		child->transform.rotation = { 0,0,0 };
		child->transform.localRotation = { 0,0,0 };
	}

	CameraEventData ced = { 180, 180, 50, 1, 3, 0.008F };
	AddCameraEvent(ced);
	ced = { 0, 0, 0, 0, 5, 0.1F };
	AddCameraEvent(ced);
	ced = { 360, 1, 0, 0, 0, 0 };
	AddCameraEvent(ced);
	ced = { 0, 0, 0, 0, -5, -0.1F };
	AddCameraEvent(ced);
	ced = { -40, -0.5F, 8, 0.1F, -11, -0.1F };
	AddCameraEvent(ced);

	// play player pose animation
	player->PlayAnimation(0);
	player->SetAnimationSpeed(0.02F);

	// offset player and camera to final position
	Direction exit = GetMap()->back()->exit;
	player->parent->transform.position = GetMap()->back()->GetCombinedPosition();
	player->enableDraw = true;

	GetCamera()->targetOffset.y += 2;
	player->transform.position.y += 200;

	if (exit == Direction::NORTH)
	{
		GetCamera()->positionOffset.x = -3;
		player->transform.position.x += 10;
		player->transform.position.z -= 90;
		player->transform.rotation.y = 180;
		player->child.front()->transform.rotation = { 0,90,0 };
	}
	if (exit == Direction::SOUTH)
	{
		GetCamera()->positionOffset.x = 3;
		player->transform.position.x -= 10;
		player->transform.position.z += 90;
		player->child.front()->transform.rotation = { 0,90,0 };
	}
	if (exit == Direction::EAST)
	{
		GetCamera()->positionOffset.z = 3;
		player->transform.position.x -= 90;
		player->transform.position.z -= 10;
		player->transform.rotation.y = -90;
		player->child.front()->transform.rotation = { 0,90,0 };
	}
	if (exit == Direction::WEST)
	{
		GetCamera()->positionOffset.z = -3;
		player->transform.position.x += 90;
		player->transform.position.z += 10;
		player->transform.rotation.y = 90;
		player->child.front()->transform.rotation = { 0,90,0 };
	}
}
