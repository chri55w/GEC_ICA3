#include <iostream>
#include <vector>

#pragma once
class CAIMap {
	public:
		CAIMap();
		~CAIMap();

		void parseMap(std::string mapLocation);

	private:
		int mapWidth;
		int mapHeight;

		std::vector<int> mapData;
};

