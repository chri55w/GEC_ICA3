#include "AIMap.h"

#include <string>
#include <windows.h>


CAIMap::CAIMap() {
}


CAIMap::~CAIMap() {
}

void CAIMap::loadAllFiles() {
	WIN32_FIND_DATAA fd;

	std::string  directory = "..\\Maps\\";
	std::string  filter = directory + "*.map";

	HANDLE h = FindFirstFileA(filter.c_str(), &fd);
	if (h != INVALID_HANDLE_VALUE) {
		do {
			fileNames.push_back(fd.cFileName);

		} while (FindNextFileA(h, &fd));
		FindClose(h);
	}

	int count = fileNames.size();
	std::cout << std::to_string(count) << std::endl;

}