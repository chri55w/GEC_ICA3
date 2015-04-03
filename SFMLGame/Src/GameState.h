#pragma once

#include "State.h"
#include <sfml/Graphics.hpp>

class CGameState : public CState {
	public:
		CGameState();
		~CGameState();

		void onCreate();
		void onDestroy();
		void onEnter();
		void onExit();
		void onUpdate();
		void onRender(sf::RenderWindow& window);

	private:
		sf::VertexArray mapImage;

		sf::Font font;
		sf::Text *textTip = nullptr;

		bool exploring = true;

		void tryRandomPoint(int randX, int randY);
};

