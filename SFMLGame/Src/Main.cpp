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
#include "GameState.h"
#include "MapHandler.h"

int main() {

	int screenHeight = 900;
	int screenWidth = 650;

	int frameRate = 0;
	sf::Clock clock;

	//Render Window using screen dimensions
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "RRT Path Planning Algorithm. AI - Chris Walters");

	//Initialise All States
	STATEHANDLER.initState("menuState", new CMenuState);
	STATEHANDLER.initState("gameState", new CGameState);

	//Set Starting State
	STATEHANDLER.changeState("menuState");

	//GameLoop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			//Check for Close
			if (event.type == sf::Event::Closed)
				window.close();
		}
		STATEHANDLER.updateCurrState();
		STATEHANDLER.renderCurrState(window, screenHeight, screenWidth);
		if (clock.getElapsedTime().asMilliseconds() > 1000) {
			std::cout << "FPS: " << frameRate << std::endl;
			clock.restart();
			frameRate = 0;
		} else {
			frameRate++;
		}
	}
	return 0;
}