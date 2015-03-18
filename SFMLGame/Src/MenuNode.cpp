#include "MenuNode.h"
#include <iostream>


CMenuNode::CMenuNode(sf::Text nodeText, sf::Vector2f pos, CMenuNode::callbackType buttonCallback) {
	nodeText_ = nodeText;
	callID_ = buttonCallback;
	position_ = pos;
	nodeText_.setPosition(pos);
	nodeText_.setColor(sf::Color::White);
}

CMenuNode::CMenuNode(sf::Text nodeText, sf::Vector2f pos, CMenuNode *parent, CMenuNode::callbackType buttonCallback) {
	nodeText_ = nodeText;
	parentNode_ = parent;
	callID_ = buttonCallback;
	position_ = pos;
	nodeText_.setPosition(pos);
	nodeText_.setColor(sf::Color::White);
}

CMenuNode::~CMenuNode() {
	for (auto node : childNodes_) {
		delete node;
	}
}

void CMenuNode::addChildNode(sf::Text nodeText, sf::Vector2f pos, CMenuNode::callbackType buttonCallback) {
	CMenuNode *newNode = new CMenuNode(nodeText, pos, buttonCallback);
	newNode->parentNode_ = this;

	childNodes_.push_back(newNode);
	if (rootNode) {
		if (selectedNode_ == nullptr) {
			selectedNode_ = newNode;
			selectedNode_->nodeText_.setColor(sf::Color::Red);
		}
	}
}

void CMenuNode::selectNextNode() {
	if (selectedNode_ == nullptr) {
		return;
	}
	selectedNode_->nodeText_.setColor(sf::Color::White);
	for (int i = 0; i < childNodes_.size(); i++) {
		if (selectedNode_ == childNodes_[i]) {
			if (i < childNodes_.size() - 1) {
				selectedNode_ = childNodes_[i + 1];
			} else {
				selectedNode_ = childNodes_[0];
			}
			selectedNode_->nodeText_.setColor(sf::Color::Red);
			return;
		}
	}
}

void CMenuNode::selectPreviousNode() {
	if (selectedNode_ == nullptr) {
		return;
	}
	selectedNode_->nodeText_.setColor(sf::Color::White);
	for (int i = 0; i < childNodes_.size(); i++) {
		if (selectedNode_ == childNodes_[i]) {
			if (i > 0) {
				selectedNode_ = childNodes_[i - 1];
			} else {
				selectedNode_ = childNodes_[childNodes_.size()-1];
			}
			selectedNode_->nodeText_.setColor(sf::Color::Red);
			return;
		}
	}
}

void CMenuNode::deselectNode() {
	if (selectedNode_ != nullptr) {
		selectedNode_->nodeText_.setColor(sf::Color::White);
		selectedNode_ = nullptr;
	}
}

void CMenuNode::enterNode() {
	std::string nodeText = this->nodeText_.getString();
	if (childNodes_.size() > 0) {
		selectedNode_ = childNodes_[0];
		selectedNode_->nodeText_.setColor(sf::Color::Red);
	}
	std::cout << "Entered Node: " << (std::string)this->nodeText_.getString() << std::endl;
}

