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
		void onUpdate();
		void onRender(sf::RenderWindow& window, int s_height, int s_width);

	private:
		CMenuNode *currentNode_ = nullptr; 
		CMenuNode *rootNode_ = nullptr;

		sf::Font font;
		enum direction {UP, DOWN, LEFT};

		bool keyPressed = false;
		bool forceSurroundingWalls = true;

		void checkMenuCallback(CMenuNode::callbackType callBack, CMenuNode *node);
};

