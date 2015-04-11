#include "MapHandler.h"

#include "StateHandler.h"
#include <string>
#include <windows.h>
#include <fstream>

//Singleton Implementation.
CMapHandler *CMapHandler::instance_ = NULL;

CMapHandler & CMapHandler::getInstance() {
	if (instance_ == NULL)
		instance_ = new CMapHandler;
	return *instance_;
}

CMapHandler::~CMapHandler() {
	//Clean Up Map Data!
	for (auto m : maps_) {
		for (auto d : m->mapDrawData) {
			delete d;
		}
		delete m;
	}
}

std::vector<std::string> CMapHandler::loadAllMaps(std::string directory, std::string extension) {

	//Clear Vector of file names and print to console search begin.
	fileNames.clear();
	std::cout << "Searching Files" << std::endl;

	//Declare file directory variable
	WIN32_FIND_DATAA fd;

	//Identify directory maps are stored in and filter results 
	//		to files with the provided extension.
	mapFilesDir = directory + "\\";
	std::string filter = mapFilesDir + "*." + extension;

	//Assign a handle to the first file to begin map parsing.
	HANDLE h = FindFirstFileA(filter.c_str(), &fd);
	if (h != INVALID_HANDLE_VALUE) {

		//Push back the first files name to the vector of filenames.
		do {
			fileNames.push_back(fd.cFileName);
			std::cout << "Found Map: " << fd.cFileName << std::endl;
		} while (FindNextFileA(h, &fd));

		//Once all files are found delete the handle and sort the file names into order
		FindClose(h);
		std::stable_sort(fileNames.begin(), fileNames.end());
		for (std::string name : fileNames) {

			//Loop through all map names and parse them into the map data vector
			parseMap(name);
		}
	}

	std::cout << "All Maps Loaded" << std::endl;

	//Return the filenames vector for displaying purposes.
	return fileNames;
}

void CMapHandler::parseMap(std::string mapName) {

	//Create a stream file reader and open the corresponding map to the one provided
	std::ifstream fileReader;
	fileReader.open(mapFilesDir + mapName);

	//Check the file validity, if the file reader .good() function 
	//		returns false then exit the function and print an error.
	if (!fileReader.good()) {
		std::cerr << "ERROR: Cannot Find Map File '" + mapFilesDir + mapName + "'" << std::endl;
		return;
	}

	//Create a new Blank map
	map *newMap = new map();

	//Declare a boolean identifying if the system is yet processing the physical map data.
	bool parsingMapData = false;

	//Loop until the file reader reaches the end of the file.
	while (!fileReader.eof()) {

		//Declare a token string which will be compared to evaluate variables etc.
		std::string token;

		//take the fileReader line as input into token string.
		fileReader >> token;

		//If not yet parsing Map data
		if (!parsingMapData) {

			//if this token equals type then currently do nothing
			if (token == "type") {
				//TODO: Posibly add a check to see if type is equal to octile, otherwise return?

			//If the token is equal to height take the next input and push it to the mapHeight int.
			} else if (token == "height") {
				fileReader >> newMap->mapHeight;

			//If the token is equal to width take the next input and push it to the mapWidth int.
			} else if (token == "width") {
				fileReader >> newMap->mapWidth;

			//If this token is equal to map then the next token will be the map data, 
			//		set parsing map to equal true.
			} else if (token == "map") {	
				parsingMapData = true;
			}

		//If parsing map data then check if mapData size divided by the new maps with is 
		//		less than the new map height.
		} else if ((int)newMap->mapData.size() / newMap->mapWidth < newMap->mapHeight) {

			//check the length of the token taken from the map file, if it is 
			//		less than the map height stated, fill the rest of the line with walls
			while ((int)token.size() < newMap->mapWidth) {
				token.push_back('@');
			}

			//Parse the top, bottom and side lines of the map by pushing data into an alternate vector for 
			//		wall replacement -  Optional setting can be turned off in settings menu.
			if ((int)newMap->mapData.size() < newMap->mapWidth) { //Parsing Top Line
				for (char c : token) {
					newMap->origEdgeData.push_back(c);
				}
			} else if ((int)newMap->mapData.size() >= newMap->mapWidth * (newMap->mapHeight - 1)) { //Parsing Bottom Line
				for (char c : token) {
					newMap->origEdgeData.push_back(c);
				}
			} else if ((int)newMap->mapData.size() % newMap->mapWidth == 0 || newMap->mapData.size() % newMap->mapWidth == newMap->mapWidth - 1) { //Parsing either Left or Right edge
				newMap->origEdgeData.push_back(token.front());
				newMap->origEdgeData.push_back(token.back());
			}

			//push all data into the map data vector too for collision processing etc
			for (char c : token) {
				if (c == '@' || c == '.' || c == 'W' || c == 'T') {
					newMap->mapData.push_back(c);
				}
			}
		}
	}

	//Close the file
	fileReader.close();

	//take the map draw data, this reduces the amount of data processed to create the draw image.
	parseMapDrawData(newMap);

	//Push this new map into the maps vector
	maps_.push_back(newMap);
	std::cout << "Successfully Loaded Map File '" + mapFilesDir + mapName + "'" << std::endl;
}

void CMapHandler::parseMapDrawData(map *mapToParse) {
	mapToParse->mapDrawData.clear();

	//set the starting position
	float currX = 0;
	float currY = 0;

	//Itterate through the map data, increasing the position on each loop, and 
	//		each end of width, take all the relevent data you wish to display 
	//		(Walls, Trees, Water etc.) and assign a colour to that pixel then 
	//		assign its position. skip all '.' as they represent no obstacle.
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
	//redirect current map pointer to the specified map in the maps 
	//		vector (unless it is not found in the vector of filenames).
	for (unsigned int i = 0; i < maps_.size(); i++) {
		if (fileNames.size() > i) {
			if (mapName == fileNames[i]) {
				currentMap = maps_[i]; 
				return;
			}
		}
		
	}
}

void CMapHandler::updateMapEdges(bool forceEdges) {
	//Updates every single map based on the change of the force walls setting.
	//		this method will either force wall characters to every edge pixel or 
	//		replace the current data with the originally loaded data from the map file.
	for (unsigned int mIdx = 0; mIdx < maps_.size(); mIdx++) {

		map* m = maps_[mIdx];

		std::cout << "Updating Edge Collision Data for map: " << fileNames[mIdx] << std::endl;
		if (forceEdges) {
			if ((int)m->mapData.size() >= m->mapHeight * m->mapWidth) {			//Check Map Data contains all data
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
			int origDataOffset = 0;
			int mapDataOffset = 0;
			if ((int)m->mapData.size() >= m->mapHeight * m->mapWidth) {			//Check Map Data contains all data
				for (int i = 0; i < m->mapWidth; i++) {						//TOP
					m->mapData[mapDataOffset] = m->origEdgeData[origDataOffset];
					origDataOffset++; 
					mapDataOffset++;
				}
				for (int i = 0; i < m->mapHeight - 2; i++) {					//EDGES (ignore top and bottom lines that have already been restored)
					m->mapData[mapDataOffset] = m->origEdgeData[origDataOffset];
					mapDataOffset += 511;
					origDataOffset++;
					m->mapData[mapDataOffset] = m->origEdgeData[origDataOffset];
					origDataOffset++;
					mapDataOffset++;
				}
				for (int i = 0; i < m->mapWidth; i++) {						//Bottom
					m->mapData[mapDataOffset] = m->origEdgeData[origDataOffset];
					origDataOffset++;
					mapDataOffset++;
				}
			}
		}
		std::cout << "Updating Edge Draw Data for map: " << fileNames[mIdx] << std::endl;
		parseMapDrawData(m);
		int i = 0;
	}
}





