#include "MapHandler.h"

#include "StateHandler.h"
#include <string>
#include <windows.h>
#include <fstream>

CMapHandler *CMapHandler::instance_ = NULL;

CMapHandler & CMapHandler::getInstance() {
	if (instance_ == NULL)
		instance_ = new CMapHandler;
	return *instance_;
}

CMapHandler::CMapHandler() {

}


CMapHandler::~CMapHandler() {

}

std::vector<std::string> CMapHandler::loadAllMapNames(std::string directory, std::string extension) {
	std::cout << "Searching Files" << std::endl;
	WIN32_FIND_DATAA fd;
	std::vector<std::string> fileNames;

	mapFilesDir = directory + "\\";
	std::string filter = mapFilesDir + "*." + extension;

	HANDLE h = FindFirstFileA(filter.c_str(), &fd);
	if (h != INVALID_HANDLE_VALUE) {
		do {
			fileNames.push_back(fd.cFileName);
			std::cout << "Found Map: " << fd.cFileName << std::endl;

		} while (FindNextFileA(h, &fd));
		FindClose(h);
	}


	std::cout << "All Maps Loaded" << std::endl;

	return fileNames;
}

void CMapHandler::parseMap(std::string mapName, bool forceWallEdge) {
	std::ifstream fileReader;

	fileReader.open(mapFilesDir + mapName);
	if (!fileReader.good()) {
		std::cerr << "ERROR: Cannot Find Map File '" + mapFilesDir + mapName + "'" << std::endl;
		STATEHANDLER.changeState("menuState");
		return;
	} else {
		std::cout << "Successfully Loaded Map File '" + mapFilesDir + mapName + "'" << std::endl;
	}
	bool parsingMapData = false;
	while (!fileReader.eof()) {
		std::string token;

		fileReader >> token;
		if (!parsingMapData) {
			if (token == "type") {
				//getMapType
			} else if (token == "height") {
				fileReader >> mapHeight;
			} else if (token == "width") {
				fileReader >> mapWidth;
			} else if (token == "map") {	
				parsingMapData = true;
			}
		} else if ((int)mapData.size() / mapWidth < mapHeight) {
			while ((int)token.size() < mapWidth) {
				token.push_back('@');
			}
			for (char c : token) {
				if (forceWallEdge) {
					if ((int)mapData.size() < mapWidth) { //Parsing Top Line
						mapData.push_back('@');
						continue;
					} else if ((int)mapData.size() % mapWidth == 0 || mapData.size() % mapWidth == mapWidth - 1) { //Parsing either Left or Right edge
						mapData.push_back('@');
						continue;
					} else if (mapData.size() > mapWidth * (mapHeight-1)) { //Parsing Bottom Line
						mapData.push_back('@');
						continue;
					}
				}
				if (c == '@' || c == '.' || c == 'W' || c == 'T') {
					mapData.push_back(c);
				}
			}
		}
	}
	fileReader.close();
	parseMapDrawData();
}

void CMapHandler::parseMapDrawData() {

	float currX = 0;
	float currY = 0;

	for (char data : mapData) {
		if (data != '.') {
			mapPixel *newMapPixel = new mapPixel();
			newMapPixel->x_pos = currX;
			newMapPixel->y_pos = currY;
			if (data == '@') {
				newMapPixel->pixel_colour = sf::Color::Red;
			} else if (data == 'W') {
				newMapPixel->pixel_colour = sf::Color::Blue;
			} else if (data == 'T') {
				newMapPixel->pixel_colour = sf::Color::Yellow;
			}
			mapDrawData.push_back(newMapPixel);
		}
		
		if (currX < 512-1) {
			currX++;
		} else {
			currX = 0;
			currY++;
		}
	}

}