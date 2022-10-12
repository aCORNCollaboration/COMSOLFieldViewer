/*
*	Geometry2D.h
*
*	This is the home of a set of 2-D floating point
*	geometrical objects. Here are points, vectors,
*	and rectangles oriented parallel to the coordinate
*	axes.
*
*	Brian Collett 10/1/2003 based on GeomtryHelpers.
*/
#include "assert.h"
#include <math.h>
#include "Geometry2D.h"

/*
*	Points support a few operations.
*/
//
//	Construct/Destruct
//
Point2D::Point2D(Real nx, Real ny) {
	x = nx;
	y = ny;
}

//
//	Need copy constructor and assignment op.
//
Point2D::Point2D(const Point2D& oPt) {
	x = oPt.x;
	y = oPt.y;
}

Point2D& Point2D::operator=(const Point2D& oPt) {
	x = oPt.x;
	y = oPt.y;
	return *this;
}

//
//	Operations on points.
//
//	Set can reset the components.
//
Point2D& Point2D::Set(Real nx, Real ny) {
	x = nx;
	y = ny;
	return *this;
}
Point2D& Point2D::Set(const float* const parray) {
	x = parray[0];
	y = parray[1];
	return *this;
}
Point2D& Point2D::Set(const double* const parray) {
	x = (Real) parray[0];
	y = (Real) parray[1];
	return *this;
}

//
//	Can subtract two points to get a vector.
//	Can add or subtract a vector to a point giving a point.
//
Vector2D Point2D::operator-(const Point2D& oPt) const 
{
	return Vector2D(x - oPt.x, y - oPt.y);
}
/*
*	Can add a vector to a point to get a new point.
*/
Point2D Point2D::operator+(const Vector2D& v) const
{
	return Point2D(x + v.x, y + v.y);
}
/*
*	Or to modify the current point.
*/
Point2D& Point2D::operator+=(const Vector2D& v) 
{
	x += v.x;
	y += v.y;
	return *this;
}
/*
*	Likewise for subtracting a vector from a point.
*/
Point2D Point2D::operator-(const Vector2D& v) const
{
	return Point2D(x - v.x, y - v.y);
}
Point2D& Point2D::operator-=(const Vector2D& v) 
{
	x -= v.x;
	y -= v.y;
	return *this;
}

//
//	Can compute the distance to some other point.
//
Real Point2D::DistanceTo(const Point2D& p) const
{
	return (Real) sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
}

Real Point2D::DistSquaredTo(const Point2D& p) const
{
	return ((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
}
/*
*	Vector2D in an extension that supports rather more operations.
*/
/*
Vector2D& Vector2D::operator=(Vector2D& v) {
	x = v.x;
	y = v.y;
	return *this;
}
*/
//
//	Operations on Vectors.
//	Can find length of vector and its polar
//	angle.
//
Real Vector2D::Length() const
{
	return (Real) sqrt(x*x + y*y);
}

Real Vector2D::LengthSq() const
{
    return (x*x + y*y);
}
/*
*   These are  NOT as unsafe as they look because atan2 correctly 
*   handles the case where its second argument is 0.
*/
Real Vector2D::AngTheta() const
{
    return Real(atan2(y, x));
}

//
//	Can add or subtract a vector to a vector giving a vector.
//	Each has both a form creating a new vector and a form
//	to alter this vector.
//
Vector2D Vector2D::operator+(const Vector2D& v) const
{
	return Vector2D(x + v.x, y + v.y);
}

Vector2D& Vector2D::operator+=(const Vector2D& v) 
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2D Vector2D::operator-(const Vector2D& v) const
{
	return Vector2D(x - v.x, y - v.y);
}

Vector2D& Vector2D::operator-=(const Vector2D& v) 
{
	x -= v.x;
	y -= v.y;
	return *this;
}

//
//	Can multiply or divide a vector by a scalar to get a new
//	vector.
//
Vector2D Vector2D::operator*(Real s) const
{
	return Vector2D(x * s, y * s);
}

Vector2D& Vector2D::operator*=(Real s) {
	x *= s;
	y *= s;
	return *this;
}

Vector2D Vector2D::operator/(Real s) const
{
	return Vector2D(x / s, y / s);
}

Vector2D& Vector2D::operator/=(Real s) {
	x /= s;
	y /= s;
	return *this;
}

//
//	Dotting two vectors gives a scalar. Crossing them gives
//	a new vector.
//
Real Vector2D::Dot(const Vector2D& v) const
{
	return (x * v.x + y * v.y);
}
//
//	2-D vectors don't have a real cross product so this
//	returns the z component of the 3-D cross product
//	and so tells us about the winding sense going from
//	one vector to the other.
//
Real Vector2D::Winding(const Vector2D& v2) const 
{
	return x*v2.y-y*v2.x;
}
/*
Vector2D& Vector2D::Cross(Vector2D& v) {
	static Vector2D result;
	result.Set(y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x);
	return result;
}
*/
//
//	Normalize sets the vector length to 1.
//
Vector2D& Vector2D::Normalize()
{
	Real length = Length();
	x /= length;
	y /= length;
	return *this;
}
/*
*	Frame2D
*
*	This is a class that encapsulates the idea of a rectangular
*	parallelipiped (a box) oriented parallel to the coordinate
*	axes. Its chief purpose is to describe the area bounded
*	by a 2-D model.
*	A Frame2D supports a number of access methods that know practically
*	anything you could want to know about a Frame2D. It also supports
*	operations such as adding a point to the Frame2D (the Frame makes sure
*	that it is big enough to contain the point), unioning two Frame2D's
*	(giving a Frame2D that encompasses all of both), intersecting two
*	Frame2D's, etc. It is NOT a drawable GeometryObject.
*/
//
//	Constructors and destructor.
//	Default constructor creates an empty box centred on
//	the origin. Alternate versions allow you to specify an
//	alternate location for the box and even an initial size.
//
Frame2D::Frame2D(const Point2D* const startPoint) 
{
	if (startPoint != NULL) {
		mMin.x = mMax.x = startPoint->x;
		mMin.y = mMax.y = startPoint->y;
		mValid = true;
	} else {
		mMin.x = mMax.x = 0.0;
		mMin.y = mMax.y = 0.0;
		mValid = false;
	}
}
//
//	Alternate constructor builds smallest box that encompasses
//	two given points. It does not assume that the two points are
//	sorted in any way.
//
Frame2D::Frame2D(const Point2D* const startPoint, const Point2D* const endPoint) 
{
	if (startPoint->x <= endPoint->x) {
		mMin.x = startPoint->x;
		mMax.x = endPoint->x;
	} else {
		mMin.x = endPoint->x;
		mMax.x = startPoint->x;
	}
	if (startPoint->y <= endPoint->y) {
		mMin.y = startPoint->y;
		mMax.y = endPoint->y;
	} else {
		mMin.y = endPoint->y;
		mMax.y = startPoint->y;
	}
	mValid = true;
}
Frame2D::Frame2D(const Point2D& startPoint, const Point2D& endPoint) 
{
	if (startPoint.x <= endPoint.x) {
		mMin.x = startPoint.x;
		mMax.x = endPoint.x;
	} else {
		mMin.x = endPoint.x;
		mMax.x = startPoint.x;
	}
	if (startPoint.y <= endPoint.y) {
		mMin.y = startPoint.y;
		mMax.y = endPoint.y;
	} else {
		mMin.y = endPoint.y;
		mMax.y = startPoint.y;
	}
	mValid = true;
}
//
//	Alter a frame (very useful when constructed by default).
//
Frame2D& Frame2D::Set(Real xLeft, Real yBottom, Real xRight, Real yTop) 
{
	assert(xLeft <= xRight);
	assert(yBottom <= yTop);
	mMin.x = xLeft;
	mMin.y = yBottom;
	mMax.x = xRight;
	mMax.y = yTop;
	mValid = true;
	return *this;
}
//
//	Find out if the box is empty.
//
bool Frame2D::IsEmpty() const
{
	if (mValid) {
		if (fabs(mMax.x - mMin.x) < 1.0e-10) {
			return true;
		}
		if (fabs(mMax.y - mMin.y) < 1.0e-10) {
			return true;
		}
		return false;
	} else {		// Uninited box must be empty
		return true;
	}
}
//
//	Then ones that act by constructive geometry.
//
Frame2D* Frame2D::AddPoint(const Point2D& p) 
{
	if (mValid) {
		if (p.x < mMin.x) {
			mMin.x = p.x;
		} else if (p.x > mMax.x) {
			mMax.x = p.x;
		}
		if (p.y < mMin.y) {
			mMin.y = p.y;
		} else if (p.x > mMax.y) {
			mMax.y = p.y;
		}
	} else {
		mMax = mMin = p;
		mValid = true;
	}
	return this;
}

Frame2D* Frame2D::AddPoint(Real px, Real py) 
{
	if (mValid) {
		if (px < mMin.x) {
			mMin.x = px;
		} else if (px > mMax.x) {
			mMax.x = px;
		}
		if (py < mMin.y) {
			mMin.y = py;
		} else if (py > mMax.y) {
			mMax.y = py;
		}
	} else {
		mMax.x = mMin.x = px;
		mMax.y = mMin.y = py;
		mValid = true;
	}
	return this;
}

Frame2D* Frame2D::AddBox(const Frame2D& f) 
{
	if (mValid) {
		if (f.mMin.x < mMin.x) {
			mMin.x = f.mMin.x;
		}
		if (f.mMax.x > mMax.x) {
			mMax.x = f.mMax.x;
		}
		if (f.mMin.y < mMin.y) {
			mMin.y = f.mMin.y;
		}
		if (f.mMax.y > mMax.y) {
			mMax.y = f.mMax.y;
		}
	} else {
		mMin = f.mMin;
		mMax = f.mMax;
		mValid = true;
	}
	return this;
}
//
//	This tells us if two Frames intersect.
//
bool Frame2D::Intersect(const Frame2D& f) const
{
	//
	//	Empty frames cannot overlap.
	//
	if (IsEmpty() || f.IsEmpty()) {
		return false;
	}
	//
	//	Frames intersect if and only if there is overlap
	//	in x and in y so do separately.
	//	Two line segments overlap if any end-point of one
	//	lies within the other.
	//
	if (((f.mMin.x >= mMin.x) && (f.mMin.x <= mMax.x)) ||
		((f.mMax.x >= mMin.x) && (f.mMax.x <= mMax.x)) ||
		((mMin.x >= f.mMin.x) && (mMin.x <= f.mMax.x)) ||
		((mMax.x >= f.mMin.x) && (mMax.x <= f.mMax.x))) {
		if (((f.mMin.y >= mMin.y) && (f.mMin.y <= mMax.y)) ||
			((f.mMax.y >= mMin.y) && (f.mMax.y <= mMax.y)) ||
			((mMin.y >= f.mMin.y) && (mMin.y <= f.mMax.y)) ||
			((mMax.y >= f.mMin.y) && (mMax.y <= f.mMax.y))) {
				return true;
		}
	}
	return false;
}





