/*
*	RGBColour.cpp
*
*	An RGB color is a useful tool for passing around OpenGL style colors
*	represented as three floating numbers between 0.0f and 1.0f.
*	NOTE that the instance variables are public!
*
*	BCollett 10/1/2003 based on GeometryHelpers.
*/
#include "Geometry3d.h"
/*
*	A color is very simple.
*	Constructors, including copy.
*/
RGBColour::RGBColour(double r, double g, double b){
	mRed = r;
	mGreen = g;
	mBlue = b;
}
RGBColour::RGBColour(GLfloat r, GLfloat g, GLfloat b){
	mRed = r;
	mGreen = g;
	mBlue = b;
}
RGBColour::RGBColour(const RGBColour& c)
{
	mRed = c.mRed;
	mGreen = c.mGreen;
	mBlue = c.mBlue;
}
//
//	Assignment
//
RGBColour& RGBColour::operator=(const RGBColour& c) {
	mRed = c.mRed;
	mGreen = c.mGreen;
	mBlue = c.mBlue;
	return *this;
}
//
// And abilty to alter.
//
RGBColour& RGBColour::Set(GLfloat r, GLfloat g, GLfloat b) {
	mRed = r;
	mGreen = g;
	mBlue = b;
	return *this;
}
#ifdef UseOpenGL
//
//  Override Draw.
//
void RGBColour::Draw()
{
  glColor3f(mRed, mGreen, mBlue);
}
#endif
