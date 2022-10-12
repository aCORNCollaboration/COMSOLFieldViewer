/*
*	Geometry2D.h
*
*	This is the home of a set of 2-D floating point
*	geometrical objects. Here are points, vectors,
*	and rectangles oriented parallel to the coordinate
*	axes.
*	I am adding a 2-D array with range chaecking that
*	can multiply a suitable vector.
*
*	Brian Collett 10/1/2003 based on GeomtryHelpers.
*/
#ifndef Geometry2D_h
#define Geometry2D_h

#include <stdio.h>
//#include "Real.h"		// Defines the Real type
#include "Geometry3d.h"

class Vector2D;			// Forward ref for Point2D.
/*
*	A point is a mathematical object used to do arithmetic on points
*	in 2D space.
*/
class Point2D {
	//
	//	Instance Vars.
	//
public:
	Real x;
	Real y;
	//
	//	Construct/Destruct
	//
	Point2D(Real nx = 0.0, Real ny = 0.0);
	//
	//	Need copy constructor and assignment op.
	Point2D(const Point2D& oPt);
	Point2D& operator=(const Point2D&);
  virtual ~Point2D() {};
	//
	//	Operations on points.
	//
	//	Set can reset the components of a point.
	//	NOTE that both float* arrays and double*
	//	arrays are supported as initialisers regardless
	//	of the setting of Real.
	//
	Point2D& Set(Real nx, Real ny);
	Point2D& Set(const float* const parray);
	Point2D& Set(const double* const parray);
	//
	//	Can subtract two points to get a vector.
	//	Can add or subtract a vector to a point giving a point.
	//	These additions can make new points or alter the current
	//	point
	//
	Vector2D operator-(const Point2D&) const;
	Point2D operator+(const Vector2D&) const;
	Point2D& operator+=(const Vector2D&);
	Point2D operator-(const Vector2D&) const;
	Point2D& operator-=(const Vector2D&);
	//
	//	Can compute the distance to some other point.
	//	Second version skips the square root to save time.
	//
	Real DistanceTo( const Point2D&) const;
	Real DistSquaredTo( const Point2D&) const;
};
/*
*	Similarly, a Vector is an extension of a point that
*	supports more operations to act as a vector.
*/
class Vector2D : public Point2D {
public:
	//
	//	Construct/Destruct
	//
	Vector2D(Real nx = 0.0, Real ny = 0.0) : Point2D(nx, ny) {};
	Vector2D(const Point2D& oPt) : Point2D(oPt) {};
	Vector2D(const Vector2D& oVec) : Point2D(oVec) {};
//	Vector2D& operator=(Vector2D&);
  virtual ~Vector2D() {};
	//
	//	Operations on Vectors.
	//	Can find length of vector and its polar
	//	angle.
	//
	Real Length() const;
	Real LengthSq() const;
	Real AngTheta() const;
	Real AngPhi() const;
	//
	//	Can add or subtract a vector to a vector giving a vector.
	//	Each has both a form creating a new vector and a form
	//	to alter this vector.
	//
	Vector2D operator+( const Vector2D&) const;
	Vector2D& operator+=( const Vector2D&);
	Vector2D operator-( const Vector2D&) const;
	Vector2D& operator-=( const Vector2D&);
	//
	//	Can multiply or divide a vector by a scalar to get a new
	//	vector.
	//
	Vector2D operator*(Real) const;
	Vector2D& operator*=(Real);
	Vector2D operator/(Real) const;
	Vector2D& operator/=(Real);
	//
	//	Dotting two vectors gives a scalar. 
	//	Crossing them would have to produce	a Vector3D.
	//
	Real Dot( const Vector2D&) const;
//
//	2-D vectors don't have a real cross product so this
//	returns the z component of the 3-D cross product
//	and so tells us about the winding sense going from
//	one vector to the other.
//
	Real Winding(const Vector2D&) const;
//	Vector3D& Cross(Vector2D&);
	//
	//	Normalize sets the vector length to 1.
	//
	Vector2D& Normalize();
};
/*
*	Frame2D
*
*	This is a class that encapsulates the idea of a rectangle
*	oriented parallel to the coordinate axes.
*	A Frame2D supports a number of access methods that know practically
*	anything you could want to know about a Frame2D. It also supports
*	operations such as adding a point to the Frame2D (the Frame makes sure
*	that it is big enough to contain the point), unioning two Frame2D's
*	(giving a Frame2D that encompasses all of both), intersecting two
*	Frame2D's, etc. It is NOT a drawable GeometryObject.
*/
class Frame2D {
protected:
	//
	//  Instance variables.
	//	Internally I choose to represent a box by two corners
	//	We use the minimal and maximal corner, the ones with
	//	the lowest and highest values of all three coordinates.
	//	Accessors hide the representation so that the internal
	//	should be easy to change without any change to the
	//	interface.
	//
public:
	Point2D mMin;
	Point2D mMax;
	bool mValid;		// True when bounds have been set
	//
	//	Constructors.
	//	Default constructor creates an empty box centred on
	//	the origin. Alternate versions allow you to specify an
	//	alternate location for the box and even an initial size.
	//	NOTE that a class with no storage does not need a
	//	destructor.
	//
	Frame2D(const Point2D* const startPoint = NULL);
	Frame2D(const Point2D* const startPoint, const Point2D* const endPoint);
	Frame2D(const Point2D& startPoint, const Point2D& endPoint);
	//
	//	Alter a frame (very useful when constructed by default).
	//
	Frame2D& Set(Real xLeft, Real yBottom, Real xRight, Real yTop);
	//
	//	Accessors.
	//
	//	First a set to get at the dimensions of the box. Can't
	//	call them lenth, width, etc. because you never know which
	//	one is which. I call them spans.
	//
	Real XSpan() const { return mMax.x - mMin.x; };
	Real YSpan() const { return mMax.y - mMin.y; };
	//
	//	Find out if the box is empty.
	//
	bool IsEmpty() const;
	//
	//	Then ones for the individual coordinates.
	//	NOTE these are not really needed as the values
	//	are public but it gives an alternate view of
	//	them as 4 vars rather than 2 points.
	//	By making them return refs these can be use
	//	as lvals or rvals.
	//
	Real& XMin() { return mMin.x; };
	Real& XMax() { return mMax.x; };
	Real& YMin() { return mMin.y; };
	Real& YMax() { return mMax.y; };
	//
	//	Manipulators
	//
	//	Then ones that act by constructive geometry.
	//
	Frame2D* AddPoint(const Point2D& p);
	Frame2D* AddPoint(const Real x, Real y);
	Frame2D* AddBox(const Frame2D& f);
	//
	//	This tells us if two Frames intersect.
	//
	bool Intersect(const Frame2D& f) const;
protected:
	//
	//	Internal Helpers.
	//
};

#endif


