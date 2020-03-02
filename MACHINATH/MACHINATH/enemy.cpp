#include "enemy.h"
#include "map.h"
#include "buttonInput.h"
#include "player.h"
#include "customMath.h"
#include "cameraevent.h"
#include "effect.h"
#include "camera.h"
#include "sound.h"
#include "mesh.h"


void Enemy::Draw()
{
#if _DEBUG
	if (!enableDraw) return;

	BoxCollider::DrawCollider(collider, D3DCOLOR(D3DCOLOR_RGBA(255, 0, 0, 255)));
	if(eventCollider) BoxCollider::DrawCollider(*eventCollider, D3DCOLOR(D3DCOLOR_RGBA(0, 255, 0, 255)));
#endif

	BoneObject::Draw();
}



GameObject* g_center;
Enemy* g_yellow;
Enemy* g_blue;
Enemy* g_red;
Enemy* g_twotone;

void UpdateEnemyEvent(Enemy*& enemy);



void InitEnemy()
{
	g_center = new GameObject();
	auto mapArray = GetMap();

	D3DXVECTOR3 enemyScale = { 2,2,2 };

	// init enemies at map
	for (Map* map : *mapArray)
	{
		if (!g_yellow && map->data.name == MESH_MAP_YELLOW_HIROBA)
		{
			Transform trans;
			D3DXVECTOR3 eventOffset, eventPos;

			if (map->exit == Direction::NORTH)
			{
				eventOffset = { 0, 15, -80 };
				eventPos = { 100, 40, 15 };
				trans.position.z += 40;
			}
			else if (map->exit == Direction::SOUTH)
			{
				eventOffset = { 0, 15, 80 };
				eventPos = { 100, 40, 15 };
				trans.position.z -= 40;
			}
			else if (map->exit == Direction::WEST)
			{
				eventOffset = { 80, 15, 0 };
				eventPos = { 15, 40, 100 };
				trans.position.x -= 40;
			}
			else
			{
				eventOffset = { -80, 15, 0 };
				eventPos = { 15, 40, 100 };
				trans.position.x += 40;
			}

			g_yellow = new Enemy(trans, A_MESH_ENEMY_YELLOW, SHADER_DEFAULT,
				20, 20, 20, { 0,10,0 }, eventPos.x, eventPos.y, eventPos.z, eventOffset, true, map);

			g_yellow->transform.rotation.y = 180;
			g_yellow->transform.scale = enemyScale;
			g_yellow->enableDraw = map->enableDraw;
			g_yellow->PlayAnimation(1);
			g_yellow->SetAnimationSpeed(0);
		}
		else if (!g_blue && map->data.name == MESH_MAP_BLUE_HIROBA)
		{
			Transform trans;
			D3DXVECTOR3 eventOffset, eventPos;

			if (map->exit == Direction::NORTH)
			{
				eventOffset = { 0, 15, -80 };
				eventPos = { 100, 40, 15 };
				trans.position.z += 40;
			}
			else if (map->exit == Direction::SOUTH)
			{
				eventOffset = { 0, 15, 80 };
				eventPos = { 100, 40, 15 };
				trans.position.z -= 40;
			}
			else if (map->exit == Direction::WEST)
			{
				eventOffset = { 80, 15, 0 };
				eventPos = { 15, 40, 100 };
				trans.position.x -= 40;
			}
			else
			{
				eventOffset = { -80, 15, 0 };
				eventPos = { 15, 40, 100 };
				trans.position.x += 40;
			}

			g_blue = new Enemy(Transform(), A_MESH_ENEMY_BLUE, SHADER_DEFAULT,
				20, 20, 20, { 0,10,0 }, eventPos.x, eventPos.y, eventPos.z, eventOffset, true, map);

			g_blue->transform.rotation.y = 180;
			g_blue->transform.scale = enemyScale;
			g_blue->enableDraw = map->enableDraw;
			g_blue->PlayAnimation(1);
			g_blue->SetAnimationSpeed(0);
		}
		else if (!g_red && map->data.name == MESH_MAP_RED_HIROBA)
		{
			Transform trans;
			D3DXVECTOR3 eventOffset, eventPos;

			if (map->exit == Direction::NORTH)
			{
				eventOffset = { 0, 15, -80 };
				eventPos = { 100, 40, 15 };
				trans.position.z += 40;
			}
			else if (map->exit == Direction::SOUTH)
			{
				eventOffset = { 0, 15, 80 };
				eventPos = { 100, 40, 15 };
				trans.position.z -= 40;
			}
			else if (map->exit == Direction::WEST)
			{
				eventOffset = { 80, 15, 0 };
				eventPos = { 15, 40, 100 };
				trans.position.x -= 40;
			}
			else
			{
				eventOffset = { -80, 15, 0 };
				eventPos = { 15, 40, 100 };
				trans.position.x += 40;
			}

			g_red = new Enemy(Transform(), A_MESH_ENEMY_RED, SHADER_DEFAULT,
				20, 20, 20, { 0,10,0 }, eventPos.x, eventPos.y, eventPos.z, eventOffset, true, map);

			g_red->transform.rotation.y = 180;
			g_red->transform.scale = enemyScale;
			g_red->enableDraw = map->enableDraw;
			g_red->PlayAnimation(1);
			g_red->SetAnimationSpeed(0.000);
		}
		else if (!g_twotone && map->data.name == MESH_MAP_TWOTONE_HIROBA)
		{
			Transform trans;
			D3DXVECTOR3 eventOffset, eventPos;

			if (map->exit == Direction::NORTH)
		{
			eventOffset = { 0, 15, -80 };
			eventPos = { 100, 40, 15 };
			trans.position.z += 40;
		}
			else if (map->exit == Direction::SOUTH)
		{
			eventOffset = { 0, 15, 80 };
			eventPos = { 100, 40, 15 };
			trans.position.z -= 40;
		}
			else if (map->exit == Direction::WEST)
		{
			eventOffset = { 80, 15, 0 };
			eventPos = { 15, 40, 100 };
			trans.position.x -= 40;
		}
			else
			{
				eventOffset = { -80, 15, 0 };
				eventPos = { 15, 40, 100 };
				trans.position.x += 40;
			}

			g_twotone = new Enemy(Transform(), A_MESH_ENEMY_TWOTONE, SHADER_DEFAULT,
				20, 20, 20, { 0,10,0 }, eventPos.x, eventPos.y, eventPos.z, eventOffset, true, map);

			g_twotone->transform.rotation.y = 180;
			g_twotone->transform.scale = enemyScale;
			g_twotone->enableDraw = map->enableDraw;
			g_twotone->PlayAnimation(1);
			g_twotone->SetAnimationSpeed(0.000);
		}
	}
}

void UninitEnemy()
{
	SAFE_DELETE(g_yellow);
	SAFE_DELETE(g_blue);
	SAFE_DELETE(g_red);
	SAFE_DELETE(g_twotone);
	SAFE_DELETE(g_center);
}

void UpdateEnemy()
{
	UpdateEnemyEvent(g_yellow);
	UpdateEnemyEvent(g_blue);
	UpdateEnemyEvent(g_red);
	UpdateEnemyEvent(g_twotone);
}

void UpdateEnemyEvent(Enemy*& enemy)
{
	static float lerpValue = 0.01F;
	static float scale = 0.1F;
	static int handle = -1;

	// return if not needed
	if (!enemy) return;

	// collided with event collider
	if (enemy->eventCollider && enemy->eventCollider->CheckCollision(GetPlayer()->col))
	{
		lerpValue = 0.01F;
		scale = 0.1F;
		handle = -1;

		// start qte
		QueueMapEvent({ {}, MapEvent::QTE_MULTI, 20, 0.1F });
		SAFE_DELETE(enemy->eventCollider);

		enemy->PlayAnimation(1);
		enemy->SetAnimationSpeed(0.005F);

		CameraEventData ced = { -80, -3.0F, -10, -0.2F, -1, -0.2F };
		AddCameraEvent(ced);

		// set camera target between player and enemy
		Direction dir = ((Map*)enemy->parent)->exit;
		auto playerPos = GetPlayer()->GetCombinedPosition();

		if (dir == Direction::NORTH || dir == Direction::SOUTH)
			playerPos.x = enemy->GetCombinedPosition().x;		
		else
			playerPos.z = enemy->GetCombinedPosition().z;

		g_center->transform.position = Lerp(playerPos, enemy->GetCombinedPosition(), 0.7F);
		GetCamera()->target = g_center;

		// play attack effect
		handle = PlayEffect(EFFECT_BOOST, GetPlayer()->GetCombinedPosition(), 
			GetPlayer()->GetCombinedRotation(), { scale,scale,scale }, 1.0F);
	}

	// after colliding with event collider
	else if (!enemy->collided && !enemy->eventCollider)
	{
		// update the attack effect
		UpdateEffect(handle, GetPlayer()->GetCombinedPosition() + (GetPlayer()->GetForward() * 8), 
			GetPlayer()->GetCombinedRotation(), { scale,scale,scale });

		// increase effect scale
		if (onButtonPressed) scale += 0.1F;

		// move player forcefully back to center
		GetPlayer()->isMovingSideways = false;
		GetPlayer()->transform.localRotation = { 0,0,0 };
		for (auto child : GetPlayer()->child)
		{
			child->transform.localRotation = { 0,0,0 };
		}

		Direction dir = ((Map*)enemy->parent)->exit;
		float px, mx;
		if (dir == Direction::NORTH || dir == Direction::SOUTH)
		{
			px = GetPlayer()->parent->transform.position.x;
			mx = enemy->GetCombinedPosition().x;
			GetPlayer()->parent->transform.position.x = Lerp(px, mx, 0.03F);
		}
		else
		{
			px = GetPlayer()->parent->transform.position.z;
			mx = enemy->GetCombinedPosition().z;
			GetPlayer()->parent->transform.position.z = Lerp(px, mx, 0.03F);
		}
	}

	// the frame player collided with enemy
	if (!enemy->collided && enemy->collider.CheckCollision(GetPlayer()->col))
	{
		// stop attack effect
		StopEffect(handle);

		// set target back to player
		GetCamera()->target = GetPlayer();

		// set variables
		enemy->collided = true;
		enemy->enableDraw = false;
		GetPlayer()->isMovingSideways = true;
		GetPlayer()->moveSpeed = 0.3F;
		GetPlayer()->sideSpeed = 0;

		lerpValue = 0.001F;
		SetLerpSpeed(lerpValue);

		// queue up camera event
		CameraEventData ced = { 80, 2.0F, 10, 0.2F, 1, 0.2F };
		AddCameraEvent(ced);

		// play explosion effect
		if (enemy == g_yellow)
			PlayEffect(EFFECT_EXPLOSION_YELLOW, enemy->GetCombinedPosition() + D3DXVECTOR3{ 0,5,0 }, { 0,0,0 }, { 3,3,3 }, 1.0F);
		else if (enemy == g_red)
			PlayEffect(EFFECT_EXPLOSION_RED, enemy->GetCombinedPosition() + D3DXVECTOR3{ 0,5,0 }, { 0,0,0 }, { 3,3,3 }, 1.0F);
		else if(enemy == g_blue)
			PlayEffect(EFFECT_EXPLOSION_BLUE, enemy->GetCombinedPosition() + D3DXVECTOR3{0,5,0}, { 0,0,0 }, { 3,3,3 }, 1.0F);
		else if(enemy == g_twotone)
			PlayEffect(EFFECT_EXPLOSION_TWOTONE, enemy->GetCombinedPosition() + D3DXVECTOR3{ 0,5,0 }, { 0,0,0 }, { 3,3,3 }, 0.3F);

		PlaySound(AUDIO_SE_EXPLOSION);
	}

	// after colliding with enemy
	if (enemy->collided)
	{
		// increase movespeed
		GetPlayer()->moveSpeed += 0.007F;
		if (GetPlayer()->moveSpeed > 2.0F)
			GetPlayer()->moveSpeed = 2;

		// increase sidespeed
		GetPlayer()->sideSpeed += 0.004F;
		if (GetPlayer()->sideSpeed > 1.0F)
			GetPlayer()->sideSpeed = 1;

		// increase camera lerp speed
		lerpValue += 0.0003F;
		if (lerpValue > 0.1F)
			lerpValue = 0.1F;

		SetLerpSpeed(lerpValue);

		// delete enemy when value is reached
		if (GetPlayer()->moveSpeed == 2 && GetPlayer()->sideSpeed == 1 && lerpValue == 0.1F)
			SAFE_DELETE(enemy);
	}
}
