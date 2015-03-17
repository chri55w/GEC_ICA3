#include "MenuNode.h"


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

	if (selectedNode_ == nullptr) {
		selectedNode_ = newNode;
	}
}

void CMenuNode::selectNextNode() {
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
	selectedNode_->nodeText_.setColor(sf::Color::White);
	for (int i = 0; i < childNodes_.size(); i++) {
		if (selectedNode_ == childNodes_[i]) {
			if (i > 1) {
				selectedNode_ = childNodes_[i - 1];
			} else {
				selectedNode_ = childNodes_[childNodes_.size()-1];
			}
			selectedNode_->nodeText_.setColor(sf::Color::Red);
			return;
		}
	}
}

void CMenuNode::pressNode() {

}