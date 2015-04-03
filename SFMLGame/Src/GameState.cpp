#include "GameState.h"
#include "MapHandler.h"
#include "StateHandler.h"


CGameState::CGameState() {
}

CGameState::~CGameState() {
}

void CGameState::onCreate() {

}
void CGameState::onDestroy() {

}
void CGameState::onEnter() {

	sf::Time currTimeMillis = sf::Time::Time();
	srand(currTimeMillis.asMilliseconds());

	font.loadFromFile("..\\Fonts\\ariel.ttf");

	textTip = new sf::Text("Tip: Click Esc to return to menu", font, 25);
	textTip->setPosition((s_width - textTip->getLocalBounds().width) / 2, 10);
	textTip->setColor(sf::Color::Black);

	std::vector<CMapHandler::mapPixel*> mapDrawData = MAP.getMapDrawData();

	mapImage = sf::VertexArray(sf::Points, mapDrawData.size() * 4);

	int mapOffsetX = (s_width - MAP.getMapWidth()) / 2;
	int mapOffsetY = (s_height - MAP.getMapHeight()) / 2;

	for (int i = 0; i < mapDrawData.size(); i++) {
		CMapHandler::mapPixel *pixel = mapDrawData[i];
		mapImage[i].position = sf::Vector2f(pixel->x_pos + mapOffsetX, pixel->y_pos + mapOffsetY);
		mapImage[i].color = pixel->pixel_colour;
	}
}
void CGameState::onExit() {

}
void CGameState::onRender(sf::RenderWindow& window) {
	window.clear(sf::Color::White);

	window.draw(mapImage);
	window.draw(*textTip);
	window.display();

}
void CGameState::onUpdate() {
	if (exploring) {

		tryRandomPoint(rand() % MAP.getMapWidth(), rand() % MAP.getMapHeight());
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		STATEHANDLER.changeState("menuState");
	}
}

void CGameState::tryRandomPoint(int randX, int randY) {

	int i = MAP.getMapHeight() * MAP.getMapWidth();
}