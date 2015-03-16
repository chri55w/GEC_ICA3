#include "GameState.h"
#include "MapHandler.h"


CGameState::CGameState() {
}

CGameState::~CGameState() {
}

void CGameState::onCreate() {

}
void CGameState::onDestroy() {

}
void CGameState::onEnter() {

}
void CGameState::onExit() {

}
void CGameState::onRender(sf::RenderWindow& window, int s_height, int s_width) {
	window.clear(sf::Color::White);


	std::vector<CMapHandler::mapPixel*> mapDrawData = MAP.getMapDrawData();

	sf::VertexArray mapImage(sf::Points, mapDrawData.size() * 4);

	int mapOffsetX = (s_width - MAP.getMapWidth()) / 2;
	int mapOffsetY = (s_height - MAP.getMapHeight()) / 2;

	for (int i = 0; i < mapDrawData.size(); i++) {
		CMapHandler::mapPixel *pixel = mapDrawData[i];
		mapImage[i].position = sf::Vector2f(pixel->x_pos + mapOffsetX, pixel->y_pos + mapOffsetY);
		mapImage[i].color = pixel->pixel_colour;
	}

	window.draw(mapImage);
	window.display();

}
void CGameState::onUpdate() {

}