#pragma once

#include "State.h"
#include <sfml/Graphics.hpp>
#include "RRTNode.h"
#include "Vector2D.h"


class CGameState : public CState {
	public:
		CGameState();
		~CGameState();

		void onCreate();
		void onDestroy();
		void onEnter();
		void onExit();
		void onUpdate(sf::RenderWindow& window);
		void onRender(sf::RenderWindow& window);

	private:
		sf::VertexArray mapImage;
		sf::VertexArray nodesImage;
		std::vector<sf::CircleShape> nodePositions;
		sf::CircleShape nodeHighlight;
		sf::CircleShape randHighlight;
		sf::CircleShape closestHighlight;

		sf::Font font;
		sf::Text *textTip = nullptr;
		std::vector<std::string> textStringTips;
		int selectedText = 0;

		int nodeCount = 1;
		sf::Text nodeCountText = sf::Text(std::to_string(nodeCount), font, 10);

		bool startNodeSelected = false;
		bool exploring = false;
		bool endNodeSelected = false;
		bool destinationReached = false;

		bool mouseClicked = false;

		CRRTNode *rootNode_ = nullptr;

		void tryRandomPoint(CVector2D randPos);
		void findClosestNode(int x, int y);
		void tryDrawLine(CRRTNode &posA, CVector2D posB, CVector2D direction);
		bool canDraw(CVector2D position);
		void addNode(CRRTNode &parentNode, CVector2D position);

		int minDist = 5;
		int maxDist = 25;
};

