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
	nodePositions.clear();
	nodeCount = 0;

	nodeHighlight = sf::CircleShape(1.5);
	nodeHighlight.setFillColor(sf::Color::Cyan);
	nodeHighlight.setOrigin(1.5, 1.5);

	randHighlight = sf::CircleShape(3.0);
	randHighlight.setFillColor(sf::Color(245, 88, 88));
	randHighlight.setOrigin(1.5, 1.5);

	closestHighlight = sf::CircleShape(3.0);
	closestHighlight.setFillColor(sf::Color(222,29,29));
	closestHighlight.setOrigin(1.5, 1.5);

	textStringTips.push_back("Tip: Press 'Esc' to return to menu");
	textStringTips.push_back("Tip: Click map whitespace to begin RRT");
	textStringTips.push_back("                    Info: RRT Evaluating \nTip: Click map whitespace to select end point.");

	nodeCountText.setPosition(5, 5);
	nodeCountText.setColor(sf::Color::Black);

	sf::Time currTimeMillis = sf::Time::Time();
	srand(time(NULL));

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

	int drawablePixels = (MAP.getMapWidth() * MAP.getMapHeight());

	nodesImage = sf::VertexArray(sf::Points, drawablePixels * 4);

	startNodeSelected = false;
	endNodeSelected = false;
	exploring = false;
	destinationReached = false;

}
void CGameState::onExit() {

}
void CGameState::onRender(sf::RenderWindow& window) {
	window.clear(sf::Color::White);

	window.draw(mapImage);
	window.draw(nodesImage);
	window.draw(*textTip);
	window.draw(nodeCountText);
	for (int i = 0; i < nodePositions.size(); i++) {
		window.draw(nodePositions[i]);
	}
	window.draw(randHighlight);
	window.draw(closestHighlight);
	window.display();

}
void CGameState::onUpdate(sf::RenderWindow& window) {
	int nodesAtStart = nodeCount;
	//Current State Processing
	if (!startNodeSelected) {
		selectedText = 1;
		exploring = true;
	} else if (exploring) {
		selectedText = 2;
		tryRandomPoint(CVector2D(rand() % MAP.getMapWidth(), rand() % MAP.getMapHeight()));
	} else if (destinationReached) {
		selectedText = 3;
	} else {
		selectedText = 0;
	}

	//END Current State Processing

	//Text Rendering of Tips and Info

	if (textStringTips[selectedText] != textTip->getString()) {
		textTip->setString(textStringTips[selectedText]);
		textTip->setPosition((s_width - textTip->getLocalBounds().width) / 2, 10);
	}

	//END Text Rendering of Tips and Info

	//Mouse and Keyboard Input

	if (mouseClicked && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		mouseClicked = false;
	}
	if (!mouseClicked) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			if (!startNodeSelected) {
				startNodeSelected = true;
				int mapOffsetX = (s_width - MAP.getMapWidth()) / 2;
				int mapOffsetY = (s_height - MAP.getMapHeight()) / 2;

				int mouseClickPosX = sf::Mouse::getPosition(window).x - mapOffsetX;
				int mouseClickPosY = sf::Mouse::getPosition(window).y - mapOffsetY;

				if (canDraw(CVector2D(mouseClickPosX, mouseClickPosY))) {
					nodeCount++;
					rootNode_ = new CRRTNode(mouseClickPosX, mouseClickPosY);
				}
			} else if (!endNodeSelected) {
				endNodeSelected = true;
			}
			mouseClicked = true;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		STATEHANDLER.changeState("menuState");
	}

	// END Mouse and Keyboard Input

	int nodesAtEnd = nodeCount;
	if (startNodeSelected) {
		if (nodesAtEnd - nodesAtStart == 0) {
			std::cout << "No Nodes Drawn" << std::endl;
		} else if (nodesAtEnd - nodesAtStart > 1) {
			std::cout << "Multiple Nodes Drawn: " + std::to_string(nodesAtEnd - nodesAtStart) << std::endl;
		}
	}
}

void CGameState::tryRandomPoint(CVector2D randPos) {

	CRRTNode *closestNode = rootNode_->getClosestNode(randPos);

	if (closestNode != rootNode_) {
		int i = 0;
	}
	int mapOffsetX = (s_width - MAP.getMapWidth()) / 2;
	int mapOffsetY = (s_height - MAP.getMapHeight()) / 2;


	CVector2D direction = CVector2D(0, 0);
	direction.asUnitVector(closestNode->getPosition(), randPos);

	CVector2D endPos = CVector2D(randPos);

	int distance = closestNode->getPosition().distance(randPos);
	if (distance > maxDist) {
		//If random point is too far away move it closer using direction
		endPos.xPos_ = closestNode->getPosition().xPos_ + direction.xPos_ * maxDist;
		endPos.yPos_ = closestNode->getPosition().yPos_ + direction.yPos_ * maxDist;
		closestNode = rootNode_->getClosestNode(endPos);
		CRRTNode *newClosestNode = rootNode_->getClosestNode(endPos);
		int i = 0;
	}
	distance = closestNode->getPosition().distance(endPos);
	if (distance > minDist) {
		tryDrawLine(*closestNode, endPos, direction);
	}
	randHighlight.setPosition(randPos.xPos_ + mapOffsetX, randPos.yPos_ + mapOffsetY);
	closestHighlight.setPosition(closestNode->getPosition().xPos_ + mapOffsetX, closestNode->getPosition().yPos_ + mapOffsetY);
}

void CGameState::tryDrawLine(CRRTNode &posA, CVector2D posB, CVector2D direction) {
	CVector2D drawPos = CVector2D(posA.getPosition());

	int mapOffsetX = (s_width - MAP.getMapWidth()) / 2;
	int mapOffsetY = (s_height - MAP.getMapHeight()) / 2;
	bool drawingLine = true;
	while (drawingLine) {
		if (canDraw(drawPos)) {
			int offset = (int)drawPos.xPos_ + ((int)drawPos.yPos_ * MAP.getMapWidth());
			nodesImage[offset].color = sf::Color::Magenta;
			nodesImage[offset].position = sf::Vector2f((int)drawPos.xPos_+ mapOffsetX, (int)drawPos.yPos_ + mapOffsetY);
			drawPos += direction;
		} else {
			CRRTNode *closestNodeToCollision = rootNode_->getClosestNode(drawPos);
			int dist = drawPos.distance(closestNodeToCollision->getPosition());
			if (dist > minDist) {
				addNode(posA, CVector2D(drawPos.xPos_ - direction.xPos_, drawPos.yPos_ - direction.yPos_));
			}
			drawingLine = false;
		}
		if (abs(drawPos.xPos_ - posB.xPos_) < 1) {
			if (abs(drawPos.yPos_ - posB.yPos_) < 1) {
				addNode(posA, CVector2D(drawPos.xPos_ - direction.xPos_, drawPos.yPos_ - direction.yPos_));
				drawingLine = false;
			}
		}
	}
}

bool CGameState::canDraw(CVector2D position) {
	int offset = (int)position.xPos_ + ((int)position.yPos_ * MAP.getMapWidth());
	
	char mapData = MAP.getMapData(offset);

	if (mapData == '@' || mapData == 'T') {
		return false;
	} else {
		return true;
	}
}
void CGameState::addNode(CRRTNode &parentNode, CVector2D position) {
	int mapOffsetX = (s_width - MAP.getMapWidth()) / 2;
	int mapOffsetY = (s_height - MAP.getMapHeight()) / 2;
	if (position.xPos_ > MAP.getMapWidth() || position.yPos_ > MAP.getMapHeight()) {
		int i = 0;
	}
	CVector2D nodePos = position;
	parentNode.addChildNode(nodePos);
	nodeHighlight.setPosition(nodePos.xPos_ + mapOffsetX, nodePos.yPos_ + mapOffsetY);
	nodePositions.push_back(nodeHighlight);
	nodeCount++;
	nodeCountText.setString("Node Count: " + std::to_string(nodeCount));

}