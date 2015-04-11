#pragma once
#include "GameState.h"

class CDemoState : public CGameState {
public:
	CDemoState() {};
	~CDemoState() {};
	void onUpdate(sf::RenderWindow& window) override;
	void onRender(sf::RenderWindow& window) override;
};

