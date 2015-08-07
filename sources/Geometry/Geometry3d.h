/*
 *	Geometry3D.h
 *
 *	Here live mathematical objects, Color, Point, and Vector,
 *	that have no visible form. They are used for various operations and
 *	as instance variables of the GeometricObject classes but are not
 *	themselves drawable objects.
 *
 *  Brian Collett 2/2014 Significantly upgraded for FieldViewer.
 *  Added Rect3D class and added idea of comparing various objects to
 *  some tolerance. Each class has its own idea of what it means for two
 *  objects of that class to be equal. This does not usually mean that the
 *  internal values are EXACTLY the same. Because we store all coordinates
 *  as floating point numbers that are necessarily imprecise the classes
 *  have an idea of tolerance for equality. The specific details vary from
 *  class to class but all provide class variables to store the parameters
 *  that control the degree of equality and these are made public so that
 *  the user can choose their own idea of how precise equality is. The
 *  default is 10mRad (about 0.5 degreee) for angles and 1% for lengths.
 *	Brian Collett 7/7/2010 revised from 3/15/2000
 *  Brian Collett 3/18/11 Add equality operators for vectors and points.
 */
#ifndef _Geometry3D_h
#define _Geometry3D_h

#include <cmath>

#ifdef UseOpenGL

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <gl/gl.h>
#include <gl/glu.h>
#include "DisplayList.h"
#endif

#else
#include <stdlib.h>
#define GLfloat float
#endif

typedef double Real;
/*
 *	An RGB color is a useful tool for passing around OpenGL style colors
 *	represented as three floating numbers between 0.0f and 1.0f.
 *	NOTE that the instance variables are public!
 */
class RGBColour {
public:
	union {
		struct {
			GLfloat red;
			GLfloat green;
			GLfloat blue;
		} _c;
		GLfloat _comps[3];
	} _m;
	//
	//	Constructor, defaults to white.
	//	Note support for copying and assignment.
	//
  RGBColour(double r, double g, double b);
  RGBColour(GLfloat r = 1.0, GLfloat g = 1.0, GLfloat b = 1.0);
	RGBColour(const RGBColour&);
	RGBColour& operator=(const RGBColour&);
#ifdef UseOpenGL
  //
  //  So can use as a geometric object, provide Draw.
  //
  void Draw(void);
#endif
	//
	//	Alter value.
	//
	RGBColour& Set(GLfloat r, GLfloat g, GLfloat b);
};
//
//	Accessor macros for the components.
//
#define mRed _m._c.red
#define mGreen _m._c.green
#define mBlue _m._c.blue
#define mComponents _m._comps

class Vector3D;
/*
 *	A point is a mathematical object used to do arithmetic on points
 *	in 3D space.
 */
class Point3D {
public:
  //
  //  Class vars.
  //  How close must two coords be to be treated as identical. Test is
  //  a-b < cAbsDiff + cRelDiff * a.
  //
  static Real cAbsDiff;
  static Real cRelDiff;
	//
	//	Instance Vars.
	//
	union {
		struct {
			Real _x;
			Real _y;
			Real _z;
		} _c;
		Real _coords[3];
	} _m;
	//
	//	Construct/Destruct
	//
	Point3D(Real nx = 0.0, Real ny = 0.0, Real nz = 0.0);
  Point3D(const float* v);
  Point3D(const double* v);
  virtual ~Point3D() {};
	//
	//	Need copy constructor and assignment op.
	Point3D(const Point3D& oPt);
	Point3D& operator=(const Point3D&);
	//
	//	Operations on points.
	//
	//	Set can reset the components of a point.
	//
	Point3D& Set(Real nx, Real ny, Real nz);
	Point3D& Set(const float* parray);
	Point3D& Set(const double* parray);
	//
	//	Can subtract two points to get a vector.
	//	Can add or subtract a vector to a point giving a point.
	//	These additions can make new points or alter the current
	//	point
	//
	Vector3D operator-(const Point3D&) const;
	Point3D operator+(const Vector3D&) const;
	Point3D& operator+=(const Vector3D&);
	Point3D operator-(const Vector3D&) const;
	Point3D& operator-=(const Vector3D&);
  //
  //  Can see if two points are the same.
  //
  bool operator==(const Point3D&) const;
	//
	//	Can compute the distance to some other point.
	//	Second version skips the square root to save time.
	//
	Real DistanceTo(const Point3D&) const;
	Real DistSquaredTo(const Point3D&) const;
};
/*
 *	Similarly, a Vector is used for mathematical operations.
 */
class Vector3D {
public:
  //
  //  Class vars control equality comparison. Comparison is by angle and
  //  direction.
  //
  static Real cMagRelDiff;
  static Real cAngDiff;
	//
	//	Instance Vars.
	//
	union {
		struct {
			Real _x;
			Real _y;
			Real _z;
		} _c;
		Real _coords[3];
	} _m;
  //
  //  Cache length because it is so useful.
  //
  Real mLength;
	//
	//	Construct/Destruct
	//
	Vector3D(Real nx = 0.0, Real ny = 0.0, Real nz = 0.0);
  Vector3D(float* v);
  Vector3D(double* v);
	//
	//	Need copy constructor and assignment op.
	Vector3D(const Vector3D& oPt);
	Vector3D& operator=(const Vector3D&);
  virtual ~Vector3D() {};
	//
	//	Operations on Vectors.
	//	Can find length of vector and its spherical
	//	angular components.
	//
	Real Length() const;
	Real LengthSq() const;
	Real AngTheta() const;
	Real AngPhi() const;
	//
	//	Set can reset the components of a vector.
	//
	Vector3D& Set(Real nx, Real ny, Real nz);
	//
	//	Can add or subtract a vector to a vector giving a vector.
	//	Each has both a form creating a new vector and a form
	//	to alter this vector.
	//
	Vector3D operator+(const Vector3D&) const;
	Vector3D& operator+=(const Vector3D&);
	Vector3D operator-(const Vector3D&) const;
	Vector3D& operator-=(const Vector3D&);
	//
	//	Can multiply or divide a vector by a scalar to get a new
	//	vector.
	//
	Vector3D operator*(Real) const;
	Vector3D& operator*=(Real);
	Vector3D operator/(Real) const;
	Vector3D& operator/=(Real);
  //
  //  Can see if two Vectors are the same.
  //
  bool operator==(Vector3D&) const;
	//
	//	Dotting two vectors gives a scalar. Crossing them gives
	//	a new vector.
	//
	Real Dot(const Vector3D&) const;
	Vector3D Cross(const Vector3D&) const;
	//
	//	Normalize sets the vector length to 1.
	//
	Vector3D Normalize();
  Vector3D Norm() const;
};
//
//	Accessor macros for Point and Vector components.
//
#define mX _m._c._x 
#define mY _m._c._y 
#define mZ _m._c._z 
#define mCoords _m._coords
/*
 *  Rect3D
 *
 *  A Rect3D is a generally oriented rectangular surface patch. It
 *  is stored as a set of four corners but it goes to some trouble
 *  to guarantee that those corners represent a true planar rectangle.
 *  Because of this it is important to check that the Rect3D is valid
 *  before doing much of anything with it.
 *  At the moment (2/14) I do not define comparison for Rects but we
 *  do define an angle tolerance for checking that our sides are
 *  perpendicular. The test is actually fabs(left.dot.right) < mCAngTol.
 */
class Rect3D {
public:
  static Real cCAngTol;
protected:
  //
  //  We are not valid unless our corners do define a rectangular region
  //  of a plane.
  //
  bool mValid;
  //
  //  From our point of view we think of the corners as
  //  0 top left
  //  1 top right
  //  2 bottom right
  //  3 bottom left
  //
  Point3D mCorners[4];
  //
  //  It may help to cache the vectors that describe the two kinds of side.
  //
  Vector3D mHoriz;
  Vector3D mVert;
public:
  //
  //  ctors
  //
  Rect3D(const Point3D* corners);
  Rect3D(const Point3D& topLeft, const Point3D& topRight,
         const Point3D& bottomRight, const Point3D& bottomLeft);
  virtual ~Rect3D() {};
  //
  //  Copy constructor and assignment operator.
  //
  Rect3D(const Rect3D* fp) : Rect3D(*fp) {};
  Rect3D(const Rect3D& f);
  Rect3D& operator=(const Rect3D& f);
  //
  //  Accessors
  //
  bool IsValid() const { return mValid; }
  const Point3D& TopLeft() const { return mCorners[0]; };
  const Point3D& TopRight() const { return mCorners[1]; };
  const Point3D& BottomRight() const { return mCorners[2]; };
  const Point3D& BottomLeft() const { return mCorners[3]; };
  Vector3D GetNormal() const;
  Vector3D GetHorizontal() const { return mHoriz; };
  Vector3D GetVertical() const { return mVert; };
  double GetWidth() const { return mHoriz.Length(); };
  double GetHeight() const { return mVert.Length(); };
  //
  //  This allows us to index into the rect as if it were a 2D surface
  //  runing from 0->Width in one direction and 0->height in the other.
  //
  Point3D Map2D(double x, double y);
protected:
  //
  //  Internal helpers.
  //  This checks that the four corners define a planar rectangle.
  //
  bool Validate();
};
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
 *  At the moment (2/14) I do not define comparison for Frames.
 */
class Frame3D {
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
	Point3D mMin;
	Point3D mMax;
  //
  //  We also have a flag that allows us to represent a completely
  //  empty Frame, something without even one point to call its own.
  //  Note that a Frame with one point is NOT empty.
  //
  bool isEmpty;
public:
	//
	//	Constructors and destructor.
	//	Default constructor creates an empty box centred on
	//	the origin. Alternate versions allow you to specify an
	//	alternate location for the box and even an initial size.
	//
	Frame3D(const Point3D* startPoint = nullptr);
  Frame3D(const Point3D& p);
  Frame3D(const Point3D& start, const Point3D& end);
	Frame3D(const Point3D* startPoint, const Point3D* endPoint);
	~Frame3D();
  //
  //  Copy constructor and assignment operator.
  //
  Frame3D(const Frame3D* fp) : Frame3D(*fp) {};
  Frame3D(const Frame3D& f);
  Frame3D& operator=(const Frame3D& f);
	//
	//	Initialisers allow you to start with points or other
	//	frames at any time, not only construction.
	//
	void SetMin(const Point3D& startPoint);
	void SetMin3dv(double* vals);
	void SetMax(const Point3D& startPoint);
	void SetMax3dv(double* vals);
	void Set(const Point3D& min, const Point3D& max);
  void Set(const Frame3D& frame);
  void Clear(void);
	//
	//	Accessors.
	//
  bool IsEmpty(void) const { return isEmpty; };
	//
	//	This tells us whether a point is in the box.
	//
	bool PtInside(const Point3D&) const;
  //
	//	First a set to get at the dimensions of the box. Can't
	//	call them lenth, width, etc. because you never know which
	//	one is which. I call them spans.
	//
	Real XSpan() const { return mMax.mX - mMin.mX; };
	Real YSpan() const { return mMax.mY - mMin.mY; };
	Real ZSpan() const { return mMax.mZ - mMin.mZ; };
	//
	//	Then ones for the individual coordinates.
	//
	Real XMin() const { return mMin.mX; };
	Real XMax() const { return mMax.mX; };
	Real YMin() const { return mMin.mY; };
	Real YMax() const { return mMax.mY; };
	Real ZMin() const { return mMin.mZ; };
	Real ZMax() const { return mMax.mZ; };
  //
  //  And for the centre of the box.
  //
	Real XMid() const { return 0.5 * (mMin.mX + mMax.mX); };
	Real YMid() const { return 0.5 * (mMin.mY + mMax.mY); };
	Real ZMid() const { return 0.5 * (mMin.mZ + mMax.mZ); };
  //
  //  For the bounding points and centre.
  //
  const Point3D& GetMin() const { return mMin; };
  const Point3D& GetMax() const { return mMax; };
  const Point3D& GetMid() const;
  //
  //  This one gets the radius of a sphere big enough to
  //  encompass the whole box. This can be computed either
  //  based on the center of the box or on the origin of
  //  coordinates (the default).
  //
  Real Radius(bool OriginCentered = true) const;
	//
	//	Manipulators
	//	These alter the coordinates of the box and return 
	//	a pointer to the altered box.
	//	First a rather nasty set that directly alter the
	//	individual corner coordinates.
	//
	Frame3D* SetXMin(Real);
	Frame3D* SetXMax(Real);
	Frame3D* SetYMin(Real);
	Frame3D* SetYMax(Real);
	Frame3D* SetZMin(Real);
	Frame3D* SetZMax(Real);
	//
	//	Then ones that act by constructive geometry.
	//
	Frame3D* AddPoint(const Point3D*);
	Frame3D* AddPoint(const Point3D&);
	Frame3D* AddPoint3dv(const double*);
	Frame3D* AddPoint3fv(const float*);
	Frame3D* AddFrame(const Frame3D*);
protected:
	//
	//	Internal Helpers.
	//
};

#endif

