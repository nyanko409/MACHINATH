#include <vector>
#include <tuple>
#include "map.h"
#include "mesh.h"
#include "playTime.h"
#include "sceneManagement.h"
#include "customMath.h"
#include "player.h"
#include "input.h"
#include "pickup.h"

enum class MapEvent
{
	CURVE_LEFT, CURVE_RIGHT
};

struct EventTime
{
	MapEvent mapEvent;
	float time;
};

Transform GetStartTransform(const Map& prevMap);
void MoveSideways(int index);

// globals
std::vector<EventTime> event; 
std::vector<Map*> map;
static float mapRadius = 0;
static int drawCount;
static float poolDistance;
static float curveSpeed;

static int mapId;
static int drawIndex;
static bool isCurving;
static float curveRot;


void InitMap()
{
	mapId = 0;
	mapRadius = 90.0F;
	drawCount = 3;
	curveSpeed = 1.5F;
	poolDistance = 60.0F;

	isCurving = false;
	curveRot = 0;

	// init event times
	event = std::vector<EventTime>();
	event.emplace_back(EventTime {MapEvent::CURVE_LEFT, 4.5F});
	event.emplace_back(EventTime {MapEvent::CURVE_LEFT, 9.5F});
	event.emplace_back(EventTime {MapEvent::CURVE_LEFT, 24.5F});
	event.emplace_back(EventTime {MapEvent::CURVE_LEFT, 34.5F});
	event.emplace_back(EventTime {MapEvent::CURVE_LEFT, 89.5F});

	// init map
	map = std::vector<Map*>();

	Transform transform(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[0]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_CURVELEFT, Direction::WEST, SHADER_DEFAULT));

	transform = GetStartTransform(*map[1]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_CURVELEFT, Direction::SOUTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[2]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::SOUTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[3]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::SOUTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[4]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_CURVELEFT, Direction::EAST, SHADER_DEFAULT));

	transform = GetStartTransform(*map[5]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::EAST, SHADER_DEFAULT));

	transform = GetStartTransform(*map[6]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_CURVELEFT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[7]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[8]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[9]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[10]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[11]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[12]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[13]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[14]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[15]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[16]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*map[17]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_CURVELEFT, Direction::WEST, SHADER_DEFAULT));

	transform = GetStartTransform(*map[18]);
	map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUND, Direction::WEST, SHADER_DEFAULT));

	// enable draw for drawcount
	drawIndex = map.size() < drawCount ? map.size() : drawCount;
	for (int i = 0; i < drawIndex; ++i)
	{
		map[i]->enableDraw = true;
	}
}

void UpdateMap()
{
	if (GetScene() != SCENE_GAMESCREEN) return;

	// for every map in array
	for (int i = 0; i < map.size(); ++i)
	{
		// move map by player speed
		map[i]->transform.position -= map[i]->GetForward() * GetPlayer()->moveSpeed;

		// move sideways
		MoveSideways(i);

		// handle events
		if (event.size() > 0 && event.front().time <= playTime)
		{
			// curve right
			if (event.front().mapEvent == MapEvent::CURVE_RIGHT)
			{
				isCurving = true;
				if (curveRot < 90)
				{
					map[i]->transform.rotation.y -= curveSpeed;
				}
				else
				{
					event.erase(event.begin());
					isCurving = false;
					curveRot = 0;
				}
			}

			// curve left
			else if (event.front().mapEvent == MapEvent::CURVE_LEFT)
			{
				isCurving = true;
				if (curveRot < 90)
				{
					map[i]->transform.rotation.y += curveSpeed;
				}
				else
				{
					event.erase(event.begin());
					isCurving = false;
					curveRot = 0;
				}
			}
		}
	}

	// add rotation while curving
	if (isCurving)
		curveRot += curveSpeed;

	// remove first map from array if out of camera view
	if (map.size() > 0 && GetDistance(map[0]->transform.position, GetPlayer()->transform.position) > poolDistance)
	{
		// display next map
		if (map.size() > drawIndex)
		{
			map[drawIndex]->enableDraw = true;
			ActivatePickup(map[drawIndex]->id);
		}

		// delete pickup
		CleanPickup(map[0]->id);

		// delete map
		delete map[0];
		map.erase(map.begin());
	}
}

void UninitMap()
{
	// free memory
	for (int i = 0; i < map.size(); ++i)
	{
		delete map[i];
	}
}


void MoveSideways(int index)
{
	// move left and right
	if (Keyboard_IsPress(DIK_F))
	{
		D3DXMATRIX rot;
		D3DXVECTOR3 left;
		D3DXMatrixRotationY(&rot, D3DXToRadian(-90));
		D3DXVec3TransformCoord(&left, &map[index]->GetForward(), &rot);
		map[index]->transform.position -= left * GetPlayer()->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_G))
	{
		D3DXMATRIX rot;
		D3DXVECTOR3 left;
		D3DXMatrixRotationY(&rot, D3DXToRadian(90));
		D3DXVec3TransformCoord(&left, &map[index]->GetForward(), &rot);
		map[index]->transform.position -= left * GetPlayer()->moveSpeed;
	}
}

Transform GetStartTransform(const Map& prevMap)
{
	// cache previous map transform
	Transform trans = prevMap.transform;

	// offset to new location
	if (prevMap.exit == Direction::NORTH)
	{
		trans.position.z += mapRadius;
		trans.localRotation.y = 0;
	}
	else if (prevMap.exit == Direction::EAST)
	{
		trans.position.x += mapRadius;
		trans.localRotation.y = 90;
	}
	else if (prevMap.exit == Direction::WEST)
	{
		trans.position.x -= mapRadius;
		trans.localRotation.y = 270;
	}
	else if (prevMap.exit == Direction::SOUTH)
	{
		trans.position.z -= mapRadius;
		trans.localRotation.y = 180;
	}

	return trans;
}

Map* GetMapById(int mapId)
{
	for (int i = 0; i < map.size(); ++i)
	{
		if (map[i]->id == mapId) 
			return map[i];
	}

	return nullptr;
}

std::vector<Map*>* GetMap()
{
	return &map;
}
