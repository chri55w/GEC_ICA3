#include "MenuState.h"
#include "MapHandler.h"
#include "StateHandler.h"


void CMenuState::onCreate() {

	//Initialise the font to be used for each menu item.
	font.loadFromFile("..\\Fonts\\ariel.ttf");
	
	//Create the first node, used for containing all main nodes.
	rootNode_ = new CMenuNode();
	currentNode_ = rootNode_;

	//Create the main menu options
	rootNode_->addChildNode(sf::Text("Load Map", font, 25), sf::Vector2f(50.0f, 325.0f), CMenuNode::NULLCALL);
	rootNode_->addChildNode(sf::Text("Demo Game", font, 25), sf::Vector2f(50.0f, 350.0f), CMenuNode::NULLCALL);
	rootNode_->addChildNode(sf::Text("Settings", font, 25), sf::Vector2f(50.0f, 375.0f), CMenuNode::NULLCALL);
	rootNode_->addChildNode(sf::Text("Exit", font, 25), sf::Vector2f(50.0f, 400.0f), CMenuNode::EXIT);

	//Load all maps in and take the names of them, 
	//		push names into the load map node as sub nodes.
	std::vector<std::string> mapNames = MAP.loadAllMaps("..\\Maps", "map");
	float yPos = 362.5f - (mapNames.size() * 15) / 2;
	for (std::string name : mapNames) {
		rootNode_->fetchNode(0)->addChildNode(sf::Text(name, font, 15), sf::Vector2f(200.0f, yPos), CMenuNode::STARTGAME);
		yPos += 15;
	}
	yPos = 362.5f - (mapNames.size() * 15) / 2;
	for (std::string name : mapNames) {
		rootNode_->fetchNode(1)->addChildNode(sf::Text(name, font, 15), sf::Vector2f(200.0f, yPos), CMenuNode::STARTDEMO);
		yPos += 15;
	}

	//push a setting into the settings node
	rootNode_->fetchNode(2)->addChildNode(sf::Text("Force Walls : True", font, 15), sf::Vector2f(200.0f, 380.0f), CMenuNode::FORCEWALLS);

	//Update the map edges to force walls - Could use some optimisation
	MAP.updateMapEdges(forceSurroundingWalls);

}
void CMenuState::onDestroy() {

}
void CMenuState::onEnter() {

}
void CMenuState::onExit() {

}
void CMenuState::onRender(sf::RenderWindow& window) {
	window.clear();
	//Loop through and render all actively visible nodes.
	CMenuNode *renderNode = rootNode_;
	while (renderNode->countChildren() > 0) {
		for (unsigned int i = 0; i < renderNode->countChildren(); i++) {
			window.draw(*renderNode->fetchNode(i)->getText());
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
void CMenuState::onUpdate(sf::RenderWindow& window) {

	//Boolean tracking the state of a key press, used 
	//		to prevent multiple key proccessing for a single 
	//		long key press.
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		keyPressed = false;
	}	
	if (!keyPressed) {	

		//If 'RETURN' or 'RIGHT' key pressed get the current node and identify its selected sub node.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {

			//if the selected sub node is not null then check the callback function 
			if (currentNode_->getSelectedNode() != nullptr) {
				//If the key pressed is the 'RETURN' key use the menu button callback
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
					checkMenuCallback(currentNode_->getSelectedNode()->getCallBack(), currentNode_->getSelectedNode());
				}
				//If the selected node has any child nodes then enter the 
				//		selected node and assign the it to the current node
				if (currentNode_->getSelectedNode()->countChildren() > 0) {
					currentNode_ = currentNode_->getSelectedNode();
					currentNode_->enterNode();
				}
			}
			keyPressed = true;

		//If 'UP' key is pressed the current nodes selected node should 
		//		change to the node above
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			currentNode_->selectPreviousNode();
			keyPressed = true;

		//If 'DOWN' key is pressed the current nodes selected node should 
		//		change to the node below
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			currentNode_->selectNextNode();
			keyPressed = true;

		//If 'LEFT' key is pressed the current node should be assigned 
		//		to the parent node of the current node.
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			if (currentNode_->getParentNode() != nullptr) {
				currentNode_->deselectNode();
				currentNode_ = currentNode_->getParentNode();
			}
			keyPressed = true;
		}
	}	
}

void CMenuState::checkMenuCallback(CMenuNode::callbackType callBack, CMenuNode *node) {
	switch (callBack) {
		//In the case of a null button call print a message to the console
		case CMenuNode::NULLCALL:
			std::cout << "Null Button Call Ignored" << std::endl;
			break;
		//If the callback is an exit call trigger a close event.
		case CMenuNode::EXIT:
			//TODO: trigger close event...
			std::cout << "Exit Called" << std::endl;
			break;
		//If call back is force walls settings change then check the 
		//		current setting and alter maps to recognise the setting.
		case CMenuNode::FORCEWALLS:
			if (forceSurroundingWalls == true) {
				forceSurroundingWalls = false;
				MAP.updateMapEdges(forceSurroundingWalls);
				node->getText()->setString("Force Walls : False");
			} else {
				forceSurroundingWalls = true;
				MAP.updateMapEdges(forceSurroundingWalls);
				node->getText()->setString("Force Walls : True");
			}
			std::cout << "Force Walls Call" << std::endl;
			break;
		//If callback is start game then set the currentmap to the 
		//		selected string then call the change of state function 
		//		to begin the gamestate
		case CMenuNode::STARTGAME:
			std::cout << "Calling Map Parser for Map: " << std::string(node->getText()->getString()) << std::endl;
			MAP.setCurrentMap(node->getText()->getString());
			STATEHANDLER.changeState("gameState");
			std::cout << "Start Game Called" << std::endl;
			break; 
		case CMenuNode::STARTDEMO:
				std::cout << "Calling Map Parser for Map: " << std::string(node->getText()->getString()) << std::endl;
				MAP.setCurrentMap(node->getText()->getString());
				STATEHANDLER.changeState("demoState");
				std::cout << "Demo Game Environment Called" << std::endl;
				break;

		default:
			std::cout << "Unrecognised Button Call Ignored" << std::endl;
			break;
	}
}