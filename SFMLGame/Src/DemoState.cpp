#include "DemoState.h"
#include "MapHandler.h"
#include "StateHandler.h"


void CDemoState::onRender(sf::RenderWindow& window) {
	window.clear(sf::Color::White);

	//Draw Map Data Image (walls etc)
	window.draw(mapImage);

	window.display();
}

void CDemoState::onUpdate(sf::RenderWindow& window) {

	//TO DO: IMPLEMENT DEMO ENVIRONMENT
	//		CREATE A TREE, PICK RANDOM POSITION AND DIRECT NPC ALONG THAT PATH
	//		ONCE DESTINATION IS REACHED PICK ANOTHER POSITION AND DIRECT NPC TO 
	//		THE NEW POSITION.
	//		USE A MOUSE CLICK EVENT TO TRIGGER 'DISTRACTIONS' IF THE NPC IS IN RANGE
	//		REASSIGN THE DESTINATION TO THE CLOSEST NODE TO THE DISTRACTION.



	//Check if mouse click has been released to prevent multiple processing of single long clicks
	if (mouseClicked && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		mouseClicked = false;
	}
	if (!mouseClicked) {
		//Check if left mouse button currently pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			//Set the mouse clicked bool to true as part of the prevention of multiple prossessing of single clicks.
			mouseClicked = true;
		}
	}
	//Check for the 'ESCAPE' key press to return to the menu state.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		STATEHANDLER.changeState("menuState");
	}
}