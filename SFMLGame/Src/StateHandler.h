#include <unordered_map>
#include "state.h"

#include <SFML/Graphics.hpp>

#pragma once
class CStateHandler {
public:
	static CStateHandler &getInstance();
	~CStateHandler();

	void initState(std::string stateName, CState *newState, int screenHeight, int screenWidth);
	void destroyState(std::string stateName);
	void renderCurrState(sf::RenderWindow& window);
	void updateCurrState();
	void changeState(std::string stateName);

protected:
	CStateHandler() {};
private:
	static CStateHandler *instance_;
	std::unordered_map < std::string, CState*> myStates_;
	CState *currentState;
};

#define STATEHANDLER CStateHandler::getInstance()

