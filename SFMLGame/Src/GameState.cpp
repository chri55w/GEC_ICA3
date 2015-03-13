#include "GameState.h"
#include "MapHandler.h"


CGameState::CGameState() {
}

CGameState::~CGameState() {
}

void CGameState::onCreate() {
	square = sf::RectangleShape(sf::Vector2f(1.0f, 1.0f));
}
void CGameState::onDestroy() {

}
void CGameState::onEnter() {

}
void CGameState::onExit() {

}
void CGameState::onRender(sf::RenderWindow& window, int s_height, int s_width) {
	window.clear(sf::Color::White);
	//render here

	std::vector<CMapHandler::mapPixel*> mapDrawData = MAP.getMapDrawData();

	int mapOffsetX = (s_width - MAP.getMapWidth()) / 2;
	int mapOffsetY = (s_height - MAP.getMapHeight()) / 2;

	for (CMapHandler::mapPixel* pixel : mapDrawData) {
		square.setPosition(sf::Vector2f(pixel->x_pos + mapOffsetX, pixel->y_pos + mapOffsetY));
		square.setFillColor(pixel->pixel_colour);
		window.draw(square);
	}

	window.display();

}
void CGameState::onUpdate() {

}