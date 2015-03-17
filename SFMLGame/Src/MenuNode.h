#pragma once
#include <String>
#include <vector>

#include <SFML/Graphics.hpp>

class CMenuNode {
	public:

		enum callbackType { NULLCALL, STARTGAME, EXIT, FORCEWALLS };
		CMenuNode(sf::Text nodeText, sf::Vector2f pos, CMenuNode *parent, callbackType buttonCallback = NULLCALL);
		CMenuNode(sf::Text nodeText, sf::Vector2f pos, callbackType buttonCallback = NULLCALL); 
		CMenuNode() { rootNode = true; } //RootNode Creation
		~CMenuNode();
		sf::Text getText() { return nodeText_; };
		CMenuNode *getParentNode() const { return parentNode_; }
		CMenuNode *fetchNode(int nodeID) const { return childNodes_[nodeID];}
		CMenuNode *getSelectedNode() { return selectedNode_; }

		sf::Vector2f getPosition() const { return position_; }


		unsigned int countChildren() const { return childNodes_.size(); }

		void addChildNode(sf::Text nodeText, sf::Vector2f pos, callbackType buttonCallback = NULLCALL);

		void selectNextNode();
		void selectPreviousNode();

		void pressNode();

	private:

		sf::Text nodeText_;
		sf::Vector2f position_;
		std::vector<CMenuNode*> childNodes_;
		CMenuNode *selectedNode_ = nullptr;
		CMenuNode *parentNode_ = nullptr;
		callbackType callID_ = callbackType::NULLCALL;
		bool rootNode = false;
};

