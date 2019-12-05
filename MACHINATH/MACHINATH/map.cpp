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
	CURVE, SLOPE
};

struct EventTime
{
	MapEvent mapEvent;
	float value;
	float speed;
	float time;
};

Transform GetStartTransform(const Map& prevMap);
void Curve(const EventTime& event);
void Slope(const EventTime& event);
void MoveSideways(int index);

// globals
std::vector<EventTime> g_event; 
std::vector<Map*> g_map;
static float g_mapRadius = 0;
static int g_drawCount;
static float g_poolDistance;

static int g_drawIndex;
static float g_curRot, g_curHeight;


void InitMap()
{
	// init values
	g_mapRadius = 90.0F;
	g_drawCount = 3;
	g_poolDistance = 60.0F;

	g_curRot = 0;
	g_curHeight = 0;
	g_drawIndex = 0;

	// init event times
	g_event = std::vector<EventTime>();
	g_event.emplace_back(EventTime {MapEvent::CURVE, 90, 1.5F, 4.5F});
	g_event.emplace_back(EventTime {MapEvent::CURVE, 90, 1.5F, 9.5F});
	g_event.emplace_back(EventTime {MapEvent::SLOPE, -17, 0.18F, 17.0F});
	g_event.emplace_back(EventTime{ MapEvent::CURVE, 270, 1.0F, 20.0F});

	// init map
	g_map = std::vector<Map*>();
	int mapId = 0;

	Transform transform(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	g_map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, SHADER_DEFAULT));

	transform = GetStartTransform(*g_map[0]);
	g_map.emplace_back(new Map(mapId++, transform, MESH_MAP_CURVELEFT, Direction::WEST, SHADER_DEFAULT));

	transform = GetStartTransform(*g_map[1]);
	g_map.emplace_back(new Map(mapId++, transform, MESH_MAP_CURVELEFT, Direction::SOUTH, SHADER_DEFAULT));

	transform = GetStartTransform(*g_map[2]);
	g_map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUNDABOUT, Direction::SOUTH, SHADER_DEFAULT));

	transform = GetStartTransform(*g_map[3]);
	g_map.emplace_back(new Map(mapId++, transform, MESH_MAP_ROUND, Direction::WEST, SHADER_DEFAULT));

	// enable draw for drawcount
	g_drawIndex = g_map.size() < g_drawCount ? g_map.size() : g_drawCount;
	for (int i = 0; i < g_drawIndex; ++i)
	{
		g_map[i]->enableDraw = true;
	}
}

void UpdateMap()
{
	if (GetScene() != SCENE_GAMESCREEN) return;

	// for every map in array
	for (int i = 0; i < g_map.size(); ++i)
	{
		// move map by player speed
		g_map[i]->transform.position -= g_map[i]->GetForward() * GetPlayer()->moveSpeed;

		// move sideways
		MoveSideways(i);
	}

	// handle events
	if (g_event.size() > 0 && g_event.front().time <= playTime)
	{
		// curve
		if (g_event.front().mapEvent == MapEvent::CURVE)
			Curve(g_event.front());

		// slope
		else if (g_event.front().mapEvent == MapEvent::SLOPE)
			Slope(g_event.front());
	}

	// map pooling
	if (g_map.size() > 0 && GetDistance(g_map[0]->transform.position, GetPlayer()->transform.position, true) > g_poolDistance)
	{
		// display next map and pickups
		if (g_map.size() > g_drawIndex)
		{
			g_map[g_drawIndex]->enableDraw = true;
			ActivatePickup(g_map[g_drawIndex]->id);
		}

		// delete pickup
		CleanPickup(g_map[0]->id);

		// delete map
		delete g_map[0];
		g_map.erase(g_map.begin());
	}
}

void UninitMap()
{
	// free memory
	for (int i = 0; i < g_map.size(); ++i)
	{
		delete g_map[i];
	}
}


void Curve(const EventTime& event)
{
	// loop for every map
	for (int i = 0; i < g_map.size(); ++i)
	{
		// rotate by curRot
		if (g_curRot < fabsf(event.value))
		{
			g_map[i]->transform.rotation.y += event.value < 0 ? -event.speed : event.speed;
		}
		else
		{
			g_event.erase(g_event.begin());
			g_curRot = 0;
		}
	}

	g_curRot += event.speed;
}

void Slope(const EventTime& event)
{
	// loop for every map
	for (int i = 0; i < g_map.size(); ++i)
	{
		// move by curHeight
		if (g_curHeight < fabsf(event.value))
		{
			g_map[i]->transform.position.y += event.value < 0 ? -event.speed : event.speed;
		}
		else
		{
			g_event.erase(g_event.begin());
			g_curHeight = 0;
		}
	}

	g_curHeight += event.speed;
}

void MoveSideways(int index)
{
	// move left and right
	if (Keyboard_IsPress(DIK_F))
	{
		D3DXMATRIX rot;
		D3DXVECTOR3 left;
		D3DXMatrixRotationY(&rot, D3DXToRadian(-90));
		D3DXVec3TransformCoord(&left, &g_map[index]->GetForward(), &rot);
		g_map[index]->transform.position -= left * GetPlayer()->moveSpeed;
	}
	if (Keyboard_IsPress(DIK_G))
	{
		D3DXMATRIX rot;
		D3DXVECTOR3 left;
		D3DXMatrixRotationY(&rot, D3DXToRadian(90));
		D3DXVec3TransformCoord(&left, &g_map[index]->GetForward(), &rot);
		g_map[index]->transform.position -= left * GetPlayer()->moveSpeed;
	}
}

Transform GetStartTransform(const Map& prevMap)
{
	// cache previous map transform
	Transform trans = prevMap.transform;

	// offset to new location
	if (prevMap.exit == Direction::NORTH)
	{
		trans.position.z += g_mapRadius;
		trans.localRotation.y = 0;
	}
	else if (prevMap.exit == Direction::EAST)
	{
		trans.position.x += g_mapRadius;
		trans.localRotation.y = 90;
	}
	else if (prevMap.exit == Direction::WEST)
	{
		trans.position.x -= g_mapRadius;
		trans.localRotation.y = 270;
	}
	else if (prevMap.exit == Direction::SOUTH)
	{
		trans.position.z -= g_mapRadius;
		trans.localRotation.y = 180;
	}

	return trans;
}

Map* GetMapById(int mapId)
{
	for (int i = 0; i < g_map.size(); ++i)
	{
		if (g_map[i]->id == mapId) 
			return g_map[i];
	}

	return nullptr;
}

std::vector<Map*>* GetMap()
{
	return &g_map;
}
