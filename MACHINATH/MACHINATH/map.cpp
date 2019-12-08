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

// globals
static MapData g_MapData[] =
{
	{MESH_MAP_STRAIGHT, Direction::NORTH, std::vector<EventData>({EventData{MapEvent::NONE}})},
	{MESH_MAP_CURVELEFT, Direction::WEST, std::vector<EventData>{EventData{MapEvent::CURVE, 11.0F, false, false, -90, 1.5F}}},
};
std::vector<EventData> g_event; 
std::vector<Map*> g_map;
static float g_mapRadius = 0;
static int g_drawCount;
static float g_poolDistance;

static int g_drawIndex;


void InitMap()
{
	// init values
	g_mapRadius = 90.0F;
	g_drawCount = 3;
	g_poolDistance = 60.0F;

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
	// map pooling
	if (g_map.size() > 0 && GetDistance(g_map[0]->transform.position, GetPlayer()->GetCombinedPosition(), true) > g_poolDistance)
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
