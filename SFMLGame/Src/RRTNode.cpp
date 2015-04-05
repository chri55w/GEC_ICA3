#include "RRTNode.h"



CRRTNode::CRRTNode(int x, int y) {
	position_ = new CVector2D(x, y);
}


CRRTNode::~CRRTNode() {
}


void CRRTNode::addChildNode(CVector2D position) {
	childNodes_.push_back(new CRRTNode(position.xPos_, position.yPos_));
}



CRRTNode *CRRTNode::getClosestNode(CVector2D position) {
	if (childNodes_.size() == 0) {
		return this;
	}
	CRRTNode *closestChild = childNodes_[0];
	int distToClosestChild = closestChild->getPosition().distance(position);
	for (int i = 0; i < childNodes_.size(); i++) {
		CRRTNode *thisClosestChild = childNodes_[i]->getClosestNode(position);
		int distToThisClosestChild = thisClosestChild->getPosition().distance(position);
		if (distToThisClosestChild < distToClosestChild) {
			closestChild = thisClosestChild;
			distToClosestChild = distToThisClosestChild;
		}		
	}
	int distToThis = position_->distance(position);
	if (distToThis > distToClosestChild) {
		return closestChild;
	}
	return this;
}
