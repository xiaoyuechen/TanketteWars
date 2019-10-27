#pragma once
#include "Actors/Actor.h"
#include "ResourceManagers/ResourceManager.h"

#include <string>
#include <vector>


using namespace mw;
namespace tankett
{
class Map
{
public:
	enum ID
	{
		DefaultMap,
	};

public:
	Map();

	bool loadFromFile(const ::std::string& filename);

	::std::unique_ptr<Actor> buildMap(float width, float height, const ::sf::Texture& texture);

	::sf::Vector2i getRandomEmptyTile();

private:
	size_t mRow;
	size_t mCol;
	::std::vector<bool> mMatrix;
};

typedef ResourceManager<Map, Map::ID> MapManager;
}