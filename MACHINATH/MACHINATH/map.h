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
	float value;
	float speed;
	BoxCollider trigger;
	bool started;
	bool finished;
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
	int id;								// id of the map
	std::vector<BoxCollider> col;		// list of colliders attached to the map
	Direction exit;						// exit direction of the map
	MapData data;						// data of the map like events

	// costructor
	Map(int id, Transform transform, MapData data, Direction exit, 
		std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> mapCollider,
		std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> eventCollider,
		SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) :
		id(id), MeshObject(transform, data.name, type, parent), exit(exit), data(data), col(col)
	{
		// disable draw
		enableDraw = false;

		// populate map collider
		col = std::vector<BoxCollider>();
		for (int i = 0; i < mapCollider.size(); ++i)
		{
			col.emplace_back((BoxCollider(this, 
				mapCollider[i].first.x, mapCollider[i].first.y, mapCollider[i].first.z,
				mapCollider[i].second)));
		}

		// pupulate event collider
		for (int i = 0; i < eventCollider.size(); ++i)
		{
			this->data.event[i].trigger = BoxCollider(this,
				eventCollider[i].first.x, eventCollider[i].first.y, eventCollider[i].first.z,
				eventCollider[i].second);
		}
	}

	// destructor
	virtual ~Map() {}

	// override draw from meshobject
	void Draw() override;
};


// returns the vector of map
std::vector<Map*>* GetMap();

// return the map with the given mapId
Map* GetMapById(int mapId);

void InitMap();
void UninitMap();
void UpdateMap();
