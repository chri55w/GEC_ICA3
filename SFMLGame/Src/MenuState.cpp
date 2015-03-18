#include "MenuState.h"
#include "MapHandler.h"
#include "StateHandler.h"

CMenuState::CMenuState() {
}

CMenuState::~CMenuState() {

}

void CMenuState::onCreate() {

	font.loadFromFile("..\\Fonts\\ariel.ttf");

	rootNode_ = new CMenuNode();
	currentNode_ = rootNode_;

	rootNode_->addChildNode(sf::Text("Load Map", font, 25), sf::Vector2f(50.0f, 50.0f), CMenuNode::NULLCALL);
	rootNode_->addChildNode(sf::Text("Settings", font, 25), sf::Vector2f(50.0f, 75.0f), CMenuNode::NULLCALL);
	rootNode_->addChildNode(sf::Text("Exit", font, 25), sf::Vector2f(50.0f, 75.0f), CMenuNode::NULLCALL);



	rootNode_->fetchNode(1)->addChildNode(sf::Text("Node 2-1", font, 15), sf::Vector2f(200.0f, 75.0f), CMenuNode::STARTGAME);

}
void CMenuState::onDestroy() {

}
void CMenuState::onEnter() {

}
void CMenuState::onExit() {

}
void CMenuState::onRender(sf::RenderWindow& window, int s_height, int s_width) {
	window.clear();
	CMenuNode *renderNode = rootNode_;
	while (renderNode->countChildren() > 0) {
		for (int i = 0; i < renderNode->countChildren(); i++) {
			window.draw(renderNode->fetchNode(i)->getText());
		}
		if (renderNode->getSelectedNode() != nullptr) {
			if (renderNode->getSelectedNode()->countChildren() > 0) {
				renderNode = renderNode->getSelectedNode();
			} else {
				break;
			}
		} else {
			break;
		}
	}

	window.display();

}
void CMenuState::onUpdate() {
	//Check for text button clicks
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		keyPressed = false;
	}	
	if (!keyPressed) {	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			if (currentNode_->getSelectedNode() != nullptr) {
				checkMenuCallback(currentNode_->getSelectedNode()->getCallBack());
				if (currentNode_->getSelectedNode()->countChildren() > 0) {
					currentNode_ = currentNode_->getSelectedNode();
					currentNode_->enterNode();
				}
			}
			keyPressed = true;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			currentNode_->selectPreviousNode();
			keyPressed = true;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			currentNode_->selectNextNode();
			keyPressed = true;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			if (currentNode_->getParentNode() != nullptr) {
				currentNode_->deselectNode();
				currentNode_ = currentNode_->getParentNode();
			}
			keyPressed = true;
		}
	}	
}

void CMenuState::checkMenuCallback(CMenuNode::callbackType callBack) {
	switch (callBack) {
		case CMenuNode::NULLCALL:
			std::cout << "Null Button Call Ignored" << std::endl;
			break;
		case CMenuNode::EXIT:
			std::cout << "Exit Called" << std::endl;
			break;
		case CMenuNode::FORCEWALLS:
			std::cout << "Force Walls Call" << std::endl;
			break;
		case CMenuNode::STARTGAME:
			std::cout << "Start Game Called" << std::endl;
			break;
	}
}