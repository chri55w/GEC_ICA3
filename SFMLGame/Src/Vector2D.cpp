#include "Vector2D.h"

CVector2D::~CVector2D() {
}

void CVector2D::asUnitVector(CVector2D posA, CVector2D posB) {
	float xDist = posB.xPos_ - posA.xPos_;
	float yDist = posB.yPos_ - posA.yPos_;

	xPos_ = xDist;
	yPos_ = yDist;

	normalize();
}

float CVector2D::distance(CVector2D posB) {
	float xDist = fabs(posB.xPos_ - xPos_);
	float yDist = fabs(posB.yPos_ - yPos_);

	return sqrt((xDist * xDist) + (yDist * yDist));
}

void CVector2D::normalize() {
	float length = this->distance(CVector2D(0, 0));
	xPos_ /= length;
	yPos_ /= length;
}

CVector2D& CVector2D::operator+=(CVector2D vectorToAdd) {
	xPos_ += vectorToAdd.xPos_;
	yPos_ += vectorToAdd.yPos_;

	return *this;
}

CVector2D& CVector2D::operator-=(CVector2D vectorToSubtract) {
	xPos_ -= vectorToSubtract.xPos_;
	yPos_ -= vectorToSubtract.yPos_;

	return *this;
}