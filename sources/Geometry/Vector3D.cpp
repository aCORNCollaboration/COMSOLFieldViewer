/*
 *  Vector3D.cpp
 *  Simulator1_13_09
 *
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include <math.h>
#include "Geometry3d.h"
//
//  Class vars control comparison.
//
Real Vector3D::cMagRelDiff = 0.01;  // 1%
Real Vector3D::cAngDiff = 0.01;     // 10mRad, about 0.5 degree
//
//	Construct/Destruct
//
Vector3D::Vector3D(Real nx, Real ny, Real nz) 
{
	mX = nx;
	mY = ny;
	mZ = nz;
  mLength = (Real) sqrt(mX*mX + mY*mY + mZ*mZ);
}
Vector3D::Vector3D(float* v)
{
	mX = (Real) v[0];
	mY = (Real) v[1];
	mZ = (Real) v[2];
  mLength = (Real) sqrt(mX*mX + mY*mY + mZ*mZ);
}
Vector3D::Vector3D(double* v)
{
	mX = (Real) v[0];
	mY = (Real) v[1];
	mZ = (Real) v[2];
  mLength = (Real) sqrt(mX*mX + mY*mY + mZ*mZ);
}

//
//	Need copy constructor and assignment op.
//
Vector3D::Vector3D(const Vector3D& v) 
{
	mX = v.mX;
	mY = v.mY;
	mZ = v.mZ;
  mLength = (Real) sqrt(mX*mX + mY*mY + mZ*mZ);
}

Vector3D& Vector3D::operator=(const Vector3D& v) 
{
	mX = v.mX;
	mY = v.mY;
	mZ = v.mZ;
  mLength = (Real) sqrt(mX*mX + mY*mY + mZ*mZ);
	return *this;
}

//
//	Operations on Vectors.
//	Can find length and spherical angular components.
//
Real Vector3D::Length() const
{
  return mLength;
}
Real Vector3D::LengthSq() const
{
  return mLength * mLength;
}
/*
 *   These are  NOT as unsafe as they look because atan2 correctly 
 *   handles the case where its second argument is 0.
 */
Real Vector3D::AngTheta() const
{
	double rad = sqrt(mX*mX + mY*mY);
	return Real(atan2(rad, mZ));
}
/*
 *   Just as theta is odd so is phi.
 */
Real Vector3D::AngPhi() const
{
	return Real(atan2(mY, mX));
}

//
//	Set can reset the components of a vector.
//
Vector3D& Vector3D::Set(Real nx, Real ny, Real nz) 
{
	mX = nx;
	mY = ny;
	mZ = nz;
  mLength = (Real) sqrt(mX*mX + mY*mY + mZ*mZ);
	return *this;
}

//
//	Can add or subtract a vector to a vector giving a vector.
//	Each has both a form creating a new vector and a form
//	to alter this vector.
//
Vector3D Vector3D::operator+(const Vector3D& v) const
{
	return Vector3D(mX + v.mX, mY + v.mY, mZ + v.mZ);
}

Vector3D& Vector3D::operator+=(const Vector3D& v) 
{
	mX += v.mX;
	mY += v.mY;
	mZ += v.mZ;
  mLength = (Real) sqrt(mX*mX + mY*mY + mZ*mZ);
	return *this;
}

Vector3D Vector3D::operator-(const Vector3D& v) const
{
	return Vector3D(mX - v.mX, mY - v.mY, mZ + v.mZ);
}

Vector3D& Vector3D::operator-=(const Vector3D& v) 
{
	mX -= v.mX;
	mY -= v.mY;
	mZ -= v.mZ;
  mLength = (Real) sqrt(mX*mX + mY*mY + mZ*mZ);
	return *this;
}

//
//	Can multiply or divide a vector by a scalar to get a new
//	vector.
//
Vector3D Vector3D::operator*(Real s) const
{
	return Vector3D(mX * s, mY * s, mZ * s);
}

Vector3D& Vector3D::operator*=(Real s) 
{
	mX *= s;
	mY *= s;
	mZ *= s;
  mLength = (Real) sqrt(mX*mX + mY*mY + mZ*mZ);
	return *this;
}

Vector3D Vector3D::operator/(Real s) const
{
	return Vector3D(mX / s, mY / s, mZ / s);
}

Vector3D& Vector3D::operator/=(Real s) 
{
	mX /= s;
	mY /= s;
	mZ /= s;
  mLength = (Real) sqrt(mX*mX + mY*mY + mZ*mZ);
	return *this;
}
//
//  Can see if two vectors are the same to within tolerance.
//
bool Vector3D::operator==(Vector3D& v) const
{
  double angle = acos(Dot(v)/(mLength * v.mLength));
  if (angle > cAngDiff) return false;
  if ((mLength - v.mLength) > 0.01 * mLength) return false;
  return true;
}
//
//	Dotting two vectors gives a scalar. Crossing them gives
//	a new vector.
//
Real Vector3D::Dot(const Vector3D& v) const
{
	return (mX * v.mX + mY * v.mY + mZ * v.mZ);
}

Vector3D Vector3D::Cross(const Vector3D& v) const
{
	return Vector3D(mY * v.mZ - mZ * v.mY,
						 mZ * v.mX - mX * v.mZ,
						 mX * v.mY - mY * v.mX);
}
//
//	Normalize sets the vector length to 1.
//  Norm returns a new normalized vector.
//
Vector3D Vector3D::Normalize() {
	Real length = Length();
	mX /= length;
	mY /= length;
	mZ /= length;
	return *this;
}
Vector3D Vector3D::Norm() const {
  return Vector3D(mX/mLength, mY/mLength, mZ/mLength);
}
