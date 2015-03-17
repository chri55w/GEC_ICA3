#include "MenuNode.h"


CMenuNode::CMenuNode(sf::Text nodeText, sf::Vector2f pos, CMenuNode::callbackType buttonCallback) {
	nodeText_ = nodeText;
	callID_ = buttonCallback;
	position_ = pos;
	nodeText_.setPosition(pos);
}

CMenuNode::CMenuNode(sf::Text nodeText, sf::Vector2f pos, CMenuNode *parent, CMenuNode::callbackType buttonCallback) {
	nodeText_ = nodeText;
	parentNode_ = parent;
	callID_ = buttonCallback;
	position_ = pos;
	nodeText_.setPosition(pos);
}

CMenuNode::~CMenuNode() {
	for (auto node : childNodes_) {
		delete node;
	}
}

void CMenuNode::addChildNode(sf::Text nodeText, sf::Vector2f pos, CMenuNode::callbackType buttonCallback) {
	CMenuNode *newNode = new CMenuNode(nodeText, pos, buttonCallback);

	childNodes_.push_back(newNode);

	if (selectedNode_ == nullptr) {
		selectedNode_ = newNode;
	}
}

void CMenuNode::selectNextNode() {
	for (int i = 0; i < childNodes_.size(); i++) {
		if (selectedNode_ == childNodes_[i]) {
			if (i < childNodes_.size() - 1) {
				selectedNode_ = childNodes_[i + 1];
			} else {
				selectedNode_ = childNodes_[0];
			}
			return;
		}
	}
}

void CMenuNode::selectPreviousNode() {
	for (int i = 0; i < childNodes_.size(); i++) {
		if (selectedNode_ == childNodes_[i]) {
			if (i > 1) {
				selectedNode_ = childNodes_[i - 1];
			} else {
				selectedNode_ = childNodes_[childNodes_.size()-1];
			}
			return;
		}
	}
}

void CMenuNode::pressNode() {

}