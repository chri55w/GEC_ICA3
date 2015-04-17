#include "GameState.h"
#include "MapHandler.h"
#include "StateHandler.h"


void CGameState::onCreate() {

}
void CGameState::onDestroy() {

}
void CGameState::onEnter() {
	//Reset Node Position Vector and the current node  count
	nodePositions.clear();
	nodeCount = 0;
	path.clear();

	//Setup Node Highlights including:		(Circles for all nodes),	
	//		(Circle for current random Location),	(Circle for current closest node), 
	//		(Square shapes for start and end positions).
	nodeHighlight = sf::CircleShape(1.5);
	nodeHighlight.setFillColor(sf::Color::Cyan);
	nodeHighlight.setOrigin(1.5, 1.5);

	randHighlight = sf::CircleShape(3.0);
	randHighlight.setFillColor(sf::Color(245, 88, 88));
	randHighlight.setOrigin(1.5, 1.5);

	closestHighlight = sf::CircleShape(3.0);
	closestHighlight.setFillColor(sf::Color(222, 29, 29));
	closestHighlight.setOrigin(1.5, 1.5);

	playerShape = sf::CircleShape(7.0);
	playerShape.setFillColor(sf::Color(222, 29, 29));
	playerShape.setOrigin(3.5, 3.5);

	startPosHighlight = sf::RectangleShape(sf::Vector2f(4, 4));
	startPosHighlight.setFillColor(sf::Color(100, 100, 100));
	startPosHighlight.setOrigin(2, 2);

	endPosHighlight = sf::RectangleShape(sf::Vector2f(4, 4));
	endPosHighlight.setFillColor(sf::Color(100, 100, 100));
	endPosHighlight.setOrigin(2, 2);

	//Load font file into SFML for all text objects.
	font.loadFromFile("..\\Fonts\\ariel.ttf");

	//Setup text strings vector with all possible strings for tips
	//		Setup location and colour of node counter text object.
	textStringTips.push_back("Tip: Press 'Esc' to return to the menu");
	textStringTips.push_back("Tip: Click map whitespace to begin RRT");
	textStringTips.push_back("                    Info: RRT Evaluating \nTip: Click near a node to select an end point.");
	textStringTips.push_back("           Info: Path Evaluated \nTip: Press 'Esc' to return to the menu");

	textTip = new sf::Text("", font, 25);
	textTip->setColor(sf::Color::Black);

	updateTextTips();

	nodeCountText = new sf::Text("", font, 15);
	nodeCountText->setPosition(5, 5);
	nodeCountText->setColor(sf::Color::Black);

	//Seed Rand Function
	srand((unsigned int)time(NULL));

	//Initialize the map offsets for drawing.
	mapOffsetX = (s_width - MAP.getMapWidth()) / 2;
	mapOffsetY = (s_height - MAP.getMapHeight()) / 2;

	//Draw map data to SFML vertex array object for faster drawing at runtime.
	setUpMapVertexArray();

	//Setup the SFML Vertex array that stores all the drawn lines between nodes.
	int drawablePixels = (MAP.getMapWidth() * MAP.getMapHeight());
	nodesImage = sf::VertexArray(sf::Points, drawablePixels * 4);
	pathImage = sf::VertexArray(sf::Points, drawablePixels * 4);

	//Reset all game states
	startNodeSelected = false;
	endNodeSelected = false;
	exploring = false;
	destinationReached = false;

}
void CGameState::onExit() {

}
void CGameState::onRender(sf::RenderWindow& window) {
	window.clear(sf::Color::White);
	//Draw Map Data Image (walls etc)
	window.draw(mapImage);

	//Draw Path Image (Vectors between nodes)
	window.draw(nodesImage);

	//Draw the current text tip and the current count of nodes
	window.draw(*textTip);
	window.draw(*nodeCountText);

	//Draw circles to represent every node created
	for (unsigned int i = 0; i < nodePositions.size(); i++) {
		window.draw(nodePositions[i]);
	}

	//Draw circles to represent this updates random position and closest node.
	window.draw(randHighlight);
	window.draw(closestHighlight);

	//Draw Rectangles to represent the start and end positions.
	window.draw(startPosHighlight);
	window.draw(endPosHighlight);
	
	window.draw(pathImage);

	window.draw(playerShape);

	window.display();

}
void CGameState::onUpdate(sf::RenderWindow& window) {
	//Current State Processing

	//If the start node has not yet been selected display tip to
	//		hint user to select a root node location.
	if (!startNodeSelected) {
		selectedText = 1;

	//If the start node has been selected and the system is exploring
	//		display tip to hint user to select an end position to 
	//		identify a path to and keep trying random points.
	} else if (exploring) {
		selectedText = 2;
		tryRandomPoint(CVector2D((float)(rand() % MAP.getMapWidth()), (float)(rand() % MAP.getMapHeight())));
	//If the system is no longer exploring an end node has been 
	//		selected so process the quickest path to the destination.
	}
	else if (path.size() != 0) {
		//Move a character along the path
		/*CVector2D direction = CVector2D(0, 0);
		direction.asUnitVector(path[playerLastNodePassed], path[playerLastNodePassed + 1]);
		int playerX = playerShape.getPosition().x;
		int playerY = playerShape.getPosition().y;

		playerX 
		playerShape.setPosition(playerShape.getPosition() + direction);*/

	} else {
		selectedText = 0;
	}

	//Update to the relevant tip.
	updateTextTips();

	//Mouse and Keyboard Input

	//Check if mouse click has been released to prevent multiple processing of single long clicks
	if (mouseClicked && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		mouseClicked = false;
	}
	if (!mouseClicked) {

		//Check if left mouse button currently pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

			//Identify mouse click position relative to the current map.
			int mouseClickPosX = sf::Mouse::getPosition(window).x - mapOffsetX;
			int mouseClickPosY = sf::Mouse::getPosition(window).y - mapOffsetY;

			//Check the mouse click position is within the boundaries of the map.
			if (mouseClickPosX >= 0 && mouseClickPosY >= 0 && 
				mouseClickPosX < MAP.getMapWidth() && mouseClickPosY < MAP.getMapHeight()) {

				//Check if the start node has not been selected yet.
				if (!startNodeSelected) {

					//Identify if the selected position is drawable
					if (canDraw(CVector2D((float)mouseClickPosX, (float)mouseClickPosY))) {

						//Change the prosessing state, increase the node count and create the root node.
						startNodeSelected = true;
						exploring = true;
						nodeCount++;
						rootNode_ = new CRRTNode(new CVector2D((float)mouseClickPosX, (float)mouseClickPosY));
						startPosHighlight.setPosition((float)mouseClickPosX + mapOffsetX, (float)mouseClickPosY + mapOffsetY);
					}

				//If the start node has been selected check if the end node has not been selected yet.
				} else if (!endNodeSelected) {
					CRRTNode *endNode = tryEndNode(CVector2D(mouseClickPosX, mouseClickPosY));
					if (endNode != nullptr) {
						CVector2D endNodePos = endNode->getPosition();

						endPosHighlight.setPosition((float)mouseClickPosX + mapOffsetX, (float)mouseClickPosY + mapOffsetY);
						closestHighlight.setPosition(-10, -10);
						randHighlight.setPosition(-10, -10);

						CRRTNode *currentNode = endNode;

						path.push_back(currentNode->getPosition());

						while (currentNode->getParent() != nullptr) {
							currentNode = currentNode->getParent();
							path.push_back(currentNode->getPosition());
						}

						if (path.size() > 0) {
							for (int i = 0; i < path.size() - 1; i++) {
								drawLine(path[i], path[i + 1]);
							}
						}
						exploring = false;
						endNodeSelected = true;
					}
				}
			}

			//Set the mouse clicked bool to true as part of the prevention of multiple prossessing of single clicks.
			mouseClicked = true;
		}
	}

	//Check for the 'ESCAPE' key press to return to the menu state.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		STATEHANDLER.changeState("menuState");
	}
}

void CGameState::setUpMapVertexArray() {

	//Fetch the mapDrawData for the current map
	//		This was calculated at startup
	std::vector<CMapHandler::mapPixel*> mapDrawData = MAP.getMapDrawData();

	//Initialise the SFML Vertex array 
	mapImage = sf::VertexArray(sf::Points, mapDrawData.size() * 4);

	//Loop through the vector setting each pixel in the
	//		map vertex array to match the data from the map.
	for (unsigned int i = 0; i < mapDrawData.size(); i++) {
		mapImage[i].position = sf::Vector2f(mapDrawData[i]->x_pos + mapOffsetX, 
											mapDrawData[i]->y_pos + mapOffsetY);
		mapImage[i].color = mapDrawData[i]->pixel_colour;
	}
}

void CGameState::updateTextTips() {
	//Check if the selected text string matches the currently
	//		displayed text, if not update the string and position.
	if (textStringTips[selectedText] != textTip->getString()) {
		textTip->setString(textStringTips[selectedText]);
		textTip->setPosition((s_width - textTip->getLocalBounds().width) / 2, 10);
	}
}

void CGameState::tryRandomPoint(CVector2D randPos) {

	//Find the closest node
	CRRTNode *closestNode = rootNode_->getClosestNode(randPos);

	//Store the closest nodes position.
	CVector2D closestNodePos = closestNode->getPosition();

	//Calculate the direction between the randomly 
	//		generated position and the closest node then normalize.
	CVector2D direction = CVector2D(0, 0);
	direction.asUnitVector(closestNodePos, randPos);

	//Create an end position vector at the position 
	//		of the randomly generated point.
	CVector2D endPos = CVector2D(randPos);

	//Calculate the distance between the random position and the 
	//		closest node, if the distance is more then the defined 
	//		max distance then endPos needs to move closer.
	float distance = closestNodePos.distance(randPos);
	if (distance > maxDist) {

		//Move the endPosition using the previously calculated direction vector.
		endPos.xPos_ = closestNodePos.xPos_ + direction.xPos_ * maxDist;
		endPos.yPos_ = closestNodePos.yPos_ + direction.yPos_ * maxDist;

		//Recalculate the closest node and closest node position
		//		to see if this has changed
		closestNode = rootNode_->getClosestNode(endPos);
		closestNodePos = closestNode->getPosition();
	}

	//Recalculate the distance to the end position from the closest node, 
	//		if the distance is more than the defined minimum distance then 
	//		try to draw a line between the two positions.
	distance = closestNodePos.distance(endPos);
	if (distance > minDist) {
		tryDrawLine(*closestNode, endPos, direction);
	}

	//Set the current Positions of the random and closest position highlights.
	randHighlight.setPosition(randPos.xPos_ + mapOffsetX, randPos.yPos_ + mapOffsetY);
	closestHighlight.setPosition(closestNodePos.xPos_ + mapOffsetX, closestNodePos.yPos_ + mapOffsetY);
}

void CGameState::tryDrawLine(CRRTNode &posA, CVector2D posB, CVector2D direction) {
	//Set the current draw position to the position of the closestNode
	CVector2D drawPos = CVector2D(posA.getPosition());

	//Declare a vector of positions, this stores each position that we will draw 
	//		the line on should we reach the random position or at the least a 
	//		point far enough away from the other nodes.
	std::vector<CVector2D> positionsToDraw;

	//Provide the while loop with a way to exit.
	bool drawingLine = true;
	while (drawingLine) {

		//If the current draw position is not colliding with a non-traversable 
		//		object, push the position to the vector of positions to draw 
		//		then add the normalised direction vector to the position.
		if (canDraw(drawPos)) {
			positionsToDraw.push_back(CVector2D(drawPos.xPos_, drawPos.yPos_));
			drawPos += direction;

		//If the current draw position IS colliding then check to see if we can 
		//		add a node here instead, by fetching the closest node to the current 
		//		position, calculating the distance and comparing it against the defined 
		//		minimum distance then creating a node and calling the drawLine 
		//		function which proccesses the position data pushed to the vector 
		//		of draw data.
		//		Then exit the drawing loop regardless of if a new node has been 
		//		created or not.
		} else {
			CRRTNode *closestNodeToCollision = rootNode_->getClosestNode(drawPos);
			float dist = drawPos.distance(closestNodeToCollision->getPosition());
			if (dist > minDist) {
				addNode(posA, new CVector2D(drawPos.xPos_ - direction.xPos_, drawPos.yPos_ - direction.yPos_));
				drawLine(positionsToDraw);
			}
			drawingLine = false;
		}
		//After adding a new position to the draw data check to see if the 
		//		current position is within a distance of 1 from the destination. 
		//		If so then add a new node and call the drawLine function to 
		//		draw the created vector. Then finally exit the drawing loop.
		if (abs(drawPos.xPos_ - posB.xPos_) < 1) {
			if (abs(drawPos.yPos_ - posB.yPos_) < 1) {
				addNode(posA, new CVector2D(drawPos.xPos_ - direction.xPos_, drawPos.yPos_ - direction.yPos_));
				drawLine(positionsToDraw);
				drawingLine = false;
			}
		}
	}
}

bool CGameState::canDraw(CVector2D position) {

	//Initialise a vector of ints containing offsets to relevent map data
	std::vector<int> offsetsToCheck;
	offsetsToCheck.resize((nodeCollisionSqrRadius*2) * (nodeCollisionSqrRadius*2));
	
	//Identify the offset of the pixel position clicked
	int centreOffset = (int)position.xPos_ + ((int)position.yPos_ * MAP.getMapWidth());

	//Calculate the top left corner of the rectangle to check
	int startOffset = (centreOffset - nodeCollisionSqrRadius) - (nodeCollisionSqrRadius * MAP.getMapWidth());

	//Calculate the bottom right corner offset of the rectangle to check
	int endOffset = startOffset + (MAP.getMapWidth() * (nodeCollisionSqrRadius * 2)) + (nodeCollisionSqrRadius * 2);

	//Calculate the line incrementation to move from the end of a rectangle 
	//		line to the start of the rectangle on the next line.
	int lineIncrement = MAP.getMapWidth() - (nodeCollisionSqrRadius * 2);

	//Loop through the offsets array and populate it with the desired offsets.
	int currOffset = startOffset;
	for (int i = 0; i < offsetsToCheck.size(); i++) {
		offsetsToCheck[i] = currOffset;
		if (i % (nodeCollisionSqrRadius * 2) == 0 && i != 0) {
			currOffset += lineIncrement;
		} else {
			currOffset++;
		}
	}

	//Check the offsets wont go out of bounds in the map data vector.
	//		If it will then continue to the circle calculation
	if (position.xPos_ < nodeCollisionSqrRadius || position.xPos_ > MAP.getMapWidth() - nodeCollisionSqrRadius) {

		//Part of Rectangle check will be out of bounds either on the left or right of map
		//		skip to circle bounds check
	} else if (startOffset < 0 || endOffset > MAP.getMapHeight() * MAP.getMapWidth()) {

		//Part of Rectangle check will be out of bounds on either the top or bottom of map
		//		skip to circle bounds check
	} else {

		//loop through the offsets and check each one to each collidable 
		//		map object, if an obstacle is found continue to the circle calculation
		bool objectFound = false;
		int index = 0;
		while (!objectFound && index < offsetsToCheck.size()) {
			char data = MAP.getMapData(offsetsToCheck[index]);
			if (data == '@' || data == 'T') {
				objectFound = true;
			}
			index++;
		}
		if (!objectFound) {
			return true;
		}
	}
	return false;
}

void CGameState::drawLine(std::vector<CVector2D> positionsToDraw) {

	//Loop through the vector of positions to draw, calculate an offset 
	//		into the SFML Vertex Array and alter that pixel to the a light  
	//		grey colour then give that pixel a position to draw to.
	for (unsigned int i = 0; i < positionsToDraw.size(); i++) {
		int offset = (int)positionsToDraw[i].xPos_ + ((int)positionsToDraw[i].yPos_ * MAP.getMapWidth());

		nodesImage[offset].color = sf::Color(180,180,180);
		nodesImage[offset].position = sf::Vector2f(positionsToDraw[i].xPos_ + mapOffsetX, positionsToDraw[i].yPos_ + mapOffsetY);
	}
}

void CGameState::addNode(CRRTNode &parentNode, CVector2D *position) {

	//Create a child node from the parent node provided using a Vec2 position.
	parentNode.addChildNode(position);

	//Move the node highlight circle to the position of the new node and 
	//		push it to the back of the node positions vector for rendering.
	nodeHighlight.setPosition(position->xPos_ + mapOffsetX, position->yPos_ + mapOffsetY);
	nodePositions.push_back(nodeHighlight);

	//Increase the node count and update the text to display the node count.
	nodeCount++;
	nodeCountText->setString("Node Count: " + std::to_string(nodeCount));
}

CRRTNode *CGameState::tryEndNode(CVector2D posToReach) {
	CRRTNode *closestNode = rootNode_->getClosestNode(posToReach);
	return closestNode;
	CVector2D posOfClosestNode = closestNode->getPosition();

	if (posOfClosestNode.distance(posToReach) == 0) {
		closestNode->addChildNode(&posToReach);
		return closestNode->getLastChildNode();
	}

	if (posOfClosestNode.distance(posToReach) < maxDist) {

		//Declare a vector of positions, this stores each position that we will draw 
		//		the line on should we reach the random position or at the least a 
		//		point far enough away from the other nodes.
		std::vector<CVector2D> positionsToDraw;

		CVector2D drawPosition = posOfClosestNode;

		CVector2D direction = CVector2D(0, 0);
		direction.asUnitVector(posOfClosestNode, posToReach);

		bool destReached = false;
		while (!destReached) {
			if (canDraw(drawPosition)) {
				positionsToDraw.push_back(drawPosition);
				drawPosition += direction;
			} else {
				break;
			}
			//After adding a new position to the draw data check to see if the 
			//		current position is within a distance of 1 from the destination. 
			//		If so then add a new node and call the drawLine function to 
			//		draw the created vector. Then finally exit the drawing loop.
			if (abs(drawPosition.xPos_ - posToReach.xPos_) < 1) {
				if (abs(drawPosition.yPos_ - posToReach.yPos_) < 1) {
					closestNode->addChildNode(&posToReach);
					drawLine(positionsToDraw);
					destReached = true;
				}
			}
		}
		if (destReached) {
			return closestNode->getLastChildNode();
		}
	}
	return nullptr;
}

void CGameState::drawLine(CVector2D posA, CVector2D posB) {
	CVector2D drawPos = posA;

	CVector2D direction = CVector2D(0, 0);
	direction.asUnitVector(posA, posB);

	bool drawingLine = true;
	while (drawingLine) {
		int offset = (int)drawPos.xPos_ + ((int)drawPos.yPos_ * MAP.getMapWidth());
		pathImage[offset].position = sf::Vector2f(drawPos.xPos_ + mapOffsetX, drawPos.yPos_ + mapOffsetY);
		pathImage[offset].color = sf::Color::Magenta;
		drawPos += direction;


		if (abs(drawPos.xPos_ - posB.xPos_) < 1) {
			if (abs(drawPos.yPos_ - posB.yPos_) < 1) {
				drawingLine = false;
			}
		}
	}

}