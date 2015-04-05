#include "StateHandler.h"
#include <iostream>


//Set up singleton instance
CStateHandler *CStateHandler::instance_ = NULL;

CStateHandler &CStateHandler::getInstance() {
	if (instance_ == NULL) {
		instance_ = new CStateHandler();
	}
	return *instance_;
}
//Clean up all states
CStateHandler::~CStateHandler() {
	for (auto state : myStates_) {
		if (state.second != nullptr) {
			state.second->onDestroy();
			delete state.second;
		}
	}
}
//initialise states on creation and run the on create function.
void CStateHandler::initState(std::string stateName, CState *newState, int screenHeight, int screenWidth) {
	myStates_[stateName] = newState;
	newState->onCreate();
	newState->setScreenDimensions(screenHeight, screenWidth);
}
//Destroy a state
void CStateHandler::destroyState(std::string stateName) {
	if (myStates_.find(stateName) != myStates_.end() && myStates_.find(stateName)->second != nullptr) {
		if (currentState == myStates_[stateName]) {
			currentState = nullptr;
		}
		myStates_[stateName]->onDestroy();
		delete myStates_[stateName];
		myStates_[stateName] = nullptr;
	} else {
		std::cerr << "ERROR: Cannot Delete Non-Initialized or Null State! '" + stateName + "'" << std::endl;
	}
}
//Change to a different state
void CStateHandler::changeState(std::string stateName) {
	if (myStates_.find(stateName) != myStates_.end() && myStates_.find(stateName)->second != nullptr) {
		if (currentState != nullptr) {
			currentState->onExit();
		}
		currentState = myStates_[stateName];
		currentState->onEnter();
	} else {
		std::cerr << "ERROR: Cannot Change to Non-Initialized or Null State! '" + stateName + "'" << std::endl;
	}
}
//update the state you are currently in
void CStateHandler::updateCurrState(sf::RenderWindow& window) {
	if (currentState != nullptr) {
		currentState->onUpdate(window);
	}
}
//render the state you are currently in
void CStateHandler::renderCurrState(sf::RenderWindow& window) {
	if (currentState != nullptr) {
		currentState->onRender(window);
	}
}