#include "enemy.h"
#include "map.h"
#include "player.h"
#include "customMath.h"
#include "mesh.h"


void Enemy::Draw()
{
#if _DEBUG
	BoxCollider::DrawCollider(collider, D3DCOLOR(D3DCOLOR_RGBA(255, 0, 0, 255)));
	if(eventCollider) BoxCollider::DrawCollider(*eventCollider, D3DCOLOR(D3DCOLOR_RGBA(0, 255, 0, 255)));
#endif

	BoneObject::Draw();
}



Enemy* g_yellow;
Enemy* g_blue;
Enemy* g_red;
Enemy* g_twotone;

void UpdateEnemyEvent(Enemy* enemy);



void InitEnemy()
{
	auto mapArray = GetMap();

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
			g_yellow->transform.scale = {4, 4, 4};
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
			g_blue->transform.scale = { 4, 4, 4 };
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
			g_red->transform.scale = { 4, 4, 4 };
			g_red->enableDraw = map->enableDraw;
			g_red->PlayAnimation(1);
			g_red->SetAnimationSpeed(0.004);
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
			g_twotone->transform.scale = { 4, 4, 4 };
			g_twotone->enableDraw = map->enableDraw;
			g_twotone->PlayAnimation(1);
			g_twotone->SetAnimationSpeed(0.004);
		}
	}
}

void UninitEnemy()
{
	SAFE_DELETE(g_yellow);
	SAFE_DELETE(g_blue);
	SAFE_DELETE(g_red);
	SAFE_DELETE(g_twotone);
}

void UpdateEnemy()
{
	UpdateEnemyEvent(g_yellow);
	UpdateEnemyEvent(g_blue);
	UpdateEnemyEvent(g_red);
	UpdateEnemyEvent(g_twotone);

	// delete enemies when not needed anymore
	if (g_yellow && ((Map*)g_yellow->parent)->id < GetCurrentMapId())
		SAFE_DELETE(g_yellow);
	if (g_blue && ((Map*)g_blue->parent)->id < GetCurrentMapId())
		SAFE_DELETE(g_blue);
	if (g_red && ((Map*)g_red->parent)->id < GetCurrentMapId())
		SAFE_DELETE(g_red);
	if (g_twotone && ((Map*)g_twotone->parent)->id < GetCurrentMapId())
		SAFE_DELETE(g_twotone);
}

void UpdateEnemyEvent(Enemy* enemy)
{
	if (enemy && enemy->enableDraw)
	{
		// start qte event
		if (enemy->eventCollider && enemy->eventCollider->CheckCollision(GetPlayer()->col))
		{
			QueueMapEvent({ MapEvent::QTE_MULTI, 20, 0.1F });
			SAFE_DELETE(enemy->eventCollider);
		}

		// collided with event collider
		else if (!enemy->collided && !enemy->eventCollider)
		{
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

		// collided with enemy
		if (enemy->collider.CheckCollision(GetPlayer()->col))
		{
			enemy->collided = true;
			GetPlayer()->isMovingSideways = true;
		}
	}
}
