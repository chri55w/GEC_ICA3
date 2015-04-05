#pragma once

#include <vector>

class CVector2D {
public:
	CVector2D(int x, int y) : xPos_(x), yPos_(y) {};
	~CVector2D();

	float xPos_;
	float yPos_;

	void asUnitVector(CVector2D posA, CVector2D posB);
	float distance(CVector2D posB);
	void normalize();

	CVector2D& operator+=(CVector2D vectorToAdd);
	CVector2D& operator-=(CVector2D vectorToSubtract);
	
};

