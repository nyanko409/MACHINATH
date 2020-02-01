#pragma once

#include <vector>
#include "mesh.h"
#include "collider.h"
#include "cameraevent.h"


// exit direction of the map
enum class Direction
{
	NORTH, EAST, SOUTH, WEST
};

enum class MapEvent
{
	NONE, CURVE, SLOPE, QTE_SINGLE, QTE_MULTI
};

struct EventData
{
	MapEvent mapEvent;
	float value;
	float speed;
	BoxCollider trigger;
	bool started;
	bool finished;
	float curValue;
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
	BoxCollider entrance;				// entrance collider of the map
	CameraEvent camEvent;				// camera event
	std::vector<BoxCollider> col;		// list of map colliders
	Direction exit;						// exit direction of the map
	MapData data;						// data of the map like events

	// costructor
	Map(int id, Transform transform, MapData data, Direction exit, 
		const std::pair<D3DXVECTOR3, D3DXVECTOR3>& entranceCollider,
		const std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>>& mapCollider,
		const std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>>& eventCollider,
		const std::pair<D3DXVECTOR3, D3DXVECTOR3>& cameraCollider,
		const CameraEventData& cameraEventData,
		SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) :
		id(id), MeshObject(transform, data.name, type, parent), exit(exit), data(data), col(col)
	{
		// disable draw
		enableDraw = false;

		// set entrance collider
		entrance = BoxCollider(this,
			entranceCollider.first.x, entranceCollider.first.y, entranceCollider.first.z,
			entranceCollider.second, true);

		// populate map collider
		col = std::vector<BoxCollider>();
		for (int i = 0; i < mapCollider.size(); ++i)
		{
			col.emplace_back((BoxCollider(this, 
				mapCollider[i].first.x, mapCollider[i].first.y, mapCollider[i].first.z,
				mapCollider[i].second, true)));
		}

		// pupulate event collider
		for (int i = 0; i < eventCollider.size(); ++i)
		{
			this->data.event[i].trigger = BoxCollider(this,
				eventCollider[i].first.x, eventCollider[i].first.y, eventCollider[i].first.z,
				eventCollider[i].second, true);
		}

		// polulate camera collider
		camEvent.trigger = BoxCollider(this,
			cameraCollider.first.x, cameraCollider.first.y, cameraCollider.first.z,
			cameraCollider.second, true);
		camEvent.data = cameraEventData;
	}

	// destructor
	virtual ~Map() {}

	// override draw from meshobject
	void Draw() override;
};


// returns the vector of map
std::vector<Map*>* GetMap();

// returns the map with the given mapId
Map* GetMapById(int mapId);

// returns the current map id the player is on
int GetCurrentMapId();

void InitMap();
void UninitMap();
void UpdateMap();
