#pragma once

#include <vector>
#include "mesh.h"
#include "collider.h"
#include "cameraevent.h"


// map type
enum class MapType
{
	STRAIGHT, CURVE_LEFT, CURVE_RIGHT, FALLHOLE, TUNNEL, SLOPE, HIROBA, METROPOLITAN,
	NONE
};

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
	CameraEventData camEvent;
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
private:
	float m_dissolveValue;
	D3DXVECTOR3 m_dissolveColor;

public:
	int id;								// id of the map
	BoxCollider entrance;				// entrance collider of the map
	std::vector<BoxCollider> col;		// list of map colliders
	Direction exit;						// exit direction of the map
	MapData data;						// data of the map like events
	MapType mapType;					// type of the map


	// costructor
	Map(int id, Transform transform, MapData data, MapType mapType, Direction exit,
		const std::pair<D3DXVECTOR3, D3DXVECTOR3>& entranceCollider,
		const std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>>& mapCollider,
		const std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>>& eventCollider,
		SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) :
		id(id), MeshObject(transform, data.name, type, parent), exit(exit), data(data), col(col), mapType(mapType),
		m_dissolveValue(1)
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

		// init dissolve color
		m_dissolveColor = { 0,0,0 };

		if (data.name == MESH_MAP_BLUE_CURVELEFT || data.name ==   MESH_MAP_BLUE_CURVERIGHT ||
			data.name == MESH_MAP_BLUE_FALLHOLE || data.name ==    MESH_MAP_BLUE_STRAIGHT ||
			data.name == MESH_MAP_BLUE_STRAIGHT_UP || data.name == MESH_MAP_BLUE_STRAIGHT_TUNNEL_DOWN ||
			data.name == MESH_MAP_BLUE_HIROBA)
			m_dissolveColor = { 0,0,1 };

		if (data.name == MESH_MAP_RED_CURVELEFT || data.name ==   MESH_MAP_RED_CURVERIGHT ||
			data.name == MESH_MAP_RED_FALLHOLE || data.name ==    MESH_MAP_RED_STRAIGHT ||
			data.name == MESH_MAP_RED_STRAIGHT_UP || data.name == MESH_MAP_RED_STRAIGHT_TUNNEL_DOWN ||
			data.name == MESH_MAP_RED_HIROBA)
			m_dissolveColor = { 1,0,0 };

		if (data.name == MESH_MAP_YELLOW_CURVELEFT || data.name ==   MESH_MAP_YELLOW_CURVERIGHT ||
			data.name == MESH_MAP_YELLOW_FALLHOLE || data.name ==    MESH_MAP_YELLOW_STRAIGHT ||
			data.name == MESH_MAP_YELLOW_STRAIGHT_UP || data.name == MESH_MAP_YELLOW_STRAIGHT_TUNNEL_DOWN ||
			data.name == MESH_MAP_YELLOW_HIROBA)
			m_dissolveColor = {1,1,0 };

		if (data.name == MESH_MAP_GREEN_CURVELEFT || data.name ==   MESH_MAP_GREEN_CURVERIGHT ||
			data.name == MESH_MAP_GREEN_FALLHOLE || data.name ==    MESH_MAP_GREEN_STRAIGHT ||
			data.name == MESH_MAP_GREEN_STRAIGHT_UP || data.name == MESH_MAP_GREEN_STRAIGHT_TUNNEL_DOWN ||
			data.name == MESH_MAP_GREEN_HIROBA)
			m_dissolveColor = { 0,1,0 };

		if (data.name == MESH_MAP_TWOTONE_CURVELEFT || data.name ==   MESH_MAP_TWOTONE_CURVERIGHT ||
			data.name == MESH_MAP_TWOTONE_FALLHOLE || data.name ==    MESH_MAP_TWOTONE_STRAIGHT ||
			data.name == MESH_MAP_TWOTONE_STRAIGHT_UP || data.name == MESH_MAP_TWOTONE_STRAIGHT_TUNNEL_DOWN ||
			data.name == MESH_MAP_TWOTONE_HIROBA)
			m_dissolveColor = { 1,1,1 };

		if(data.name == MESH_MAP_METROPOLITAN)
			m_dissolveColor = { 0,0,0 };

		if (data.name == MESH_MAP_START)
			m_dissolveColor = {1,1,1};
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
