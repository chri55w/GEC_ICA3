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
			fileNames.push_back(mapFilesDir + fd.cFileName);
			std::cout << "Found Map: " << fd.cFileName << std::endl;

		} while (FindNextFileA(h, &fd));
		FindClose(h);
	}


	std::cout << "All Maps Loaded" << std::endl;

	return fileNames;
}

void CMapHandler::parseMap(std::string mapName) {

	std::ifstream fileReader;
	fileReader.open(mapFilesDir + mapName);
	if (!fileReader.good()) {
		std::cout << "ERROR: Cannot Find Map File '" + mapFilesDir + mapName + "'" << std::endl;
		STATEHANDLER.changeState("menuState");
		return;
	} else {
		std::cout << "Successfully Loaded Map File '" + mapFilesDir + mapName + "'" << std::endl;
	}
	char c;
	while (fileReader.get(c)) {          // loop getting single characters
		if (c == '@') {
			mapData.push_back(0);
		} else if (c == '.') {
			mapData.push_back(1);
		}
	}
	fileReader.close();
}