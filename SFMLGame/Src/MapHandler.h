#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#pragma once
class CMapHandler {
	public:
		struct mapPixel {
			float x_pos;
			float y_pos;
			sf::Color pixel_colour;
		};
		static CMapHandler &getInstance();
		~CMapHandler();

		std::vector<std::string> loadAllMaps(std::string directory, std::string extension);
		
		std::vector<mapPixel*> getMapDrawData() const { if (currentMap != nullptr) { return currentMap->mapDrawData; } else { std::vector < mapPixel* > emptyDraw; return emptyDraw; } }
		char getMapData(int offset) const { if (currentMap != nullptr) { return currentMap->mapData[offset]; } else { return '@'; } }

		int getMapWidth() const { if (currentMap != nullptr) { return currentMap->mapWidth; } else { return 0; } }
		int getMapHeight() const { if (currentMap != nullptr) { return currentMap->mapHeight; } else { return 0; } }

		void setCurrentMap(std::string mapName);

		void updateMapEdges(bool forcedEdges);

	protected:
		CMapHandler() {};

	private:
		static CMapHandler *instance_;
		struct map {
			std::vector<char> mapData;
			std::vector<mapPixel*> mapDrawData;
			std::vector<char> origEdgeData;
			int mapHeight;
			int mapWidth;
		};

		std::vector<map*> maps_;

		map* currentMap = nullptr;
		std::string mapFilesDir = "\\";
		std::vector<std::string> fileNames;

		void parseMap(std::string mapName);
		void parseMapDrawData(map *mapToParse);
};

#define MAP CMapHandler::getInstance()