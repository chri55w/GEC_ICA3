#include "MenuNode.h"
#include <iostream>


CMenuNode::CMenuNode(sf::Text nodeText, sf::Vector2f pos, CMenuNode::callbackType buttonCallback) {

	//Menu Node Constructer with no parent (only used for 
	//		rootnode)
	nodeText_ = new sf::Text(nodeText);
	callID_ = buttonCallback;
	position_ = pos;
	nodeText_->setPosition(pos);
	nodeText_->setColor(sf::Color::White);
}

CMenuNode::CMenuNode(sf::Text nodeText, sf::Vector2f pos, CMenuNode *parent, CMenuNode::callbackType buttonCallback) {

	//Standard Menu Node constructer with text element, 
	//		parent node pointer, call ID enum and a 
	//		position vector.
	nodeText_ = new sf::Text(nodeText);
	parentNode_ = parent;
	callID_ = buttonCallback;
	position_ = pos;
	nodeText_->setPosition(pos);
	nodeText_->setColor(sf::Color::White);
}

CMenuNode::~CMenuNode() {

	//CleanUp!
	delete nodeText_;
	for (auto node : childNodes_) {
		delete node;
	}
}

void CMenuNode::addChildNode(sf::Text nodeText, sf::Vector2f pos, CMenuNode::callbackType buttonCallback) {

	//Add a new child node pointer to the vector with its parent 
	//		defined as this node, if selected node is a nullptr 
	//		then assign this new child node to be the selected node.
	CMenuNode *newNode = new CMenuNode(nodeText, pos, buttonCallback);
	newNode->parentNode_ = this;

	childNodes_.push_back(newNode);
	if (rootNode) {
		if (selectedNode_ == nullptr) {
			selectedNode_ = newNode;
			selectedNode_->nodeText_->setColor(sf::Color::Red);
		}
	}
}

void CMenuNode::selectNextNode() {

	//Ensure a selected node exists to prevent crashes
	if (selectedNode_ == nullptr) {
		return;
	}

	//Reset the colour of the currently selected node to white.
	selectedNode_->nodeText_->setColor(sf::Color::White);

	//Loop through the child nodes and find the child node equal 
	//		to the current node, once found try to move to the next 
	//		index in the child nodes vectot, if there is no next 
	//		node return to the start of the child nodes vector
	for (unsigned int i = 0; i < childNodes_.size(); i++) {
		if (selectedNode_ == childNodes_[i]) {
			if (i < childNodes_.size() - 1) {
				selectedNode_ = childNodes_[i + 1];
			} else {
				selectedNode_ = childNodes_[0];
			}

			//Assign the new selected node colour to red.
			selectedNode_->nodeText_->setColor(sf::Color::Red);
			return;
		}
	}
}

void CMenuNode::selectPreviousNode() {

	//Ensure a selected node exists to prevent crashes
	if (selectedNode_ == nullptr) {
		return;
	}

	//Reset the colour of the currently selected node to white.
	selectedNode_->nodeText_->setColor(sf::Color::White);

	//Loop through the child nodes and find the child node equal 
	//		to the current node, once found try to move to the previous 
	//		index in the child nodes vectot, if there is no previous 
	//		node move to the end of the child nodes vector
	for (unsigned int i = 0; i < childNodes_.size(); i++) {
		if (selectedNode_ == childNodes_[i]) {
			if (i > 0) {
				selectedNode_ = childNodes_[i - 1];
			} else {
				selectedNode_ = childNodes_[childNodes_.size()-1];
			}

			//Assign the new selected node colour to red.
			selectedNode_->nodeText_->setColor(sf::Color::Red);
			return;
		}
	}
}

void CMenuNode::deselectNode() {
	//If the selected node is not null then reset 
	//		the colour of the node to white and set 
	//		the selected node pointer to null.
	if (selectedNode_ != nullptr) {
		selectedNode_->nodeText_->setColor(sf::Color::White);
		selectedNode_ = nullptr;
	}
}

void CMenuNode::enterNode() {
	//If Child nodes exist then assign the selected 
	//		node to the first child node and set its 
	//		text colour to red.
	if (childNodes_.size() > 0) {
		selectedNode_ = childNodes_[0];
		selectedNode_->nodeText_->setColor(sf::Color::Red);
	}
	std::cout << "Entered Node: " << (std::string)this->nodeText_->getString() << std::endl;
}

