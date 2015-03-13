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
		void onRender(sf::RenderWindow& window, int s_height, int s_width);

	private:
		sf::RectangleShape square;
};
