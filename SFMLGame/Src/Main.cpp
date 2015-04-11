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
#include "DemoState.h"
#include "MapHandler.h"

int main() {

	sf::Clock tickTimer;

	int screenHeight = 900;
	int screenWidth = 650;

	//Render Window using screen dimensions
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "RRT Path Planning Algorithm. AI - Chris Walters");

	//Initialise All States
	STATEHANDLER.initState("menuState", new CMenuState, screenHeight, screenWidth);
	STATEHANDLER.initState("gameState", new CGameState, screenHeight, screenWidth);
	STATEHANDLER.initState("demoState", new CDemoState, screenHeight, screenWidth);

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

		//Should you need to slow things down increase the '> 0' below to the
		//		number of miliseconds you require between each game tick. 

		//HOWEVER this will have cause issues with the responsivity to key 
		//		presses and mouse clicks
		if (tickTimer.getElapsedTime().asMilliseconds() > 0) {
			STATEHANDLER.updateCurrState(window);
			tickTimer.restart();
		}
		STATEHANDLER.renderCurrState(window);
	}
	return 0;
}