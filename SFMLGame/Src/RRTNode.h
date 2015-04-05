#pragma once

#include <vector>

#include "vector2D.h"

class CRRTNode {
	public:
		CRRTNode(int x, int y);
		~CRRTNode();

		void addChildNode(CVector2D position);
		CRRTNode *getClosestNode(CVector2D position);
		CVector2D getPosition() const { return *position_; }

	private: 
		CVector2D *position_ = nullptr;
		std::vector<CRRTNode*> childNodes_;

};

