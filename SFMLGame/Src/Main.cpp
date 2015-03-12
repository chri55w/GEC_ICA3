#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC


#include <SFML/Graphics.hpp>

#include "StateHandler.h"
#include "MenuState.h"
#include "AIMap.h"

int main() {
	CAIMap *newMap = new CAIMap();

	int screenHeight = 900;
	int screenWidth = 650;

	//Fetch Map Size

	//Update Screen Size to Border Map if possible

	//Render Window using screen dimensions
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "RTT Path Planning Algorithm. AI - Chris Walters");

	//Initialise All States
	STATEHANDLER.initState("menuState", new CMenuState);

	//Set Starting State
	STATEHANDLER.changeState("menuState");

	newMap->loadAllFiles();

	//GameLoop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			//Check for Close
			if (event.type == sf::Event::Closed)
				window.close();
		}
		STATEHANDLER.updateCurrState();
		STATEHANDLER.renderCurrState(window);
	}

	return 0;
}