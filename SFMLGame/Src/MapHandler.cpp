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

std::vector<std::string> CMapHandler::loadAllMaps(std::string directory, std::string extension) {
	fileNames.clear();
	std::cout << "Searching Files" << std::endl;
	WIN32_FIND_DATAA fd;

	mapFilesDir = directory + "\\";
	std::string filter = mapFilesDir + "*." + extension;

	HANDLE h = FindFirstFileA(filter.c_str(), &fd);
	if (h != INVALID_HANDLE_VALUE) {
		do {
			fileNames.push_back(fd.cFileName);
			std::cout << "Found Map: " << fd.cFileName << std::endl;
		} while (FindNextFileA(h, &fd));
		FindClose(h);
		std::stable_sort(fileNames.begin(), fileNames.end());
		for (std::string name : fileNames) {
			parseMap(name);
		}
	}

	std::cout << "All Maps Loaded" << std::endl;

	return fileNames;
}

void CMapHandler::parseMap(std::string mapName) {
	std::ifstream fileReader;

	fileReader.open(mapFilesDir + mapName);
	if (!fileReader.good()) {
		std::cerr << "ERROR: Cannot Find Map File '" + mapFilesDir + mapName + "'" << std::endl;
		STATEHANDLER.changeState("menuState");
		return;
	} else {
		std::cout << "Successfully Loaded Map File '" + mapFilesDir + mapName + "'" << std::endl;
	}
	map *newMap = new map();
	bool parsingMapData = false;
	while (!fileReader.eof()) {
		std::string token;

		fileReader >> token;
		if (!parsingMapData) {
			if (token == "type") {
				//getMapType
			} else if (token == "height") {
				fileReader >> newMap->mapHeight;
			} else if (token == "width") {
				fileReader >> newMap->mapWidth;
			} else if (token == "map") {	
				parsingMapData = true;
			}
		} else if ((int)newMap->mapData.size() / newMap->mapWidth < newMap->mapHeight) {
			while ((int)token.size() < newMap->mapWidth) {
				token.push_back('@');
			}
			if ((int)newMap->mapData.size() < newMap->mapWidth) { //Parsing Top Line
				for (char c : token) {
					newMap->origEdgeData.push_back(c);
				}
			} else if (newMap->mapData.size() > newMap->mapWidth * (newMap->mapHeight - 1)) { //Parsing Bottom Line
				for (char c : token) {
					newMap->origEdgeData.push_back(c);
				}
			} else if ((int)newMap->mapData.size() % newMap->mapWidth == 0 || newMap->mapData.size() % newMap->mapWidth == newMap->mapWidth - 1) { //Parsing either Left or Right edge
				newMap->origEdgeData.push_back(token.front());
				newMap->origEdgeData.push_back(token.back());
			}
			for (char c : token) {
				if (c == '@' || c == '.' || c == 'W' || c == 'T') {
					newMap->mapData.push_back(c);
				}
			}
		}
	}
	fileReader.close();
	parseMapDrawData(newMap);
	maps_.push_back(newMap);
}

void CMapHandler::parseMapDrawData(map *mapToParse) {

	float currX = 0;
	float currY = 0;

	for (char data : mapToParse->mapData) {
		if (data != '.') {
			mapPixel *newMapPixel = new mapPixel();
			newMapPixel->x_pos = currX;
			newMapPixel->y_pos = currY;
			if (data == '@') {
				newMapPixel->pixel_colour = sf::Color::Black;
			} else if (data == 'W') {
				newMapPixel->pixel_colour = sf::Color::Blue;
			} else if (data == 'T') {
				newMapPixel->pixel_colour = sf::Color::Green;
			}
			mapToParse->mapDrawData.push_back(newMapPixel);
		}
		
		if (currX < mapToParse->mapWidth-1) {
			currX++;
		} else {
			currX = 0;
			currY++;
		}
	}

}

void CMapHandler::setCurrentMap(std::string mapName) {
	for (int i = 0; i < maps_.size(); i++) {
		if (fileNames.size() > i) {
			if (mapName == fileNames[i]) {
				currentMap = maps_[i]; 
				return;
			}
		}
		
	}
}

void CMapHandler::updateMapEdges(bool forceEdges) {
	for (int i = 0; i < maps_.size(); i++) {

		map* m = maps_[i];

		std::cout << "Updating Edge Collision Data for map: " << fileNames[i] << std::endl;
		if (forceEdges) {
			if (m->mapData.size() >= m->mapHeight * m->mapWidth) {			//Check Map Data contains all data
				for (int i = 0; i < m->mapWidth; i++) {						//TOP
					m->mapData[i] = '@';
				}
				int mapPixels = m->mapWidth * m->mapHeight;
				for (int i = mapPixels-1; i > mapPixels - m->mapWidth-1; i--) { //BOTTOM
					m->mapData[i] = '@';
				}
				for (int i = 0; i < m->mapHeight; i++) {					//EDGES
					int index = i * m->mapWidth;
					m->mapData[index] = '@';
					m->mapData[index + m->mapWidth - 1] = '@';
				}
			}

		} else {

		}
		std::cout << "Updating Edge Draw Data for map: " << fileNames[i] << std::endl;
		parseMapDrawData(m);
	}
}

/*

if ((int)newMap->mapData.size() < newMap->mapWidth) { //Parsing Top Line
	for (char c : token) {
		newMap->origEdgeData.push_back(c);
	}
	continue;
} else if ((int)newMap->mapData.size() % newMap->mapWidth == 0 || newMap->mapData.size() % newMap->mapWidth == newMap->mapWidth - 1) { //Parsing either Left or Right edge
	newMap->origEdgeData.push_back(token.front());
	newMap->origEdgeData.push_back(token.back());
	continue;
} else if (newMap->mapData.size() > newMap->mapWidth * (newMap->mapHeight - 1)) { //Parsing Bottom Line
	for (char c : token) {
		newMap->origEdgeData.push_back(c);
	}
	continue;
}

*/