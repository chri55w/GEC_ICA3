#pragma once
#include <String>

#include <SFML/Graphics.hpp>
class CMenuNode {
	public:
		CMenuNode();
		~CMenuNode();
		sf::Text getText() const { return nodeText_; };

		void addNode(std::string nodeText /*, someway to callback*/ );

	private:
		sf::Text nodeText_;
		std::vector<CMenuNode*> childNodes;
};

