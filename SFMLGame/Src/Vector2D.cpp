#include "Vector2D.h"

CVector2D::~CVector2D() {
}

//This Function turns THIS vector into a normalized direction 
//		vector between two points.
void CVector2D::asUnitVector(CVector2D posA, CVector2D posB) {
	//Calculate the distance on each axis between each position provided
	float xDist = posB.xPos_ - posA.xPos_;
	float yDist = posB.yPos_ - posA.yPos_;

	//Set the and y position of this vector to the distances.
	xPos_ = xDist;
	yPos_ = yDist;

	//Normalise the distances.
	normalize();
}

float CVector2D::distance(CVector2D posB) {
	if (this->xPos_ == posB.xPos_ && this->yPos_ == posB.yPos_) {
		return 0;
	}

	//Calculate the distance between this vector and another vector as a 
	//		positive number (FABS FUNCTION) by minusing this positions 
	//		x and y location from the other positions x and y location.
	float xDist = fabs(posB.xPos_ - xPos_);
	float yDist = fabs(posB.yPos_ - yPos_);

	//To calculate the distance return the square root of (xDist^2 + yDist^2)
	return sqrt((xDist * xDist) + (yDist * yDist));
}

void CVector2D::normalize() {
	//Calculate the length of this vector from the origin position 0, 0.
	float length = this->distance(CVector2D(0, 0));

	//Divide the x and y position by the length returned to create a 
	//		vector of length 1 ( a normalised vector ).
	xPos_ /= length;
	yPos_ /= length;
}

CVector2D& CVector2D::operator+=(CVector2D vectorToAdd) {

	//Define the += operator with paramaters of another 
	//		vec2 as the addition of both the x and y positions 
	//		from both vectors.
	xPos_ += vectorToAdd.xPos_;
	yPos_ += vectorToAdd.yPos_;

	return *this;
}

CVector2D& CVector2D::operator-=(CVector2D vectorToSubtract) {

	//Define the -= operator with paramaters of another 
	//		vec2 as the subtraction of the x and y positions 
	//		of the paramater vector from this vector.
	xPos_ -= vectorToSubtract.xPos_;
	yPos_ -= vectorToSubtract.yPos_;

	return *this;
}