/*
 *  Point3D.cpp
 *  Simulator1_13_09
 *
 *  Updated BCollett 2/28/14 to support fuzzy comparisom.
 *  Created by Brian Collett on 7/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include <math.h>
#include "Geometry3d.h"

/*
 *  Point equality is controlled by class variables.
 */
Real Point3D::cAbsDiff = 0.001;
Real Point3D::cRelDiff = 0.0001;
/*
 *	Points support a few operations.
 */
//
//	Construct/Destruct
//
Point3D::Point3D(Real nx, Real ny, Real nz) {
	mX = nx;
	mY = ny;
	mZ = nz;
}
Point3D::Point3D(const float* v) {
	mX = (Real) v[0];
	mY = (Real) v[1];
	mZ = (Real) v[2];
}
Point3D::Point3D(const double* v) {
	mX = (Real) v[0];
	mY = (Real) v[1];
	mZ = (Real) v[2];
}

//
//	Need copy constructor and assignment op.
//
Point3D::Point3D(const Point3D& oPt) {
	mX = oPt.mX;
	mY = oPt.mY;
	mZ = oPt.mZ;
}

Point3D& Point3D::operator=(const Point3D& oPt) {
	mX = oPt.mX;
	mY = oPt.mY;
	mZ = oPt.mZ;
	return *this;
}	

//
//	Operations on points.
//
//	Set can reset the components of a vector.
//
Point3D& Point3D::Set(Real nx, Real ny, Real nz) {
	mX = nx;
	mY = ny;
	mZ = nz;
	return *this;
}
Point3D& Point3D::Set(const float* parray) {
	for (int i = 0; i < 3; ++i) {
		mCoords[i] = parray[i];
	}
	return *this;
}
Point3D& Point3D::Set(const double* parray) {
	for (int i = 0; i < 3; ++i) {
		mCoords[i] = parray[i];
	}
	return *this;
}

//
//	Can subtract two points to get a vector.
//	Can add or subtract a vector to a point giving a point.
//
Vector3D Point3D::operator-(const Point3D& oPt) const {
	return Vector3D(mX - oPt.mX, mY - oPt.mY, mZ - oPt.mZ);
}
/*
 *	Can add a vector to a point to get a new point.
 */
Point3D Point3D::operator+(const Vector3D& v) const 
{
	return Point3D(mX + v.mX, mY + v.mY, mZ + v.mZ);
}
/*
 *	Or to modify the current point.
 */
Point3D& Point3D::operator+=(const Vector3D& v) {
	mX += v.mX;
	mY += v.mY;
	mZ += v.mZ;
	return *this;
}
/*
 *	Likewise for subtracting a vector from a point.
 */
Point3D Point3D::operator-(const Vector3D& v) const
{
	return Point3D(mX - v.mX, mY - v.mY, mZ - v.mZ);
}
Point3D& Point3D::operator-=(const Vector3D& v) {
	mX -= v.mX;
	mY -= v.mY;
	mZ -= v.mZ;
	return *this;
}
//
//  Can see if two points are the same.
//
bool Point3D::operator==(const Point3D& p) const
{
  if (fabs(mX - p.mX) > cAbsDiff + cRelDiff * fabs(mX)) {
    return false;
  }
  if (fabs(mY - p.mY) > cAbsDiff + cRelDiff * fabs(mY)) {
    return false;
  }
  if (fabs(mZ - p.mZ) > cAbsDiff + cRelDiff * fabs(mZ)) {
    return false;
  }
  return true;
}
//
//	Can compute the distance to some other point.
//
Real Point3D::DistanceTo(const Point3D& p) const
{
	return (GLfloat) sqrt((mX-p.mX)*(mX-p.mX)+
												(mY-p.mY)*(mY-p.mY)+
												(mZ-p.mZ)*(mZ-p.mZ));
}

Real Point3D::DistSquaredTo(const Point3D& p) const
{
	return ((mX-p.mX)*(mX-p.mX)+(mY-p.mY)*(mY-p.mY)+(mZ-p.mZ)*(mZ-p.mZ));
}
