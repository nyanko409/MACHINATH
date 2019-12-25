#pragma once

#include <vector>
#include "mesh.h"
#include "collider.h"

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
	float value2;
	float speed2;
};

struct MapData
{
	MESH_NAME name;
	float scale;
	float yOffset;
	Direction exit;
	std::vector<EventData> event;
};


// map class
class Map : public MeshObject
{
public:
	int id;
	std::vector<BoxCollider> col;
	Direction exit;
	MapData data;

	Map(int id, Transform transform, MapData data, Direction exit, 
		std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> collider, SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) :
		id(id), MeshObject(transform, data.name, type, parent), exit(exit), data(data), col(col)
	{
		// disable draw
		enableDraw = false;

		// init collider
		col = std::vector<BoxCollider>();

		for (int i = 0; i < collider.size(); ++i)
		{
			col.emplace_back((BoxCollider(this, 
				collider[i].first.x, collider[i].first.y, collider[i].first.z, 
				collider[i].second)));
		}
	}

	~Map() {}

	void Draw() override;
};


// returns the vector of map
std::vector<Map*>* GetMap();

// return the map with the given mapId
Map* GetMapById(int mapId);

void InitMap();
void UninitMap();
void UpdateMap();
