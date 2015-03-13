#pragma once
#include "State.h"

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
		sf::Font font;
		enum direction {UP, DOWN, LEFT};

		struct textObj {
			sf::Text text;
			bool selectable;
			int selectCondition;

		};

		std::vector<textObj> menuTexts;
		std::vector<bool> selectConditions;

		bool keyPressed = false;

		int selectedText;

		void newText(std::string text, sf::Font, int size, float xPos, float yPos, bool canSelect = false, int selectCondition = 0);
		void resetSelectedText();
		void tryMoveMenu(direction moveDir);
		void selectMenuItem();
		bool stringContains(std::string stringToCheck, std::string stringToFind);
};

