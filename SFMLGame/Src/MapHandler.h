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

		std::vector<std::string> loadAllMapNames(std::string directory, std::string extension);

		void parseMap(std::string mapName, bool forceWallEdge);

		std::vector<mapPixel*> getMapDrawData() const { return mapDrawData; }

		int getMapWidth() const { return mapWidth; }
		int getMapHeight() const { return mapHeight; }

	protected:
		CMapHandler();

	private:
		static CMapHandler *instance_;
		int mapWidth;
		int mapHeight;

		std::vector<char> mapData;
		std::vector<mapPixel*> mapDrawData;

		std::string mapFilesDir = "\\";

		void CMapHandler::parseMapDrawData();
};

#define MAP CMapHandler::getInstance()