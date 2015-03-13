#include "MenuState.h"
#include "MapHandler.h"
#include "StateHandler.h"

CMenuState::CMenuState() {
	selectConditions.push_back(true);
	selectConditions.push_back(false);
}

CMenuState::~CMenuState() {
}

void CMenuState::onCreate() {
	font.loadFromFile("..\\Fonts\\ariel.ttf");
	newText("Menu", font, 25, 15.0f, 125.0f);

	newText("Select Map to Load", font, 15, 50.0f, 350.0f, true);
	newText("Exit System", font, 15, 50.0f, 375.0f, true);

	std::vector<std::string> mapNames = MAP.loadAllMapNames("..\\Maps", "map");
	float yPos = 350.0f - (mapNames.size() * 15) / 2;
	for (std::string name : mapNames) {
		newText(name, font, 12, 200, yPos, true, 1);
		yPos += 15;
	}
	resetSelectedText();

}
void CMenuState::onDestroy() {

}
void CMenuState::onEnter() {

}
void CMenuState::onExit() {

}
void CMenuState::onRender(sf::RenderWindow& window) {
	window.clear();
	//render here
	for (textObj text : menuTexts) {
		if (selectConditions[text.selectCondition]) {
			window.draw(text.text);
		}
	}
	window.display();

}
void CMenuState::onUpdate() {
	menuTexts[selectedText].text.setColor(sf::Color::White);
	//Check for text button clicks
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		keyPressed = false;
	}
	if (!keyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			//use selected button as choice
			selectMenuItem();
			keyPressed = true;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			tryMoveMenu(UP);
			keyPressed = true;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			tryMoveMenu(DOWN);
			keyPressed = true;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			tryMoveMenu(LEFT);
			keyPressed = true;
		}
	}
	menuTexts[selectedText].text.setColor(sf::Color::Red);	
}

void CMenuState::newText(std::string text, sf::Font, int size, float xPos, float yPos, bool canSelect, int selectCondition) {
	textObj newTextObject;
	newTextObject.text = sf::Text(text, font, size);
	newTextObject.text.setPosition(xPos, yPos);

	newTextObject.selectable = canSelect;
	newTextObject.selectCondition = selectCondition;

	menuTexts.push_back(newTextObject);
}
void CMenuState::resetSelectedText() {
	for (unsigned int i = 0; i < menuTexts.size(); i++) {
		if (menuTexts[i].selectable) {
			selectedText = i;
			return;
		}
	}
}
void CMenuState::tryMoveMenu(direction moveDir) {
	unsigned int i = selectedText;
	switch (moveDir) {
		case direction::UP:
			while (i > 0) {
				i--;
				if (menuTexts[i].selectable) {
					if (selectConditions[menuTexts[i].selectCondition]) {
						selectedText = i;
						return;
					}
				}
			}
			break;
		case direction::DOWN:
			while (i < menuTexts.size() - 1) {
				i++;
				if (menuTexts[i].selectable) {
					if (selectConditions[menuTexts[i].selectCondition]) {
						selectedText = i;
						return;
					}
				}
			}
			break;
		case direction::LEFT:
			if (selectConditions[1]) {
				selectConditions[1] = false;
				menuTexts[1].selectable = true;
				menuTexts[2].selectable = true;
				resetSelectedText();
			}
			break;
	}

}

void CMenuState::selectMenuItem() {
	textObj selectedItem = menuTexts[selectedText];

	if (selectedItem.text.getString() == "Exit System") {
		//Close
	} else if (selectedItem.text.getString() == "Select Map to Load") {
		if (!selectConditions[1]) {
			//Show Map Texts
			selectConditions[1] = true;
			menuTexts[1].selectable = false;
			menuTexts[2].selectable = false;
			resetSelectedText();
		}
	} else if (stringContains(selectedItem.text.getString(), ".map")) {
		std::cout << "Calling Map Parser for Map: " << std::string(selectedItem.text.getString()) << std::endl;
		MAP.parseMap(selectedItem.text.getString());
		STATEHANDLER.changeState("gameState");
		return;
	} else {
		std::cout << "Error: Unhandled Button Press with Text: " << std::string(selectedItem.text.getString()) << std::endl;
		return;
	}
	std::cout << "Button Press Handled with Text: " << std::string(selectedItem.text.getString()) << std::endl;
}

bool CMenuState::stringContains(std::string stringToCheck, std::string stringToFind) {
	if (stringToCheck.find(stringToFind) != std::string::npos) {
		return true;
	}
	return false;
}