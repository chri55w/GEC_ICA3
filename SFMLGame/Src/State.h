#include <SFML/Graphics.hpp>

#pragma once
class CState {
	public:
		//Base state class

		virtual void onCreate() = 0;
		virtual void onDestroy() = 0;
		virtual void onEnter() = 0;
		virtual void onExit() = 0;
		virtual void onUpdate() = 0;
		virtual void onRender(sf::RenderWindow& window) = 0;

		void setScreenDimensions(int screenHeight, int screenWidth)  { s_width = screenWidth; s_height = screenHeight; }

	protected:
		int s_width;
		int s_height;
};