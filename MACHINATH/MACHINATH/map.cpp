#include <vector>
#include <tuple>
#include "map.h"
#include "mesh.h"
#include "player.h"


typedef std::pair<Transform, int> mapPair;
mapPair GetStartTransform(const Map& prevMap);

// globals
std::vector<Map*> map;
float mapRadius = 160;


void InitMap()
{
	// init map
	map = std::vector<Map*>();

	Transform transform(D3DXVECTOR3(35, -1, 80), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(5, 8, 10));
	int yOffset = 90;
	map.emplace_back(new Map(transform, MESH_NEOSHINJYUKU2, Direction::EAST, yOffset, SHADER_DEFAULT));

	mapPair pair = GetStartTransform(*map[0]);
	map.emplace_back(new Map(pair.first, MESH_NEOSHINJYUKU2, Direction::NORTH, yOffset + pair.second, SHADER_DEFAULT));

	pair = GetStartTransform(*map[1]);
	map.emplace_back(new Map(pair.first, MESH_NEOSHINJYUKU2, Direction::EAST, yOffset + pair.second, SHADER_DEFAULT));
}

void UpdateMap()
{
	// for every map in array
	for (int i = 0; i < map.size(); ++i)
	{
		// move map by player speed
		//map[i]->transform.position.z -= GetPlayer()->moveSpeed;

		// rotate map
		//map[i]->transform.rotation.y += 0.5F;
	}

	// remove first map from array if out of camera view
	if (map.size() > 0 && map.front()->transform.position.z < -mapRadius)
	{
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

	return mapPair(trans, localY);
}
