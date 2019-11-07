#include "..\include\EliminationHeatMap.h"
#include <sstream>


bool server::EliminationHeatMap::loadFromFile(const::std::string& filename)
{
	mFileName = filename;

	::std::ifstream file(filename);
	if (!file.is_open())
		return false;

	::std::string line;
	while (getline(file, line))
	{
		::std::istringstream iss(line);
		int x, y;
		uint32_t elim;
		iss >> x;
		iss >> y;
		iss >> elim;
		mHeatMap[Tile{ x, y }] = elim;
	}

	file.close();
	return true;
}

void server::EliminationHeatMap::addKillAtTile(int col, int row)
{
	mHeatMap[{col, row}]++;
	::std::remove(mFileName.c_str());

	::std::ofstream ofs;
	ofs.open(mFileName, std::ofstream::out);
	for (const auto& pair : mHeatMap)
	{
		ofs << "(" << pair.first.x << ", " << pair.first.y << "): "
			<< pair.second << ::std::endl;
	}
	ofs.close();
}

bool server::EliminationHeatMap::Tile::operator<(const Tile& rhs) const
{
	return x < rhs.x || y < rhs.y;
}
