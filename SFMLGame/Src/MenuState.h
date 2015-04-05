#pragma once
#include "State.h"
#include "MenuNode.h"

#include <SFML/Graphics.hpp>

class CMenuState : public CState {
	public:
		CMenuState();
		~CMenuState();

		void onCreate();
		void onDestroy();
		void onEnter();
		void onExit();
		void onUpdate(sf::RenderWindow& window);
		void onRender(sf::RenderWindow& window);

	private:
		CMenuNode *currentNode_ = nullptr; 
		CMenuNode *rootNode_ = nullptr;

		sf::Font font;
		enum direction {UP, DOWN, LEFT};

		bool keyPressed = false;
		bool forceSurroundingWalls = true;

		void checkMenuCallback(CMenuNode::callbackType callBack, CMenuNode *node);
};

