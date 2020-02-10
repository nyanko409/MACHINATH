#include <vector>
#include <tuple>
#include <fstream>
#include "map.h"
#include "mesh.h"
#include "playTime.h"
#include "fade.h"
#include "customMath.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "texture.h"
#include "pickup.h"


// globals
// EVENTDATA:	event type, player rotation, player rotation speed
static MapData g_MapData[] =
{
	{MESH_MAP_START, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_METROPOLITAN, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::QTE_SINGLE, 50},EventData{MapEvent::QTE_MULTI, 20, 0.1F}}},
	{MESH_MAP_GREEN_STRAIGHT, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_GREEN_CURVELEFT, 1, 0, Direction::WEST, std::vector<EventData>{EventData{MapEvent::CURVE, -90, 1.0F}}},
	{MESH_MAP_GREEN_CURVERIGHT, 1, 0, Direction::EAST, std::vector<EventData>{EventData{MapEvent::CURVE, 90, 1.0F}}},
	{MESH_MAP_GREEN_STRAIGHT_UP, 1, 33, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, -20, 5.0F}, EventData{MapEvent::SLOPE, 20, 5.0F}}},
	{MESH_MAP_GREEN_STRAIGHT_TUNNEL_DOWN, 2, -52, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, 20, 5.0F}, EventData{MapEvent::SLOPE, -20, 5.0F}}},
	{MESH_MAP_GREEN_HIROBA, 2, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_GREEN_FALLHOLE, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::QTE_SINGLE, 10}}},
	{MESH_MAP_YELLOW_STRAIGHT, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_YELLOW_CURVELEFT, 1, 0, Direction::WEST, std::vector<EventData>{EventData{MapEvent::CURVE, -90, 1.0F}}},
	{MESH_MAP_YELLOW_CURVERIGHT, 1, 0, Direction::EAST, std::vector<EventData>{EventData{MapEvent::CURVE, 90, 1.0F}}},
	{MESH_MAP_YELLOW_STRAIGHT_UP, 1, 33, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, -20, 5.0F}, EventData{MapEvent::SLOPE, 20, 5.0F}}},
	{MESH_MAP_YELLOW_STRAIGHT_TUNNEL_DOWN, 2, -52, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, 20, 5.0F}, EventData{MapEvent::SLOPE, -20, 5.0F}}},
	{MESH_MAP_YELLOW_HIROBA, 2, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_YELLOW_FALLHOLE, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::QTE_SINGLE, 10}}},
	{MESH_MAP_BLUE_STRAIGHT, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_BLUE_CURVELEFT, 1, 0, Direction::WEST, std::vector<EventData>{EventData{MapEvent::CURVE, -90, 1.0F}}},
	{MESH_MAP_BLUE_CURVERIGHT, 1, 0, Direction::EAST, std::vector<EventData>{EventData{MapEvent::CURVE, 90, 1.0F}}},
	{MESH_MAP_BLUE_STRAIGHT_UP, 1, 33, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, -20, 5.0F}, EventData{MapEvent::SLOPE, 20, 5.0F}}},
	{MESH_MAP_BLUE_STRAIGHT_TUNNEL_DOWN, 2, -52, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, 20, 5.0F}, EventData{MapEvent::SLOPE, -20, 5.0F}}},
	{MESH_MAP_BLUE_HIROBA, 2, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_BLUE_FALLHOLE, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::QTE_SINGLE, 10}}},
	{MESH_MAP_BLUE_STRAIGHT_BRIDGE, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_BLUE_CLIFF, 1, -100, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_RED_STRAIGHT, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_RED_CURVELEFT, 1, 0, Direction::WEST, std::vector<EventData>{EventData{MapEvent::CURVE, -90, 1.0F}}},
	{MESH_MAP_RED_CURVERIGHT, 1, 0, Direction::EAST, std::vector<EventData>{EventData{MapEvent::CURVE, 90, 1.0F}}},
	{MESH_MAP_RED_STRAIGHT_UP, 1, 33, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, -20, 5.0F}, EventData{MapEvent::SLOPE, 20, 5.0F}}},
	{MESH_MAP_RED_STRAIGHT_TUNNEL_DOWN, 2, -52, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, 20, 5.0F}, EventData{MapEvent::SLOPE, -20, 5.0F}}},
	{MESH_MAP_RED_HIROBA, 2, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_RED_FALLHOLE, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::QTE_SINGLE, 10}}},
	{MESH_MAP_TWOTONE_STRAIGHT, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_TWOTONE_CURVELEFT, 1, 0, Direction::WEST, std::vector<EventData>{EventData{MapEvent::CURVE, -90, 1.0F}}},
	{MESH_MAP_TWOTONE_CURVERIGHT, 1, 0, Direction::EAST, std::vector<EventData>{EventData{MapEvent::CURVE, 90, 1.0F}}},
	{MESH_MAP_TWOTONE_STRAIGHT_UP, 1, 33, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, -20, 5.0F}, EventData{MapEvent::SLOPE, 20, 5.0F}}},
	{MESH_MAP_TWOTONE_STRAIGHT_TUNNEL_DOWN, 2, -52, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::SLOPE, 20, 5.0F}, EventData{MapEvent::SLOPE, -20, 5.0F}}},
	{MESH_MAP_TWOTONE_HIROBA, 2, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::NONE}}},
	{MESH_MAP_TWOTONE_FALLHOLE, 1, 0, Direction::NORTH, std::vector<EventData>{EventData{MapEvent::QTE_SINGLE, 10}}}
};

std::vector<Map*> g_map;
static float g_mapRadius = 0;
static int g_drawCount;

static int g_drawIndex;
static int g_currentMapId;

MapType GetMapType(MESH_NAME mesh);
void UpdateCurrentMap();
void LoadMapFromFile(char* path);
Transform GetStartTransform(const Map& prevMap);
Direction GetExitDirection(const MapData& data, const Direction& lastExit);
std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> GetMapCollider(MapType mt, Direction exit);
std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> GetMapEventCollider(MapType mt, Direction exit);
std::pair<D3DXVECTOR3, D3DXVECTOR3> GetMapEntranceCollider(MapType mt, Direction exit);



void Map::Draw()
{
#if _DEBUG
	if (!enableDraw) return;

	// draw entrance collider
	BoxCollider::DrawCollider(entrance, D3DCOLOR(D3DCOLOR_RGBA(0, 255, 255, 255)));

	// draw map collider
	for (int i = 0; i < col.size(); ++i)
		BoxCollider::DrawCollider(col[i], D3DCOLOR(D3DCOLOR_RGBA(255, 0, 0, 255)));

	// draw trigger collider
	for (int i = 0; i < data.event.size(); ++i)
		BoxCollider::DrawCollider(data.event[i].trigger, D3DCOLOR(D3DCOLOR_RGBA(0, 255, 0, 255)));
#endif

	//pDevice->SetRenderState(D3DRS_LIGHTING, false);
	MeshObject::Draw();
	//pDevice->SetRenderState(D3DRS_LIGHTING, true);
}


void InitMap()
{
	// init values
	g_drawCount = 5;
	g_mapRadius = 90.0F;

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
	}
}

void UpdateMap()
{
	// return to title after key press
	if(Keyboard_IsTrigger(DIK_R))
	{
		StartFade(AUDIO_BGM_GAME, 0, 3.0F);
		StartFadeToScene(SCENE_TITLESCREEN);
		return;
	}

	// update the current map the player is on
	UpdateCurrentMap();

	// map pooling
	if (!g_map.empty() && g_map.front()->id < g_currentMapId - 1)
	{
		// display next map and pickups
		if (g_map.size() > g_drawIndex)
		{
			g_map[g_drawIndex]->enableDraw = true;
			for (auto child : g_map[g_drawIndex]->child)
			{
				child->enableDraw = true;
			}
		}
	
		// delete pickup and map
		CleanPickup(g_map[0]->id);

		SAFE_DELETE(g_map[0]);
		g_map.erase(g_map.begin());
	}
}

void UninitMap()
{
	// free memory
	for (int i = 0; i < g_map.size(); ++i)
		SAFE_DELETE(g_map[i]);

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
	Direction dir = GetExitDirection(g_MapData[0], Direction::NORTH);

	g_map.emplace_back(new Map(id++, transform, g_MapData[0], MapType::NONE,  dir, 
		GetMapEntranceCollider(MapType::NONE, dir), GetMapCollider(MapType::NONE, dir), 
		GetMapEventCollider(MapType::NONE, dir)));

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

			// continue if its not a number
			if (!(c >= 48 && c <= 57)) continue;

			// get all the digits
			int ci = c - '0';
			while (true)
			{
				in.read(&c, 1);
				if (c == 10 || c == 32 || in.eof()) break;
				if(c < 48 || c > 57) throw std::runtime_error("Failed to parse map.txt!");

				// shift the number to left and append next number to front
				ci *= 10;
				ci += c - '0';
			}

			// display error when given map index is invalid
			if((sizeof(g_MapData) / sizeof(MapData)) <= ci)
				throw std::runtime_error("Given map index is invalid!");

			// get data for next map
			transform = GetStartTransform(*g_map[id - 1]);
			dir = GetExitDirection(g_MapData[ci], dir);
			MapType mt = GetMapType(g_MapData[ci].name);

			// populate map
 			g_map.emplace_back(new Map(id++, transform, g_MapData[ci], mt, dir, 
				GetMapEntranceCollider(mt, dir), GetMapCollider(mt, dir), 
				GetMapEventCollider(mt, dir)));
		}
		catch (std::runtime_error& e)
		{
			// display error box and exit program
			ShowCursor(true);
			MessageBox(NULL,
				e.what(),
				"Error!",
				MB_ICONEXCLAMATION | MB_OK);
			
			std::exit(1);
		}
	}

	g_map.shrink_to_fit();
}

// get map type
MapType GetMapType(MESH_NAME mesh)
{
	if (mesh == MESH_MAP_GREEN_STRAIGHT || mesh == MESH_MAP_YELLOW_STRAIGHT ||
		mesh == MESH_MAP_BLUE_STRAIGHT || mesh == MESH_MAP_RED_STRAIGHT ||
		mesh == MESH_MAP_TWOTONE_STRAIGHT)
	{
		return MapType::STRAIGHT;
	}

	else if (mesh == MESH_MAP_GREEN_CURVELEFT || mesh == MESH_MAP_YELLOW_CURVELEFT ||
		mesh == MESH_MAP_BLUE_CURVELEFT || mesh == MESH_MAP_RED_CURVELEFT ||
		mesh == MESH_MAP_TWOTONE_CURVELEFT)
	{
		return MapType::CURVE_LEFT;
	}

	else if (mesh == MESH_MAP_GREEN_CURVERIGHT || mesh == MESH_MAP_YELLOW_CURVERIGHT ||
		mesh == MESH_MAP_BLUE_CURVERIGHT || mesh == MESH_MAP_RED_CURVERIGHT ||
		mesh == MESH_MAP_TWOTONE_CURVERIGHT)
	{
		return MapType::CURVE_RIGHT;
	}

	else if (mesh == MESH_MAP_GREEN_STRAIGHT_UP || mesh == MESH_MAP_YELLOW_STRAIGHT_UP ||
		mesh == MESH_MAP_BLUE_STRAIGHT_UP || mesh == MESH_MAP_RED_STRAIGHT_UP ||
		mesh == MESH_MAP_TWOTONE_STRAIGHT_UP)
	{
		return MapType::SLOPE;
	}

	else if (mesh == MESH_MAP_GREEN_STRAIGHT_TUNNEL_DOWN || mesh == MESH_MAP_YELLOW_STRAIGHT_TUNNEL_DOWN ||
		mesh == MESH_MAP_BLUE_STRAIGHT_TUNNEL_DOWN || mesh == MESH_MAP_RED_STRAIGHT_TUNNEL_DOWN ||
		mesh == MESH_MAP_TWOTONE_STRAIGHT_TUNNEL_DOWN)
	{
		return MapType::TUNNEL;
	}

	else if (mesh == MESH_MAP_GREEN_FALLHOLE || mesh == MESH_MAP_YELLOW_FALLHOLE ||
		mesh == MESH_MAP_BLUE_FALLHOLE || mesh == MESH_MAP_RED_FALLHOLE ||
		mesh == MESH_MAP_TWOTONE_FALLHOLE)
	{
		return MapType::FALLHOLE;
	}

	else if (mesh == MESH_MAP_GREEN_HIROBA || mesh == MESH_MAP_YELLOW_HIROBA ||
		mesh == MESH_MAP_BLUE_HIROBA || mesh == MESH_MAP_RED_HIROBA ||
		mesh == MESH_MAP_TWOTONE_HIROBA)
	{
		return MapType::HIROBA;
	}

	else if (mesh == MESH_MAP_METROPOLITAN)
		return MapType::METROPOLITAN;

	return MapType::NONE;
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
std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> GetMapCollider(MapType mt, Direction exit)
{
	auto collider = std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>>();

	if (mt == MapType::STRAIGHT)
	{
		if (exit == Direction::NORTH || exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 90 }, { 50, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 90 }, { -50, 5, 0 }));
		}
		else
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 90, 20, 20 }, { 0, 5, 50 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 90, 20, 20 }, { 0, 5, -50 }));
		}
	}

	else if (mt == MapType::CURVE_LEFT)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 65 }, { 45, 5, -15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 40, 20, 10 }, { 35, 5, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 55, 20, 10 }, { -20, 5, 45 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 65 }, { -45, 5, 15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 40, 20, 10 }, { -35, 5, -30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 55, 20, 10 }, { 20, 5, -45 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 60, 20, 10 }, { 20, 5, 45 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 30 }, { -30, 5, 35 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 60 }, { -45, 5, -15 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 60, 20, 10 }, { -20, 5, -45 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 30 }, { 30, 5, -35 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 60 }, { 45, 5, 15 }));
		}
	}

	else if (mt == MapType::CURVE_RIGHT)
	{
		if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 60, 20, 10 }, { -20, 5, 45 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 30 }, { 30, 5, 35 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 60 }, { 45, 5, -15 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 65 }, { -45, 5, -15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 40, 20, 10 }, { -35, 5, 30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 55, 20, 10 }, { 20, 5, 45 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 60, 20, 10 }, { 20, 5, -45 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 30 }, { -30, 5, -35 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 60 }, { -45, 5, 15 }));
		}
		else if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 10, 20, 65 }, { 45, 5, 15 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 40, 20, 10 }, { 35, 5, -30 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 55, 20, 10 }, { -20, 5, -45 }));
		}
	}

	else if (mt == MapType::SLOPE)
	{
		if (exit == Direction::NORTH || exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 80, 90 }, { 50, 25, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 80, 90 }, { -50, 25, 0 }));
		}
		else
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 90, 80, 20 }, { 0, 25, 50 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 90, 80, 20 }, { 0, 25, -50 }));
		}
	}

	else if (mt == MapType::TUNNEL)
	{
		if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 80, 190 }, { 50, -10, 50 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 80, 190 }, { -50, -10, 50 }));
		}
		else if(exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 80, 190 }, { 50, -10, -50 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 80, 190 }, { -50, -10, -50 }));
		}
		else if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 190, 80, 20 }, { -50, -10, 50 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 190, 80, 20 }, { -50, -10, -50 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 190, 80, 20 }, { 50, -10, 50 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 190, 80, 20 }, { 50, -10, -50 }));
		}
	}

	else if (mt == MapType::HIROBA)
	{
		if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 180 }, { 50, 5, 45 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 180 }, { -50, 5, 45 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 180 }, { 50, 5, -45 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 180 }, { -50, 5, -45 }));
		}
		else if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 180, 20, 20 }, { -45, 5, 50 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 180, 20, 20 }, { -45, 5, -50 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 180, 20, 20 }, { 45, 5, 50 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 180, 20, 20 }, { 45, 5, -50 }));
		}
	}

	else if (mt == MapType::FALLHOLE)
	{
		if (exit == Direction::NORTH || exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 90 }, { 50, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 20, 20, 90 }, { -50, 5, 0 }));
		}
		else
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 90, 20, 20 }, { 0, 5, 50 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 90, 20, 20 }, { 0, 5, -50 }));
		}
	}

	return collider;
}

// returns a list of event collider of given map
std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> GetMapEventCollider(MapType mt, Direction exit)
{
	auto collider = std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>>();

	if (mt == MapType::CURVE_LEFT)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, -40 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, 40 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 40, 5, 0 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -40, 5, 0 }));
		}
	}

	else if (mt == MapType::CURVE_RIGHT)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, 30 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, -30 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -30, 5, 0 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 30, 5, 0 }));
		}
	}

	else if (mt == MapType::SLOPE)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 45, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -45.3F, 40, 0 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -45, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 45.3F, 40, 0 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, 46 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 40, -45.3F }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, -46 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 40, 45.3F }));
		}
	}

	else if (mt == MapType::TUNNEL)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 4, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -138, -45, 0 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -4, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 138, -45, 0 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, 4 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, -45, -138 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, -4 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, -45, 138 }));

		}
	}

	else if (mt == MapType::FALLHOLE)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 95, 5, 0 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -95, 5, 0 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, 95 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, -95 }));
		}
	}

	else if (mt == MapType::METROPOLITAN)
	{
		if (exit == Direction::WEST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 300, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 280, 5, 0 }));
		}
		else if (exit == Direction::EAST)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -320, 5, 0 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -300, 5, 0 }));
		}
		else if (exit == Direction::SOUTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, 300 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, 280 }));
		}
		else if (exit == Direction::NORTH)
		{
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 100, 2 }, { 0, 5, -320 }));
			collider.emplace_back(std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 100, 2 }, { 0, 5, -300 }));
		}
	}

	return collider;
}

// returns the entrace collider of given map
std::pair<D3DXVECTOR3, D3DXVECTOR3> GetMapEntranceCollider(MapType mt, Direction exit)
{
	if (mt == MapType::STRAIGHT || mt == MapType::FALLHOLE 
		|| mt == MapType::SLOPE || mt == MapType::TUNNEL)
	{
		if (exit == Direction::WEST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 45, 5, 0 });
		else if (exit == Direction::EAST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -45, 5, 0 });
		else if (exit == Direction::SOUTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, 45 });
		else if (exit == Direction::NORTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, -45 });
	}

	if (mt == MapType::CURVE_LEFT)
	{
		if (exit == Direction::WEST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, -45 });
		else if (exit == Direction::EAST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, 45 });
		else if (exit == Direction::SOUTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 45, 5, 0 });
		else if (exit == Direction::NORTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -45, 5, 0 });
	}

	if (mt == MapType::CURVE_RIGHT)
	{
		if (exit == Direction::WEST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, 45 });
		else if (exit == Direction::EAST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 100, 20, 2 }, { 0, 5, -45 });
		else if (exit == Direction::SOUTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { -45, 5, 0 });
		else if (exit == Direction::NORTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 100 }, { 45, 5, 0 });
	}

	if (mt == MapType::HIROBA)
	{
		if (exit == Direction::WEST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 180 }, { 45, 5, 0 });
		else if (exit == Direction::EAST)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 2, 20, 180 }, { -45, 5, 0 });
		else if (exit == Direction::SOUTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 180, 20, 2 }, { 0, 5, 45 });
		else if (exit == Direction::NORTH)
			return std::pair<D3DXVECTOR3, D3DXVECTOR3>({ 180, 20, 2 }, { 0, 5, -45 });
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
