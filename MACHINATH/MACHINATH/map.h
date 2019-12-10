#pragma once

#include <vector>
#include "mesh.h"

// exit direction of the map
enum class Direction
{
	NORTH, EAST, SOUTH, WEST
};

enum class MapEvent
{
	NONE, CURVE, SLOPE
};

struct EventData
{
	MapEvent mapEvent;
	float distance;
	bool started;
	bool finished;
	float value;
	float speed;
};

struct MapData
{
	MESH_NAME name;
	Direction exit;
	std::vector<EventData> event;
};


// map class
class Map : public MeshObject
{
public:
	int id;
	Direction exit;
	MapData data;

	Map(int id, Transform transform, MapData data, Direction exit, SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) :
		id(id), MeshObject(transform, data.name, type, parent), exit(exit), data(data)
	{
		// disable draw
		enableDraw = false;
		rotLocalAxis = false;
	}

	~Map() {}
};


// returns the vector of map
std::vector<Map*>* GetMap();

// return the map with the given mapId
Map* GetMapById(int mapId);

void InitMap();
void UninitMap();
void UpdateMap();
