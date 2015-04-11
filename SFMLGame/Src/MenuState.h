#pragma once
#include "State.h"
#include "MenuNode.h"

#include <SFML/Graphics.hpp>

class CMenuState : public CState {
	public:
		CMenuState() {};
		~CMenuState() {};

		void onCreate();
		void onDestroy();
		void onEnter();
		void onExit();
		void onUpdate(sf::RenderWindow& window);
		void onRender(sf::RenderWindow& window);

	private:
		//Pointers to nodes for the menu system
		CMenuNode *currentNode_ = nullptr; 
		CMenuNode *rootNode_ = nullptr;

		//Font declaration for menu node text
		sf::Font font;

		//Direction enum for quick reference to each arrow 
		//		key (used in button callbacks)
		enum direction {UP, DOWN, LEFT};

		//Boolean tracking the state of a key press, used 
		//		to prevent multiple key proccessing for a single 
		//		long key press.
		bool keyPressed = false;

		//Settings Variables.
		bool forceSurroundingWalls = true;

		//Function prototype for buttons with a callback when 
		//		pressed (settings etc).
		void checkMenuCallback(CMenuNode::callbackType callBack, CMenuNode *node);
};

