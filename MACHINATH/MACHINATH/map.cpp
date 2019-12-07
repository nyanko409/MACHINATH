#include <vector>
#include <tuple>
#include <fstream>
#include <iostream>
#include "map.h"
#include "mesh.h"
#include "playTime.h"
#include "sceneManagement.h"
#include "customMath.h"
#include "player.h"
#include "input.h"
#include "pickup.h"

void LoadMapFromFile(char* path);
Transform GetStartTransform(const Map& prevMap); 
Direction GetExitDirection(const MapData& data, const Direction& lastExit);
void Curve(EventData& event);
void Slope(EventData& event);

// globals
static MapData g_MapData[] =
{
	{MESH_MAP_STRAIGHT, Direction::NORTH, std::vector<EventData>()},
	{MESH_MAP_CURVELEFT, Direction::WEST, std::vector<EventData>{EventData{MapEvent::CURVE, 2.0F, false, false, 90, 1.5F}}},
};
std::vector<EventData> g_event; 
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

	// init map
	char path[] = "asset/data/map.txt";
	LoadMapFromFile(path);

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
	}

	// handle events
	if (g_map.size() > 0 && g_map.front()->data.event.size() > 0)
	{
		// check x or z distance based on local map rotation
		float check = (g_map.front()->GetLocalRotation().y == 90 || g_map.front()->GetLocalRotation().y == 270) ?
					  g_map.front()->GetCombinedPosition().x : g_map.front()->GetCombinedPosition().z;

		for (int i = 0; i < g_map.front()->data.event.size(); ++i)
		{
			// start event based on distance
			if (!g_map.front()->data.event[i].started && g_map.front()->data.event[i].distance >= fabsf(check))
			{
				g_map.front()->data.event[i].started = true;
			}

			// update events
			if (g_map.front()->data.event[i].started && !g_map.front()->data.event[i].finished)
			{
				if (g_map.front()->data.event[i].mapEvent == MapEvent::CURVE)
					Curve(g_map.front()->data.event[i]);
				else if (g_map.front()->data.event[i].mapEvent == MapEvent::SLOPE)
					Slope(g_map.front()->data.event[i]);
			}
		}
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


void Curve(EventData& event)
{
	// get degree to turn this frame
	g_curRot += event.speed;

	float rotBy = g_curRot > fabsf(event.value) ? 
		g_curRot - fabsf(event.value) : event.speed;

	// rotate map
	for (int i = 0; i < g_map.size(); ++i)
	{
		g_map[i]->transform.rotation.y += event.value < 0 ? -rotBy : rotBy;
	}

	// event finished
	if (g_curRot >= fabsf(event.value))
	{
		event.finished = true;
		g_curRot = 0;
	}
}

void Slope(EventData& event)
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
			event.finished = true;
			g_curHeight = 0;
		}
	}

	g_curHeight += event.speed;
}

void LoadMapFromFile(char* path)
{
	std::ifstream in(path);
	g_map = std::vector<Map*>();
	int id = 0;
	char c;

	// first map is always straight road
	Transform transform(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	Direction dir = GetExitDirection(g_MapData[0], Direction::NORTH);
	g_map.emplace_back(new Map(id++, transform, g_MapData[0], dir, SHADER_DEFAULT));
	while (true)
	{
		try
		{
			// read next byte
			in.read(&c, 1);

			// if c == #, loop till next line to skip comment
			if (c == 35)
			{
				do in.read(&c, 1);
				while (!(c == 10));
			}

			// continue if c == \n, break if EOF
			if (c == 10) continue;
			if (in.eof())
			{
				in.close();
				break;
			}

			// cast data to int
			int ci = c - '0';
			if (ci < 0 || ci > 9) throw std::runtime_error("Failed to parse map.txt!");

			// populate map
			transform = GetStartTransform(*g_map[id - 1]);
			dir = GetExitDirection(g_MapData[ci], dir);
			g_map.emplace_back(new Map(id++, transform, g_MapData[ci], dir, SHADER_DEFAULT));
		}
		catch (std::runtime_error& e)
		{
			// display error box
			MessageBox(NULL,
				e.what(),
				"Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
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

Direction GetExitDirection(const MapData& data, const Direction& lastExit)
{
	if (data.exit == Direction::NORTH)
		return lastExit;
	if (lastExit == Direction::NORTH)
		return data.exit;

	if (lastExit == Direction::EAST)
	{
		if (data.exit == Direction::WEST) return Direction::NORTH;
		if (data.exit == Direction::EAST) return Direction::SOUTH;
	}

	if (lastExit == Direction::WEST)
	{
		if (data.exit == Direction::WEST) return Direction::SOUTH;
		if (data.exit == Direction::EAST) return Direction::NORTH;
	}

	if (lastExit == Direction::SOUTH)
	{
		if (data.exit == Direction::WEST) return Direction::EAST;
		if (data.exit == Direction::EAST) return Direction::WEST;
	}
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
