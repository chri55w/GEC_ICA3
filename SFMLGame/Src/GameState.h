#pragma once

#include "State.h"
#include <sfml/Graphics.hpp>
#include "RRTNode.h"
#include "Vector2D.h"


class CGameState : public CState {
	public:
		CGameState() {};
		~CGameState() {};

		void onCreate();
		void onDestroy();
		void onEnter();
		void onExit();
		void onUpdate(sf::RenderWindow& window);
		void onRender(sf::RenderWindow& window);

	protected:
		//Max and Minimum distances between nodes.
		int minDist = 5;
		int maxDist = 25;
		int nodeCollisionSqrRadius = 3;

		//Offset of the map to draw each map central to the screen.
		int mapOffsetX = 0;
		int mapOffsetY = 0;

		//Map, Nodes and Path image for quick drawing on render.
		sf::VertexArray mapImage;
		sf::VertexArray nodesImage;
		sf::VertexArray pathImage;

		//Circle shapes defining nodes, and highlighted nodes.
		//		Rect shapes representing start and endNodes.
		std::vector<sf::CircleShape> nodePositions;
		sf::CircleShape nodeHighlight;
		sf::CircleShape randHighlight;
		sf::CircleShape closestHighlight;
		sf::RectangleShape startPosHighlight;
		sf::RectangleShape endPosHighlight;
		
		//Font and Text objects for drawing tips and node counts
		//		as well as the vector of all tip texts.
		sf::Font font;
		sf::Text *textTip = nullptr;
		std::vector<std::string> textStringTips;
		sf::Text *nodeCountText;

		//Variables effecting text drawing and the counting of nodes.
		int nodeCount;
		int selectedText = 0;

		//Variables to recognise the current state of the game and 
		//		identify what the update loop should be calling.
		bool startNodeSelected = false;
		bool exploring = false;
		bool endNodeSelected = false;
		bool destinationReached = false;

		//Boolean checking if the mouse is clicked for prevention of 
		//		multiple click processing from a long single click.
		bool mouseClicked = false;

		//A pointer to the root node, used to access the RRT Tree.
		CRRTNode *rootNode_ = nullptr;

		//Funtion Prototypes for functions only called at state entry.
		void setUpMapVertexArray();

		//Function Prototypes for functions called at run time.
		void updateTextTips();
		void tryRandomPoint(CVector2D randPos);
		void findClosestNode(int x, int y);
		void tryDrawLine(CRRTNode &posA, CVector2D posB, CVector2D direction);
		bool canDraw(CVector2D position);
		void drawLine(std::vector<CVector2D> positionsToDraw);
		void drawLine(CVector2D posA, CVector2D posB);
		void addNode(CRRTNode &parentNode, CVector2D *position);

		CRRTNode *tryEndNode(CVector2D posToReach);

		std::vector<CVector2D> path;
};

