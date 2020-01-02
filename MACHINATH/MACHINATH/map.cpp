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


// globals
// EVENTDATA:	event type, player rotation, player rotation speed
static MapData g_MapData[] =
{
	{MESH_MAP_STRAIGHT, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_CURVELEFT, 1, 0, Direction::WEST, std::vector<EventData>{EventData{MapEvent::CURVE, -90, 2.5F}}},
	{MESH_MAP_CURVERIGHT, 1, 0, Direction::EAST, std::vector<EventData>{EventData{MapEvent::CURVE, 90, 2.5F}}},
	{MESH_MAP_STRAIGHT_BRIDGE, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_STRAIGHT_UP, 1, 33, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, -20, 5.0F}, EventData{MapEvent::SLOPE, 20, 5.0F}}},
	{MESH_MAP_STRAIGHT_TUNNEL_DOWN, 2, -52, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, 20, 5.0F}, EventData{MapEvent::SLOPE, -20, 5.0F}}},
	{MESH_MAP_CLIFF, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_START, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
};
std::vector<Map*> g_map;
static float g_mapRadius = 0;
static int g_drawCount;
static float g_poolDistance;

static int g_drawIndex;
static int g_currentMapId;

void UpdateCurrentMap();
void LoadMapFromFile(char* path);
Transform GetStartTransform(const Map& prevMap);
Direction GetExitDirection(const MapData& data, const Direction& lastExit);
std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> GetMapCollider(MESH_NAME mesh, Direction exit);
std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> GetMapEventCollider(MESH_NAME mesh, Direction exit);
std::pair<D3DXVECTOR3, D3DXVECTOR3> GetMapEntranceCollider(MESH_NAME mesh, Direction exit);



void Map::Draw()
{
	if (!enableDraw) return;

	// draw entrance collider
	BoxCollider::DrawCollider(entrance, D3DCOLOR(D3DCOLOR_RGBA(0, 255, 255, 255)));

	// draw map collider
	for (int i = 0; i < col.size(); ++i)
		BoxCollider::DrawCollider(col[i], D3DCOLOR(D3DCOLOR_RGBA(255, 0, 0, 255)));

	// draw trigger collider
	for (int i = 0; i < data.event.size(); ++i)
		BoxCollider::DrawCollider(data.event[i].trigger, D3DCOLOR(D3DCOLOR_RGBA(0, 255, 0, 255)));

	MeshObject::Draw();
}


void InitMap()
{
	// init values
	g_drawCount = 5;
	g_mapRadius = 90.0F;
	g_poolDistance = 160.0F;

	g_drawIndex = 0;
	g_currentMapId = 0;

	// init map
	char path[] = "asset/data/map.txt";
	LoadMapFromFile(path);

	// enable draw for drawcount
	g_drawIndex = g_map.size() < g_drawCount ? g_map.size() : g_drawCount;
	for (int i = 0; i < g_drawIndex; ++i)
	{
		g_map[i]->enableDraw = true;
		ActivatePickup(g_map[i]->id);
	}
}

void UpdateMap()
{
	// update the current map the player is on
	UpdateCurrentMap();

	if (g_map.size() < 1)
	{
		SetScene(SCENE_TITLESCREEN);
		return;
	}

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
		delete g_map[i];

	g_map.clear();
}


void UpdateCurrentMap()
{
	// loop for every map and update the active index if collided with entrance
	for (Map* map : g_map)
	{
		if (map->id != g_currentMapId + 1) continue;

		if (GetPlayer()->col.CheckCollision(map->entrance) != 0)
		{
			g_currentMapId = map->id;
			break;
		}
	}
}


void LoadMapFromFile(char* path)
{
	std::ifstream in(path);
	g_map = std::vector<Map*>();

	int id = 0;
	char c;

	// insert first map
	Transform transform(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	Direction dir = GetExitDirection(g_MapData[7], Direction::NORTH);

	g_map.emplace_back(new Map(id++, transform, g_MapData[7], dir, 
		GetMapEntranceCollider(g_MapData[0].name, dir), GetMapCollider(g_MapData[0].name, dir), 
		GetMapEventCollider(g_MapData[0].name, dir)));
	while (true)
	{
		try
		{
			// read next byte
			in.read(&c, 1);

			// break if end of file is reached
			if (in.eof())
			{
				in.close();
				break;
			}

			// if c == #, loop till next line to skip comment
			if (c == 35)
			{
				do in.read(&c, 1);
				while (!(c == 10));
			}

			// continue if c == \n
			if (c == 10) continue;

			// cast data to int
			int ci = c - '0';

			// check for invalid data
			if (ci < 0 || ci > 9) throw std::runtime_error("Failed to parse map.txt!");

			// get data for next map
			transform = GetStartTransform(*g_map[id - 1]);
			dir = GetExitDirection(g_MapData[ci], dir);

			// populate map
 			g_map.emplace_back(new Map(id++, transform, g_MapData[ci], dir, 
				GetMapEntranceCollider(g_MapData[ci].name, dir), GetMapCollider(g_MapData[ci].name, dir), 
				GetMapEventCollider(g_MapData[ci].name, dir)));
		}
		catch (std::runtime_error& e)
		{
			// display error box and exit program
			MessageBox(NULL,
				e.what(),
				"Error!",
				MB_ICONEXCLAMATION | MB_OK);
			
			std::exit(0);
		}
	}
}

Transform GetStartTransform(const Map& prevMap)
{
	// cache previous map transform
	Transform trans = prevMap.transform;
	float scale = prevMap.data.scale;

	// offset to new location
	trans.position.y += prevMap.data.yOffset;

	if (prevMap.exit == Direction::NORTH)
	{
		trans.position.z += g_mapRadius * scale;
		trans.rotation.y = 0;
	}
	else if (prevMap.exit == Direction::EAST)
	{
		trans.position.x += g_mapRadius * scale;
		trans.rotation.y = 90;
	}
	else if (prevMap.exit == Direction::WEST)
	{
		trans.position.x -= g_mapRadius * scale;
		trans.rotation.y = 270;
	}
	else if (prevMap.exit == Direction::SOUTH)
	{
		trans.position.z -= g_mapRadius * scale;
		trans.rotation.y = 180;
	}

	return trans;
}

// returns a list of collider of given map
std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> GetMapCollider(MESH_NAME mesh, Direction exit)
{
	auto collider = std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>>();

	if (mesh == MESH_MAP_STRAIGHT)
	{
		if (exit == Direction::NORTH || exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 90 }, { 30, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 90 }, { -30, 5, 0 }));
		}
		else
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 90, 20, 10 }, { 0, 5, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 90, 20, 10 }, { 0, 5, -30 }));
		}
	}
	else if (mesh == MESH_MAP_CURVELEFT)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 65 }, { 30, 5, -15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 40, 20, 10 }, { 20, 5, 20 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 55, 20, 10 }, { -20, 5, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 10 }, { -30, 5, -40 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 25, 20, 10 }, { -37.5F, 5, -30 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 65 }, { -30, 5, 15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 40, 20, 10 }, { -20, 5, -20 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 55, 20, 10 }, { 20, 5, -30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 10 }, { 30, 5, 40 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 25, 20, 10 }, { 37.5F, 5, 30 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 60, 20, 10 }, { 20, 5, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 30 }, { -10, 5, 20 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 60 }, { -30, 5, -15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 10 }, { 37, 5, -30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 10 }, { 30, 5, -40 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 60, 20, 10 }, { -20, 5, -30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 30 }, { 10, 5, -20 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 60 }, { 30, 5, 15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 10 }, { -37, 5, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 10 }, { -30, 5, 40 }));
		}
	}
	else if (mesh == MESH_MAP_CURVERIGHT)
	{
		if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 60, 20, 10 }, { -20, 5, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 30 }, { 10, 5, 20 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 60 }, { 30, 5, -15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 10 }, { -37, 5, -30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 10 }, { -30, 5, -40 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 65 }, { -30, 5, -15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 40, 20, 10 }, { -20, 5, 20 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 55, 20, 10 }, { 20, 5, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 10 }, { 30, 5, -40 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 25, 20, 10 }, { 37.5F, 5, -30 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 60, 20, 10 }, { 20, 5, -30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 30 }, { -10, 5, -20 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 60 }, { -30, 5, 15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 10 }, { 37, 5, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 10 }, { 30, 5, 40 }));
		}
		else if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 65 }, { 30, 5, 15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 40, 20, 10 }, { 20, 5, -20 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 55, 20, 10 }, { -20, 5, -30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 10 }, { -30, 5, 40 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 25, 20, 10 }, { -37.5F, 5, 30 }));
		}
	}
	else if (mesh == MESH_MAP_STRAIGHT_UP)
	{
		if (exit == Direction::NORTH || exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 80, 90 }, { 30, 25, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 80, 90 }, { -30, 25, 0 }));
		}
		else
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 90, 80, 10 }, { 0, 25, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 90, 80, 10 }, { 0, 25, -30 }));
		}
	}
	else if (mesh == MESH_MAP_STRAIGHT_TUNNEL_DOWN)
	{
		if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 80, 190 }, { 30, -10, 50 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 80, 190 }, { -30, -10, 50 }));
		}
		else if(exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 80, 190 }, { 30, -10, -50 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 80, 190 }, { -30, -10, -50 }));
		}
		else if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 190, 80, 10 }, { -50, -10, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 190, 80, 10 }, { -50, -10, -30 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 190, 80, 10 }, { 50, -10, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 190, 80, 10 }, { 50, -10, -30 }));
		}
	}

	return collider;
}

// returns a list of event collider of given map
std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> GetMapEventCollider(MESH_NAME mesh, Direction exit)
{
	auto collider = std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>>();

	if (mesh == MESH_MAP_CURVELEFT)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, -20 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, 20 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { 20, 5, 0 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { -20, 5, 0 }));
		}
	}
	else if (mesh == MESH_MAP_CURVERIGHT)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, 20 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, -20 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { -20, 5, 0 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { 20, 5, 0 }));
		}
	}
	else if (mesh == MESH_MAP_STRAIGHT_UP)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { 45, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { -45.3F, 40, 0 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { -45, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { 45.3F, 40, 0 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, 46 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 40, -45.3F }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, -46 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 40, 45.3F }));
		}
	}
	else if (mesh == MESH_MAP_STRAIGHT_TUNNEL_DOWN)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { 5, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { -138, -45, 0 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { -5, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { 138, -45, 0 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, 5 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, -45, -138 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, -5 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, -45, 138 }));

		}
	}

	return collider;
}

// returns the entrace collider of given map
std::pair<D3DXVECTOR3, D3DXVECTOR3> GetMapEntranceCollider(MESH_NAME mesh, Direction exit)
{
	if (mesh == MESH_MAP_STRAIGHT)
	{
		if (exit == Direction::WEST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { 45, 5, 0 });
		else if (exit == Direction::EAST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { -45, 5, 0 });
		else if (exit == Direction::SOUTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, 45 });
		else if (exit == Direction::NORTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, -45 });
	}
	else if (mesh == MESH_MAP_CURVELEFT)
	{
		if (exit == Direction::WEST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, -45 });
		else if (exit == Direction::EAST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, 45 });
		else if (exit == Direction::SOUTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { 45, 5, 0 });
		else if (exit == Direction::NORTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { -45, 5, 0 });
	}
	else if (mesh == MESH_MAP_CURVERIGHT)
	{
		if (exit == Direction::WEST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, 45 });
		else if (exit == Direction::EAST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, -45 });
		else if (exit == Direction::SOUTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { -45, 5, 0 });
		else if (exit == Direction::NORTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { 45, 5, 0 });
	}
	else if (mesh == MESH_MAP_STRAIGHT_UP)
	{
		if (exit == Direction::WEST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { 45, 5, 0 });
		else if (exit == Direction::EAST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { -45, 5, 0 });
		else if (exit == Direction::SOUTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, 45 });
		else if (exit == Direction::NORTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, -45 });
	}
	else if (mesh == MESH_MAP_STRAIGHT_TUNNEL_DOWN)
	{
		if (exit == Direction::WEST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { 45, 5, 0 });
		else if (exit == Direction::EAST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 50 }, { -45, 5, 0 });
		else if (exit == Direction::SOUTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, 45 });
		else if (exit == Direction::NORTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 50, 20, 2 }, { 0, 5, -45 });
	}
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

int GetCurrentMapId()
{
	return g_currentMapId;
}
