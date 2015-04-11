#pragma once

#include <vector>

#include "vector2D.h"

class CRRTNode {
	public:
		CRRTNode(CVector2D *position);
		~CRRTNode();

		void addChildNode(CVector2D *position);
		CRRTNode *getClosestNode(CVector2D position);
		CRRTNode *getParent() const { return parentNode_; }
		CRRTNode *getLastChildNode() const { return childNodes_[childNodes_.size()-1]; }
		CVector2D getPosition() const { return *position_; }

	private: 
		CVector2D *position_ = nullptr;
		std::vector<CRRTNode*> childNodes_;

		CRRTNode *parentNode_ = nullptr;

};