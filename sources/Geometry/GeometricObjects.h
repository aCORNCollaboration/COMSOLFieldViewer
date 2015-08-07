/*
 *	GeometricObjects.h
 *
 *	This is a set of classes that describe geometric objects. These are
 *	things that represent potentially visible pieces of geometry such
 *	as vertices and lines as distinct from the underlying mathematical
 *	objects such as points and vectors.
 *	These can be used in non-graphical problems without significant overhead
 *	but can be upgraded to full graphical support by defining the pre-
 *	processor symbol UseOpenGL.
 *	GeometricObjects use reference counting to minimise copying and
 *	optionally (through preprocessor symbol DebugGeom) provide tagging
 *	to uniquely indentify the nature of a geometric object.
 *	The geometric classes are built on top of the purely mathematical
 *	Geometry classes.
 *
 *	BCollett 7/7/2010 Reworked from SimulatorSRC/GeometryObjects.
 */
#ifndef GeometryObjects_h
#define GeometryObjects_h

#include <stdio.h>

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

#endif

#ifdef _WIN32
#include <CrtDbg.h>
#endif

#include "Geometry3d.h"

#define DebugGeom 1
extern const char* GLErrors[6];
/*
 *   Macro to add error handling to any gl call.
 */
#define Call(x) do { GLenum CallErr  = GL_NO_ERROR; \
x; \
CallErr = glGetError();\
if (CallErr != GL_NO_ERROR) {\
fprintf(stderr,"%s(%d): OpenGL %s error\n",\
__FILE__,\
__LINE__,\
GLErrors[CallErr - GL_INVALID_ENUM]);\
}\
} while (0);

/*
*	Have an inheritance hierarchy of geometry objects to support
*	building geometries into groups.
*/
enum GeomCode {
	kGeomNull = 0,
	kGeomColor,
	kGeomPoint,
	kGeomVector,
	kGeomVertex,
	kGeomASet,
	kGeomLine,
	kGeomPolyLine,
	kGeomBox,
	kGeomGBox,
	kGeomTriangle,
	kGeomEllipse,
	kGeomTube,
	kGeomCap,
	kGeomGroup,
  kGeomView,
  kGeomRect,
	kGeomError };
//
//	Attribute flags control the appearance of geometry objects.
//
#define kGFNone		0L
#define kGFWire		0x01L	// Render as wire frame
#define kGFSolid	0x02L	// Render as solid

class RGBColour;

/******************************************************************/
//
//  Basic GeometricObject classes; DisplayList, Object, and Group
//
/******************************************************************/

#ifdef UseOpenGL
/*
 *	GeometricObjects use DisplayLists to store their visible forms
 */
typedef enum {
	kDLCompile = GL_COMPILE,
	kDLCompAndExec = GL_COMPILE_AND_EXECUTE
} DisplayListMode;

class CDisplayList {
protected:
	//
	//	Instance variables.
	//	The ListHandle is the small integer that OpenGL
	//	uses to track the display list.
	//
	int mListHandle;
	//
	//	mMode controls whether the list is executed as it
	//	is built or merely compiled for later use.
	//	Default is compile only.
	//
	DisplayListMode mMode;
	//
	//	Keep a bounding box to track the space occupied by
	//	the DisplayList.
	//
	Frame3D* mBounds;
public:
	//
	//	Constructor sets up the link to OpenGL.
	//	Its argument is a mode flag that determines
	//	whether the display list is executed as it
	//	is built or merely compiled.
	//	Base class does not need a destructor but
	//	provides the basis for a set of virtual
	//	destructors for any subclasses.
	//
	CDisplayList(DisplayListMode theMode = kDLCompile);
	virtual ~CDisplayList();
	//
	//	Validate makes sure that the list handle is valid.
	//	It is useful for statically allocated geometry
	//	objects because the list handle needs OpenGL to
	//	allocate its handles and that is not available at
	//	static init time.
	//
	bool Validate();
	//
	//	CallList passes the list to OpenGL for execution.
	//
	void CallList() { glCallList(mListHandle); };
	//
	//	GetHandle returns the OpenGL list number.
	//
	GLuint GetHandle() { return mListHandle; };
	//
	//	GetBounds returns a pointer to a CGLBox that
	//	completely surrounds the model.
	//	It is made virtual so that a subclass could
	//	override and use some more complex method of
	//	computing the bounds.
	//
	virtual Frame3D* GetBounds() { return mBounds; };
	//
	//	Start and stop building list.
	//
	void Start();
	void End();
};
#endif
/*
*	At the top of the object hierarchy is the GeometryObject
*	that has no geometry of its own but provides the basic
*	reference counting for all objects as well serving as
*	a template for the rest. It is an abstract class as it
*	does not know how to write itself to a file.
*	In this version the GeometryObject supplies a set of 
*	visual attributes for the geometry and provides both
*	a display list in which to store the visible representation
*	and the routines to manipulate it.
*	By default a GeometryObject is a white, non-shiny, opaque object.
*/
class GeometryObject {
protected:
	//
	//	Instance Vars.
	//	First ones are responisble for the underlying machanism
	//	of reference counted, tagged, storage.
	//
#ifdef DebugGeom
	char mID[4];
#endif
	int mRefCount;		// use ref counting to minimise create/copy
	int mGeomCode;		// Tells us what kind of geometry we have
	int mLastError;		// Used to record errors for retrieval.
	//
	//	The rest are associated with the physical appearance of
	//	the GeometryObject.
	//
#ifdef UseOpenGL
	RGBColour mColor;
	CDisplayList mList;		// The display list to hold our geometry
	GLfloat mMaterial[5];
	long mFlags;
#endif

public:
	//
	//	Construct/Destruct
	//
	GeometryObject(int code = kGeomNull);
	virtual ~GeometryObject();
	virtual int Delete();		// reduce ref count or destroy
	//
	//	Routines to add an object pointer to something or
	//	remove it.
	//
	GeometryObject* GetPtr();
	//
	//	Routines to manipulate the appearance of the object.
	//
#ifdef UseOpenGL
	void SetColor(RGBColour&);
	void SetColor(GLfloat, GLfloat, GLfloat);
	RGBColour GetColor() { return mColor; };
	void SetMaterials(const GLfloat* f, int n=3);
	void SetMaterials(GLfloat amb,
					  GLfloat diff, 
					  GLfloat spec, 
					  GLfloat shine = 0.0f,
					  GLfloat trans = 1.0f);
	void SetAmbient(GLfloat amb) { mMaterial[0] = amb; };
	void SetDiffuse(GLfloat diff) { mMaterial[1] = diff; };
	void SetSpecular(GLfloat spec) { mMaterial[2] = spec; };
	void SetShininess(GLfloat s) { mMaterial[3] = s; };
	void SetTransparency(GLfloat t) { mMaterial[4] = t; };
	long AddFlags(long);
	//
	//	Have a pass-thru to validate the display list.
	//
	bool Validate() { return mList.Validate(); };
	//
	//	And an accessor for the error code.
	//
	int GetError(void) { return mLastError; };
	//
	//	We supply a generic method to Draw ourselves as an OpenGL
	//	 object but allow subclasses to override.
	//
	virtual void Draw() { mList.CallList(); };
	//
	//	It can call on this helper to incorporate the material
	//	property info.
	//
	virtual void RunMaterial();
#endif

	//
	//	and the Update routine that translates the geometry into GL.
	//	It is empty if OpenGL is not used but it appears in too many
	//	places to comment out!
	//
#ifdef UseOpenGL
	virtual void Update() = 0;
#else
	virtual void Update() {};
#endif
  //
  //  AddToBounds sends in a bounding box and expects us to
  //  alter it to include ourselves. We return the altered box.
  //  Default returns unaltered box, descendents should override.
  //
  virtual Frame3D* AddToBounds(Frame3D* bBox) { return bBox; };
	//
	//	All geometries must override WriteToFile.
	//	It returns true if it succeeds. If it fails then it returns
	//	false and sets the error code.
	//
	virtual bool WriteToFile(FILE* ofp) = 0;
};

/*
 *	We represent a complete set of objects as a Group3D. Any
 *	GeometryObject can be put into a group, including another
 *	Group. This allows a complex, tree stuctured, representation
 *	of heirarchical objects.
 */
class Group3D : public GeometryObject {
protected:
	//
	//	Class Vars.
	//	These control the allocation process. Keep track of
	//	how many slots to allocate initially and how many to
	//	add later.
	//
	static int gNInitialSlots;
	static int gNAdditionalSlots;
	//
	//	Instance Vars
	//	Store the information in an array that we grow as
	//	needed.
	//
	int mNumElem;		// Number of geometry objects in group
	int mNumSlots;		// Number of slots for objects
	GeometryObject** mElems;	// Array of pointers to objects
public:
	//
	//	Constructor/Destructor
	//
	Group3D();
	virtual ~Group3D();
	//
	//	Add an object to a group.
	//
	Group3D& Add(GeometryObject*);
	//
	//	All geometries must override WriteToFile.
	//
	virtual bool WriteToFile(FILE* ofp);
	//
	//	and the Update routine that translates the geometry into GL.
	//
#ifdef UseOpenGL
	virtual void Update();
#else
	virtual void Update() {};
#endif
  //
  //  AddToBounds sends in a bounding box and expects us to
  //  alter it to include ourselves.
  //  Override to add ourselves to the box.
  //
  virtual Frame3D* AddToBounds(Frame3D* bBox);
};

/******************************************************************/
//
//          Mixed objects: both geometry and GeometricObject.
//
/******************************************************************/
/*
*	A vertex is both a point, a mathematical object, and a Geometry
*	Object, a displayable entity. It achieves this by multiple
*	inheritance which is a bit of a pain but seems to fit this
*	case exactly. 
*	If I didn't use multiple inheritance then I would need glue
*	routines to pass all of the Point operations on to the Point
*	member that would then be needed to hold the coordinates.
*/
class Vertex3D : public Point3D, public GeometryObject {
public:
	//
	//	Constructor/Destructor
	//	Supports copy/assign/destroy with both points and vertices.
	//
	Vertex3D();
	Vertex3D(GLfloat nx, GLfloat ny, GLfloat nz);
	Vertex3D(Vertex3D&);
	Vertex3D(Point3D&);
	virtual ~Vertex3D();
	Vertex3D& operator=(Vertex3D&);
	Vertex3D& operator=(Point3D&);
	//
	//	Can move a Vertex.
	//
	Vertex3D& MoveTo(GLfloat nx, GLfloat ny, GLfloat nz);
	Vertex3D& MoveTo(Point3D&);
	//
	//	All geometries must override WriteToFile.
	//
	virtual bool WriteToFile(FILE* ofp);
	//
	//	and the Update routine that translates the geometry into GL.
	//
#ifdef UseOpenGL
	virtual void Update();
#else
	virtual void Update() {};
#endif
  //
  //  AddToBounds sends in a bounding box and expects us to
  //  alter it to include ourselves. 
  //  Override to add ourselves to the box.
  //
  virtual Frame3D* AddToBounds(Frame3D* bBox) { return bBox->AddPoint(this); };
};
/*
 *  A FrameRect3D is the GeometricObject version of a Rect3D,
 *  a rectangular planar quadrilateral.
 */
class FrameRect3D : public Rect3D, public GeometryObject {
public:
  //
  //  ctors
  //
  FrameRect3D(const Point3D* corners = nullptr);
  FrameRect3D(const Point3D& topLeft, const Point3D& topRight,
              const Point3D& bottomRight, const Point3D& bottomLeft);
  virtual ~FrameRect3D() {};
  //
  //  Conversion constructors.
  //
  FrameRect3D(const Rect3D& r);
  FrameRect3D(const Rect3D* rp) : FrameRect3D(*rp) {};
  //
  //  Copy constructor and assignment operator.
  //
  FrameRect3D(const FrameRect3D& f);
  FrameRect3D(const FrameRect3D* fp) : FrameRect3D(*fp) {};
  FrameRect3D& operator=(const FrameRect3D& f);
	//
	//	We override Draw for debugging purposes at the moment.
	//
	virtual void Draw();
	//
	//	All geometries must override WriteToFile.
	//
	virtual bool WriteToFile(FILE* ofp);
	//
	//	and the Update routine that translates the geometry into GL.
	//
#ifdef UseOpenGL
	virtual void Update();
#else
	virtual void Update() {};
#endif
};
/*
 *	A Box3D is the GeometricObject version of a Frame3D, a simple box
 *	with its axes parallel to the coordinate axes.
 */
class Box3D : public Frame3D, public GeometryObject {
public:
	//
	//	Constructors and destructor
	//
	Box3D(const Point3D* startPoint = nullptr);
	Box3D(const Point3D& newMin, const Point3D& newMax);
	Box3D(const Point3D* newMin, const Point3D* newMax);
	Box3D(const Frame3D& f);
	virtual ~Box3D();
  //
  //  Copy constructor and assignment operator.
  //
  Box3D(const Box3D* bp) : Box3D(*bp) {};
	Box3D(const Box3D& b);
  Box3D& operator=(const Box3D& b);
	//
	//	Set completely replaces the existing structure
	//	of the box. It is useful when you have an empty
	//	box created by the default constructor.
	//
	Box3D& Set(const Point3D& newMin, const Point3D& newMax);
	//
	//	Add a style
	//
	void AddWire();
	void AddSolid(GLfloat = 1.0f);	// Argument is transparency value
	//
	//	All geometries must override WriteToFile.
	//
	virtual bool WriteToFile(FILE* ofp);
	//
	//	and the Update routine that translates the geometry into GL.
	//
#ifdef UseOpenGL
	virtual void Update();
#else
	virtual void Update() {};
#endif
  //
  //  AddToBounds sends in a bounding box and expects us to
  //  alter it to include ourselves.
  //  Override to add ourselves to the box.
  //
  virtual Frame3D* AddToBounds(Frame3D* bBox);
};

/******************************************************************/
//
//          Mixed objects: both geometry and GeometricObject.
//
/******************************************************************/
//
//  A line is NOT a vector because it has a definite position as
//  well as a megnitude and orientation.
//
class Line3D : public GeometryObject {
public:
	//
	//	Instance Vars.
	//	Line connects two points.
	//
	Point3D mEnds[2];
	//
	//	Constructor/Destructor
	//
	Line3D(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	Line3D(Point3D& from, Point3D& to);
	Line3D(Point3D& start, Vector3D& arrow);
	virtual ~Line3D();
	//
	//	Endpoint modifiers.
	//
	void SetStart(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void SetStart(Point3D& p);
	void SetEnd(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void SetEnd(Point3D& p);
	//
	//	All geometries must override WriteToFile.
	//
	virtual bool WriteToFile(FILE* ofp);
	//
	//	and the Update routine that translates the geometry into GL.
	//
#ifdef UseOpenGL
	virtual void Update();
#else
	virtual void Update() {};
#endif
  //
  //  AddToBounds sends in a bounding box and expects us to
  //  alter it to include ourselves. 
  //  Override to add ourselves to the box.
  //
  virtual Frame3D* AddToBounds(Frame3D* bBox);
};
/*
*	A polyline consists of an array of points joined by lines.
*	At the moment I am going to make all the line segments the
*	same color so that we need store only an array of Points
*	and not an array of vertices. It is not clear that OpenGL
*	supports polychrome polylines and they would certainly be a
*	royal pain to write out in an ascii file so I will stick to
*	applying a single color to the whole line.
*	Like a group, a polyline should be able to grow if we add
*	more points than it can hold but that will come later. For
*	now specify the maximum size at startup.
*/
class PolyLine3D : public GeometryObject {
public:
	//
	//	Instance vars.
	//
	Point3D* mPoints;		// array of points
	int mNumPoint;			// number of points in use
	int mNumSlot;			// maximum number of points
	//
	//	Constructor/Destructor
	//
	PolyLine3D(int nSlots = 20);
	PolyLine3D(PolyLine3D& p);
	virtual ~PolyLine3D();
	//
	//	Add a new point.
	//
	void Add(Point3D& p);
	//
	//	All geometries must override WriteToFile.
	//
	virtual bool WriteToFile(FILE* ofp);
	//
	//	and the Update routine that translates the geometry into GL.
	//
#ifdef UseOpenGL
	virtual void Update();
#else
	virtual void Update() {};
#endif
  //
  //  AddToBounds sends in a bounding box and expects us to
  //  alter it to include ourselves. 
  //  Override to add ourselves to the box.
  //
  virtual Frame3D* AddToBounds(Frame3D* bBox);
};
/*
*	Triangles have three vertices.
*/
class Triangle3D : public GeometryObject {
public:
	//
	//	Instance vars, three points to define the vertices.
	//
	Point3D mVertices[3];
	//
	//	Constructor/Destructor
	//
	Triangle3D();		// builds  an empty triangle
	Triangle3D(Vertex3D &v1, Vertex3D &v2, Vertex3D &v3);
	Triangle3D(Point3D &v1, Point3D &v2, Point3D &v3);
	virtual ~Triangle3D();
	//
	//	All geometries must override WriteToFile.
	//
	virtual bool WriteToFile(FILE* ofp);
	//
	//	and the Update routine that translates the geometry into GL.
	//
#ifdef UseOpenGL
	virtual void Update();
#else
	virtual void Update() {};
#endif
  //
  //  AddToBounds sends in a bounding box and expects us to
  //  alter it to include ourselves. 
  //  Override to add ourselves to the box.
  //
  virtual Frame3D* AddToBounds(Frame3D* bBox);
};
/*
*	Ellipsoid is described by three vectors that form the three
*	semi-axes of the ellipse and a point for the center.
*/
class Ellipsoid3D : public GeometryObject {
public:
	//
	//	Instance vars are the origin and three axis vectors.
	//
	Point3D mOrigin;
	Vector3D mAxes[3];
	//
	//	Constructors make a variety of default ellipsoids.
	//	Default is a sphere of given radius centered at
	//	the origin.
	//	Next builds ellipsoids with axes parallel to coord
	//	axes.
	//	Last is general one.
	//
	Ellipsoid3D(GLfloat radius = 1.0f);
	Ellipsoid3D(GLfloat xlen, GLfloat yLen, GLfloat zLen);
	Ellipsoid3D(Vector3D& v1, Vector3D& v2, Vector3D& v3);
	Ellipsoid3D(Ellipsoid3D& e);
	virtual ~Ellipsoid3D();
	Ellipsoid3D& operator=(Ellipsoid3D& e);
	//
	//	Can move the ellipsoid to a new centre in various
	//	ways.
	//
	Ellipsoid3D& MoveTo(Point3D& p);
	Ellipsoid3D& MoveTo(GLfloat nx, GLfloat ny, GLfloat nz);
	Ellipsoid3D& MoveBy(Vector3D& v);
	//
	//	All geometries must override WriteToFile.
	//
	virtual bool WriteToFile(FILE* ofp);
	//
	//	and the Update routine that translates the geometry into GL.
	//
#ifdef UseOpenGL
	virtual void Update();
#else
	virtual void Update() {};
#endif
  //
  //  AddToBounds sends in a bounding box and expects us to
  //  alter it to include ourselves. 
  //  Override to add ourselves to the box.
  //
  virtual Frame3D* AddToBounds(Frame3D* bBox);
};
/*
*   A tube is a cylinder which may or may not have end caps.
*   Internally it is represented by three numbers that describe
*   a cylinder oriented along a mythical C axis and then a vector
*   that describes the orientation of that C axis.
*
*   NOTE at the moment the tube axis must pass through the origin.
*   This restriction can be removed by adding a point to represent
*   the centre of the bottom or the min/max zero of the axis.
*   In fact this is less of a restriction as mAxis is currently unused!
*/
class Tube3D : public GeometryObject {
protected:
    GLfloat mCMin;      // lower end cap
    GLfloat mCMax;      // upper end cap
    GLfloat mRadius;    // radius of cylinder
    Vector3D mAxis;     // axis direction of cyclinder
    bool mBottomCap;    // true if we have a cMin cap
    bool mTopCap;       // true if we have a cMax cap
public:
    //
    //	Constructor/destructor.
    //
    Tube3D(double min, double max, double radius, bool bottom = true,
	bool top = true);
    virtual ~Tube3D();
    //
    //	have to be able to set the orientation.
    //
    void SetOrientation(Vector3D& newAxis);
    //
    //	All geometries must override WriteToFile.
    //
    virtual bool WriteToFile(FILE* ofp);
    //
    //	and the Update routine that translates the geometry into GL.
    //	It will compile the geometry description into an open GL display
    //	list that will be called by Draw.
    //
#ifdef UseOpenGL
	virtual void Update();
#else
	virtual void Update() {};
#endif
  //
  //  AddToBounds sends in a bounding box and expects us to
  //  alter it to include ourselves. 
  //  Override to add ourselves to the box.
  //
  virtual Frame3D* AddToBounds(Frame3D* bBox);
};
/*
 *  A cap is a circular flat surface defined by a center point,
 *	a perpendicular vector to set the direction, and an inner
 *	and outer radius. It can be used to cap a tube.
 *  Like Tube3D, the axis is currently assumed to be Z.
 */
class Cap3D : public GeometryObject {
protected:
  GLfloat mRMin;		// inner radius
  GLfloat mRMax;		// outer radius
	Point3D mCenter;  // point at center of cap
  Vector3D mAxis;		// axis direction of cyclinder
public:
  //
  //	Constructor/destructor.
	//	Cap defaults to origin and pependicular to Z axis.
	//
  Cap3D(double rmin, double rmax);
  virtual ~Cap3D();
  //
  //	have to be able to set the center and orientation.
  //
  void SetCenter(Point3D& newCenter);
	void SetCenter(double x, double y, double z);
  void SetOrientation(Vector3D& newAxis);
  void SetOrientation(double x, double y, double z);
	void SetInnerRadius(double r);
  //
  //	All geometries must override WriteToFile.
  //
  virtual bool WriteToFile(FILE* ofp);
  //
  //	and the Update routine that translates the geometry into GL.
  //	It will compile the geometry description into an open GL display
  //	list that will be called by Draw.
  //
#ifdef UseOpenGL
	virtual void Update();
#else
	virtual void Update() {};
#endif
  //
  //  AddToBounds sends in a bounding box and expects us to
  //  alter it to include ourselves. 
  //  Override to add ourselves to the box.
  //
  virtual Frame3D* AddToBounds(Frame3D* bBox);
};

#endif

