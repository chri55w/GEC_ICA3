#include "RRTNode.h"



CRRTNode::CRRTNode(CVector2D *position) {
	//Assign the provided x and y values to a new vec2
	position_ = position;
}

CRRTNode::~CRRTNode() {
	//Clean Up!
	delete position_;
	for (auto c : childNodes_) {
		delete c;
	}
}

void CRRTNode::addChildNode(CVector2D *position) {
	//Push a new node into the tree as a child of this node.
	CRRTNode *newChildNode = new CRRTNode(position);
	newChildNode->parentNode_ = this;
	childNodes_.push_back(newChildNode);
}

CRRTNode *CRRTNode::getClosestNode(CVector2D position) {

	//If this node has no children then return this as the 
	//		closest node in this branch of the tree
	if (childNodes_.size() == 0) {
		return this;
	}

	//Assign a closest child variable to the first child 
	//		in the array then calculate the distance from 
	//		the random position to that child node
	CRRTNode *closestChild = childNodes_[0];
	float distToClosestChild = closestChild->getPosition().distance(position);

	//Loop through all child nodes which will call this method 
	//		in each child node to recursively fetch the closest 
	//		child working from the base of the tree back up to the root node
	for (unsigned int i = 0; i < childNodes_.size(); i++) {

		//Fetch the closest node for this child (comparing child nodes)
		CRRTNode *thisClosestChild = childNodes_[i]->getClosestNode(position);

		//Calculate the distance to this child
		float distToThisClosestChild = thisClosestChild->getPosition().distance(position);

		//Compare the distance between this child node and the previous 
		//		closest child, if the distance to this child is less assign 
		//		this child as the closest child and assign the dist to closest child too.
		if (distToThisClosestChild < distToClosestChild) {
			closestChild = thisClosestChild;
			distToClosestChild = distToThisClosestChild;
		}		
	}
	//Once the closest child has been identified through each child node 
	//		down to the bottom of the tree compare the distance between 
	//		that node and the random position to this node and the position. 
	//		Finally return the closest node identified.
	float distToThis = position_->distance(position);
	if (distToThis > distToClosestChild) {
		return closestChild;
	}
	return this;
}
