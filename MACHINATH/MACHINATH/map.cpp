#include <vector>
#include <tuple>
#include "map.h"
#include "mesh.h"
#include "playTime.h"
#include "sceneManagement.h"
#include "player.h"

enum class MapEvent
{
	CURVE_LEFT, CURVE_RIGHT
};

struct EventTime
{
	MapEvent mapEvent;
	float time;
};

typedef std::pair<Transform, int> mapPair;
mapPair GetStartTransform(const Map& prevMap);
float GetDistance(D3DXVECTOR3 t1, D3DXVECTOR3 t2); // returns the distance between two points

// globals
std::vector<EventTime> event;
std::vector<Map*> map;
static float mapRadius=0;
static int drawCount;
static float poolDistance;
static float curveSpeed;

static int drawIndex;
static bool isCurving;
static float curveRot;


void InitMap()
{
	mapRadius = 90.0F;
	drawCount = 3;
	curveSpeed = 1.5F;
	poolDistance = 80.0F;

	isCurving = false;
	curveRot = 0;

	// init event times
	event = std::vector<EventTime>();
	event.emplace_back(EventTime {MapEvent::CURVE_LEFT, 4.5F});
	event.emplace_back(EventTime {MapEvent::CURVE_LEFT, 9.5F});

	// init map
	map = std::vector<Map*>();

	Transform transform(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	map.emplace_back(new Map(transform, MESH_MAP_ROUNDABOUT, Direction::NORTH, 0, SHADER_DEFAULT));

	mapPair pair = GetStartTransform(*map[0]);
	map.emplace_back(new Map(pair.first, MESH_MAP_CURVELEFT, Direction::WEST, pair.second, SHADER_DEFAULT));

	pair = GetStartTransform(*map[1]);
	map.emplace_back(new Map(pair.first, MESH_MAP_CURVELEFT, Direction::SOUTH, pair.second, SHADER_DEFAULT));

	pair = GetStartTransform(*map[2]);
	map.emplace_back(new Map(pair.first, MESH_MAP_ROUNDABOUT, Direction::SOUTH, pair.second, SHADER_DEFAULT));

	pair = GetStartTransform(*map[3]);
	map.emplace_back(new Map(pair.first, MESH_MAP_ROUND, Direction::WEST, pair.second, SHADER_DEFAULT));

	// enable draw for drawcount
	drawIndex = map.size() < drawCount ? map.size() : drawCount;
	for (int i = 0; i < drawIndex; ++i)
	{
		map[i]->enableDraw = true;
	}
}

void UpdateMap()
{
	if (GetScene() != SCENE_GAMESCREEN) return;

	// for every map in array
	for (int i = 0; i < map.size(); ++i)
	{
		// move map by player speed
		map[i]->transform.position -= map[i]->GetForward() * GetPlayer()->moveSpeed;

		// handle events
		if (event.size() > 0 && event.front().time <= playTime)
		{
			// curve right
			if (event.front().mapEvent == MapEvent::CURVE_RIGHT)
			{
				isCurving = true;
				if (curveRot < 90)
				{
					map[i]->transform.rotation.y -= curveSpeed;
				}
				else
				{
					event.erase(event.begin());
					isCurving = false;
					curveRot = 0;
				}
			}

			// curve left
			else if (event.front().mapEvent == MapEvent::CURVE_LEFT)
			{
				isCurving = true;
				if (curveRot < 90)
				{
					map[i]->transform.rotation.y += curveSpeed;
				}
				else
				{
					event.erase(event.begin());
					isCurving = false;
					curveRot = 0;
				}
			}
		}
	}

	// add rotation while curving
	if (isCurving)
		curveRot += curveSpeed;

	// remove first map from array if out of camera view
	if (map.size() > 0 && GetDistance(map[0]->transform.position, GetPlayer()->transform.position) > poolDistance)
	{
		// display next map
		if (map.size() > drawIndex)
		{
			map[drawIndex]->enableDraw = true;
		}

		// delete map
		delete map[0];
		map.erase(map.begin());

	}
}

void UninitMap()
{
	// free memory
	for (int i = 0; i < map.size(); ++i)
	{
		delete map[i];
	}
}


mapPair GetStartTransform(const Map& prevMap)
{
	// cache previous map transform
	Transform trans = prevMap.transform;
	int localY = 0;

	// offset to new location
	if (prevMap.exit == Direction::NORTH)
	{
		trans.position.z += mapRadius;
	}
	else if (prevMap.exit == Direction::EAST)
	{
		trans.position.x += mapRadius;
		localY = 90;
	}
	else if (prevMap.exit == Direction::WEST)
	{
		trans.position.x -= mapRadius;
		localY = 270;
	}
	else if (prevMap.exit == Direction::SOUTH)
	{
		trans.position.z -= mapRadius;
		localY = 180;
	}

	return mapPair(trans, localY);
}

float GetDistance(D3DXVECTOR3 t1, D3DXVECTOR3 t2)
{
	D3DXVECTOR3 dist = t1 - t2;
	return sqrtf(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
}
