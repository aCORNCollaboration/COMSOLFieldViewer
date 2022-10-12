/*
 *  Frame3D.cpp
 *  Simulator1_13_09
 *
 *	A Frame is a mathematical 3-D rectangular parallelipiped aligned
 *	with the coordinate axes. It is useful for representing bounding boxes.
 *
 *  BCollett added the isEmpty flag from Box3D so that we can really
 *  represent and empty frame. This allows us to add things to a
 *  completely empty box and not have an extra 'start point' in there.
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <math.h>
#include "assert.h"
#include "Geometry3d.h"
/*
 *	Frame3D
 *
 *	This is a class that encapsulates the idea of a rectangular
 *	parallelipiped (a box) oriented parallel to the coordinate
 *	axes. Its chief purpose is to describe the volume bounded
 *	by a 3-D model.
 *	A Frame3D supports a number of access methods that know practically
 *	anything you could want to know about a Frame3D. It also supports
 *	operations such as adding a point to the Frame3D (the Frame makes sure
 *	that it is big enough to contain the point), unioning two Frame3D's
 *	(giving a Frame3D that encompasses all of both), intersecting two
 *	Frame3D's, etc. It is NOT a drawable GeometryObject; for that you
 *  want a Box3D.
 */
//
//	Constructors and destructor.
//	Default constructor creates an empty box centred on
//	the origin. Alternate versions allow you to specify an
//	alternate location for the box and even an initial size.
//
Frame3D::Frame3D(const Point3D* startPoint) {
	if (startPoint != nullptr) {
    isEmpty = false;
		mMin.mX = mMax.mX = startPoint->mX;
		mMin.mY = mMax.mY = startPoint->mY;
		mMin.mZ = mMax.mZ = startPoint->mZ;
	} else {
    isEmpty = true;
		mMin.mX = mMax.mX = 0.0f;
		mMin.mY = mMax.mY = 0.0f;
		mMin.mZ = mMax.mZ = 0.0f;
	}
}
Frame3D::Frame3D(const Point3D& p)
{
  isEmpty = false;
  mMin.mX = mMax.mX = p.mX;
  mMin.mY = mMax.mY = p.mY;
  mMin.mZ = mMax.mZ = p.mZ;
}

//
//	Alternate constructor builds smallest box that encompasses
//	two given points. It does not assume that the two points are
//	sorted in any way.
//
Frame3D::Frame3D(const Point3D& start, const Point3D& end)
{
	if (start.mX <= end.mX) {
		mMin.mX = start.mX;
		mMax.mX = end.mX;
	} else {
		mMin.mX = end.mX;
		mMax.mX = start.mX;
	}
	if (start.mY <= end.mY) {
		mMin.mY = start.mY;
		mMax.mY = end.mY;
	} else {
		mMin.mY = end.mY;
		mMax.mY = start.mY;
	}
	if (start.mZ <= end.mZ) {
		mMin.mZ = start.mZ;
		mMax.mZ = end.mZ;
	} else {
		mMin.mZ = end.mZ;
		mMax.mZ = start.mZ;
	}
  isEmpty = false;
}
Frame3D::Frame3D(const Point3D* startPoint, const Point3D* endPoint) {
	if (startPoint->mX <= endPoint->mX) {
		mMin.mX = startPoint->mX;
		mMax.mX = endPoint->mX;
	} else {
		mMin.mX = endPoint->mX;
		mMax.mX = startPoint->mX;
	}
	if (startPoint->mY <= endPoint->mY) {
		mMin.mY = startPoint->mY;
		mMax.mY = endPoint->mY;
	} else {
		mMin.mY = endPoint->mY;
		mMax.mY = startPoint->mY;
	}
	if (startPoint->mZ <= endPoint->mZ) {
		mMin.mZ = startPoint->mZ;
		mMax.mZ = endPoint->mZ;
	} else {
		mMin.mZ = endPoint->mZ;
		mMax.mZ = startPoint->mZ;
	}
  isEmpty = false;
}
//
//	Destructor does nothing as we own no storage.
Frame3D::~Frame3D() {
}
//
//  Copy constructor and assignment operator.
//
Frame3D::Frame3D(const Frame3D& f)
{
  mMin = f.mMin;
  mMax = f.mMax;
  isEmpty = f.isEmpty;
}
Frame3D& Frame3D::operator=(const Frame3D& f)
{
  mMin = f.mMin;
  mMax = f.mMax;
  isEmpty = f.isEmpty;
  return *this;
}
//
//	Initialisers allow you to start with points or other
//	frames at any time, not only construction.
//
void Frame3D::SetMin(const Point3D& startPoint) {
	mMin = startPoint;
  isEmpty = false;
}
void Frame3D::SetMax(const Point3D& startPoint) {
	mMax = startPoint;
  isEmpty = false;
}
void Frame3D::SetMin3dv(double* vals) {
	mMin.mX = (Real) vals[0];
	mMin.mY = (Real) vals[1];
	mMin.mZ = (Real) vals[2];
  isEmpty = false;
}
void Frame3D::SetMax3dv(double* vals) {
	mMax.mX = (Real) vals[0];
	mMax.mY = (Real) vals[1];
	mMax.mZ = (Real) vals[2];
  isEmpty = false;
}

void Frame3D::Set(const Point3D& min, const Point3D& max) {
  assert(min.mX <= max.mX);
  assert(min.mY <= max.mY);
  assert(min.mZ <= max.mZ);
  mMin = min;
  mMax = max;
  isEmpty = false;
}
void Frame3D::Set(const Frame3D& frame) {
  if (frame.isEmpty) {
    isEmpty = true;
    return;
  }
	mMin.mX = frame.mMin.mX;
	mMax.mX = frame.mMax.mX;
	mMin.mY = frame.mMin.mY;
	mMax.mY = frame.mMax.mY;
	mMin.mZ = frame.mMin.mZ;
	mMax.mZ = frame.mMax.mZ;
  isEmpty = false;
}
void Frame3D::Clear()
{
  isEmpty = true;
}
//
//  Accessors.
//
const Point3D& Frame3D::GetMid() const
{
  static Point3D result;
  result.Set(0.5 * (mMin.mX + mMax.mX),
             0.5 * (mMin.mY + mMax.mY),
             0.5 * (mMin.mZ + mMax.mZ));
  return result;
}
//
//	This tells us whether a point is in the box.
//
bool Frame3D::PtInside(const Point3D& p) const {
	bool res = false;
	if (p.mX >= mMin.mX && p.mX <= mMax.mX) {
		if (p.mY >= mMin.mY && p.mY <= mMax.mY) {
			if (p.mZ >= mMin.mZ && p.mZ <= mMax.mZ) {
				res = true;
			}
		}
	}
	return res;
}
//
//	Manipulators
//	These alter the coordinates of the box and return 
//	a pointer to the altered box.
//	First a rather nasty set that directly alter the
//	individual corner coordinates. They do make sure
//	that the operations do not violate the basic
//	constraints of a box and only make their alterations
//	if the result would be valid. If the operation fails
//	then the method returns nullptr.
//
Frame3D* Frame3D::SetXMin(Real t) {
  if ((isEmpty) || (t < mMin.mX)) {
		mMin.mX = t;
    isEmpty = false;
		return this;
	}
	return nullptr;
}

Frame3D* Frame3D::SetXMax(Real t) {
	if ((isEmpty) || (t > mMax.mX)) {
		mMax.mX = t;
    isEmpty = false;
		return this;
	}
	return nullptr;
}

Frame3D* Frame3D::SetYMin(Real t) {
	if ((isEmpty) || (t < mMin.mY)) {
		mMin.mY = t;
    isEmpty = false;
		return this;
	}
	return nullptr;
}

Frame3D* Frame3D::SetYMax(Real t) {
	if ((isEmpty) || (t > mMax.mY)) {
		mMax.mY = t;
    isEmpty = false;
		return this;
	}
	return nullptr;
}

Frame3D* Frame3D::SetZMin(Real t) {
	if ((isEmpty) || (t < mMin.mZ)) {
		mMin.mZ = t;
    isEmpty = false;
		return this;
	}
	return nullptr;
}

Frame3D* Frame3D::SetZMax(Real t) {
	if ((isEmpty) || (t > mMax.mZ)) {
		mMax.mZ = t;
    isEmpty = false;
		return this;
	}
	return nullptr;
}

//
//	Then ones that act by constructive geometry.
//
Frame3D* Frame3D::AddPoint(const Point3D* p) {
  if (isEmpty) {
    mMin = mMax = *p;
    isEmpty = false;
    return this;
  }
	if (p->mX < mMin.mX) {
		mMin.mX = p->mX;
	} else if (p->mX > mMax.mX) {
		mMax.mX = p->mX;
	}
	if (p->mY < mMin.mY) {
		mMin.mY = p->mY;
	} else if (p->mY > mMax.mY) {
		mMax.mY = p->mY;
	}
	if (p->mZ < mMin.mZ) {
		mMin.mZ = p->mZ;
	} else if (p->mZ > mMax.mZ) {
		mMax.mZ = p->mZ;
	}
	return this;
}
Frame3D* Frame3D::AddPoint(const Point3D& p) {
  if (isEmpty) {
    mMin = mMax = p;
    isEmpty = false;
    return this;
  }
	if (p.mX < mMin.mX) {
		mMin.mX = p.mX;
	} else if (p.mX > mMax.mX) {
		mMax.mX = p.mX;
	}
	if (p.mY < mMin.mY) {
		mMin.mY = p.mY;
	} else if (p.mY > mMax.mY) {
		mMax.mY = p.mY;
	}
	if (p.mZ < mMin.mZ) {
		mMin.mZ = p.mZ;
	} else if (p.mZ > mMax.mZ) {
		mMax.mZ = p.mZ;
	}
	return this;
}
Frame3D* Frame3D::AddPoint3dv(const double* v) {
  if (isEmpty) {
    mMin.mX = mMax.mX = v[0];
    mMin.mY = mMax.mY = v[0];
    mMin.mZ = mMax.mZ = v[0];
    isEmpty = false;
    return this;
  }
	if (v[0] < mMin.mX) {
		mMin.mX = (float) v[0];
	} else if (v[0] > mMax.mX) {
		mMax.mX = (float) v[0];
	}
	if (v[1] < mMin.mY) {
		mMin.mY = (float) v[1];
	} else if (v[1] > mMax.mY) {
		mMax.mY = (float) v[1];
	}
	if (v[2] < mMin.mZ) {
		mMin.mZ = (float) v[2];
	} else if (v[2] > mMax.mZ) {
		mMax.mZ = (float) v[2];
	}
	return this;
}
Frame3D* Frame3D::AddPoint3fv(const float* v) {
  if (isEmpty) {
    mMin.mX = mMax.mX = v[0];
    mMin.mY = mMax.mY = v[0];
    mMin.mZ = mMax.mZ = v[0];
    isEmpty = false;
    return this;
  }
	if (v[0] < mMin.mX) {
		mMin.mX = (float) v[0];
	} else if (v[0] > mMax.mX) {
		mMax.mX = (float) v[0];
	}
	if (v[1] < mMin.mY) {
		mMin.mY = (float) v[1];
	} else if (v[1] > mMax.mY) {
		mMax.mY = (float) v[1];
	}
	if (v[2] < mMin.mZ) {
		mMin.mZ = (float) v[2];
	} else if (v[2] > mMax.mZ) {
		mMax.mZ = (float) v[2];
	}
	return this;
}

Frame3D* Frame3D::AddFrame(const Frame3D* b) {
	if (b->mMin.mX < mMin.mX) {
		mMin.mX = b->mMin.mX;
	}
	if (b->mMax.mX > mMax.mX) {
		mMax.mX = b->mMax.mX;
	}
	if (b->mMin.mY < mMin.mY) {
		mMin.mY = b->mMin.mY;
	}
	if (b->mMax.mY > mMax.mY) {
		mMax.mY = b->mMax.mY;
	}
	if (b->mMin.mZ < mMin.mZ) {
		mMin.mZ = b->mMin.mZ;
	}
	if (b->mMax.mZ > mMax.mZ) {
		mMax.mZ = b->mMax.mZ;
	}
	return this;
}
//
//  This one gets the radius of a sphere big enough to
//  encompass the whole box. This can be computed either
//  based on the center of the box or on the origin of
//  coordinates (the default).
//
Real Frame3D::Radius(bool OriginCentered) const
{
  Real radius = 0.0;
  if (OriginCentered) {
    Point3D origin(0.0, 0.0, 0.0);
    Real d1 = origin.DistanceTo(mMin);
    Real d2 = origin.DistanceTo(mMax);
    radius = (d1 > d2) ? d1 : d2;
  } else {
    radius = 0.5 * mMin.DistanceTo(mMax);
  }
  return radius;
}




