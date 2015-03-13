#include <iostream>
#include <vector>

#pragma once
class CMapHandler {
	public:
		static CMapHandler &getInstance();
		~CMapHandler();

		std::vector<std::string> loadAllMapNames(std::string directory, std::string extension);

		void parseMap(std::string mapName);

	protected:
		CMapHandler();

	private:
		static CMapHandler *instance_;
		int mapWidth;
		int mapHeight;

		std::vector<int> mapData;

		std::string mapFilesDir = "\\";
};

#define MAP CMapHandler::getInstance()