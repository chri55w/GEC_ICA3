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
	void onRender(sf::RenderWindow& window);
};

