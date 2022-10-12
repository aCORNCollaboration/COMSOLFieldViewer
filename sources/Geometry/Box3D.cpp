/*
 *  Box3D.cpp
 *  Simulator1_13_09
 *
 *	A box is a Frame with a visible representation. That is, it is a
 *	rectangular parallelipiped aligned with the coordinate axes.
 *
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <math.h>
#include "GeometricObjects.h"
//
//	Constructors
//
Box3D::Box3D(const Point3D* startPoint) :
        Frame3D(startPoint), GeometryObject( kGeomBox)
{
	Update();
}
Box3D::Box3D(const Point3D& newMin, const Point3D& newMax) :
            Frame3D(newMin, newMax), GeometryObject( kGeomBox)
{
	Update();
}
Box3D::Box3D(const Point3D* newMin, const Point3D* newMax) :
        Frame3D(newMin, newMax), GeometryObject( kGeomBox)
{
	Update();
}
//
//	Destructor
//
Box3D::~Box3D() {
}
//
//  Copy constructor and assignment operator.
//
Box3D::Box3D(const Box3D& b) :
        Frame3D((const Frame3D&) b), GeometryObject( kGeomBox)
{
#ifdef UseOpenGL
	mColor._m._c.red = b.mColor._m._c.red;
	mColor._m._c.green = b.mColor._m._c.green;
	mColor._m._c.blue = b.mColor._m._c.blue;
	SetMaterials(b.mMaterial, 5);
	mFlags = b.mFlags;
	Update();
#endif
}
//
//	Reset the whole geometry of the box. 
//
Box3D& Box3D::Set(const Point3D& newMin, const Point3D& newMax)
{
  Frame3D::Set(newMin, newMax);
	Update();
	return *this;
}
#ifdef UseOpenGL
//
//	Set box appearance.
//
void Box3D::AddWire() {
	AddFlags(kGFWire);
	Update();
}
void Box3D::AddSolid(GLfloat t) {
	AddFlags(kGFSolid);
	SetTransparency(t);
	Update();
}
#endif
//
//	All geometries must override WriteToFile and Draw.
//
bool Box3D::WriteToFile(FILE* ofp) {
	return false;
}

#ifdef UseOpenGL
void Box3D::Update() {
  if (isEmpty) return;
	mList.Start();
	if (mFlags & kGFSolid) {	// Solid Box
		RunMaterial();;
		
		glBegin(GL_QUAD_STRIP);
		glVertex3f(mMin.mX, mMax.mY, mMax.mZ);
		glVertex3f(mMin.mX, mMin.mY, mMax.mZ);
		glVertex3f(mMax.mX, mMax.mY, mMax.mZ);
		glVertex3f(mMax.mX, mMin.mY, mMax.mZ);
		glVertex3f(mMax.mX, mMax.mY, mMin.mZ);
		glVertex3f(mMax.mX, mMin.mY, mMin.mZ);
		glVertex3f(mMin.mX, mMax.mY, mMin.mZ);
		glVertex3f(mMin.mX, mMin.mY, mMin.mZ);
		glVertex3f(mMin.mX, mMax.mY, mMax.mZ);
		glVertex3f(mMin.mX, mMin.mY, mMax.mZ);
		glEnd();
		
		glBegin(GL_QUADS);
		glVertex3f(mMin.mX, mMin.mY, mMax.mZ);
		glVertex3f(mMax.mX, mMin.mY, mMax.mZ);
		glVertex3f(mMax.mX, mMin.mY, mMin.mZ);
		glVertex3f(mMin.mX, mMin.mY, mMin.mZ);
		glEnd();
		
		glBegin(GL_QUADS);
		glVertex3f(mMin.mX, mMax.mY, mMax.mZ);
		glVertex3f(mMax.mX, mMax.mY, mMax.mZ);
		glVertex3f(mMax.mX, mMax.mY, mMin.mZ);
		glVertex3f(mMin.mX, mMax.mY, mMin.mZ);
		glEnd();
	}
	
	if (mFlags & kGFWire) { // Wire Frame
    mColor.Draw();
		
		glBegin(GL_LINE_LOOP);
		glVertex3f(mMin.mX, mMax.mY, mMax.mZ);
		glVertex3f(mMax.mX, mMax.mY, mMax.mZ);
		glVertex3f(mMax.mX, mMin.mY, mMax.mZ);
		glVertex3f(mMin.mX, mMin.mY, mMax.mZ);
		glEnd();
		
		glBegin(GL_LINE_LOOP);
		glVertex3f(mMin.mX, mMax.mY, mMin.mZ);
		glVertex3f(mMax.mX, mMax.mY, mMin.mZ);
		glVertex3f(mMax.mX, mMin.mY, mMin.mZ);
		glVertex3f(mMin.mX, mMin.mY, mMin.mZ);
		glEnd();
		
		glBegin(GL_LINES);
		glVertex3f(mMin.mX, mMax.mY, mMin.mZ);
		glVertex3f(mMin.mX, mMax.mY, mMax.mZ);
		
		glVertex3f(mMax.mX, mMax.mY, mMin.mZ);
		glVertex3f(mMax.mX, mMax.mY, mMax.mZ);
		
		glVertex3f(mMax.mX, mMin.mY, mMin.mZ);
		glVertex3f(mMax.mX, mMin.mY, mMax.mZ);
		
		glVertex3f(mMin.mX, mMin.mY, mMin.mZ);
		glVertex3f(mMin.mX, mMin.mY, mMax.mZ);
		glEnd();
	}
	mList.End();
}
#endif
//
//  AddToBounds sends in a bounding box and expects us to
//  alter it to include ourselves. 
//  Override to add ourselves to the box.
//
Frame3D* Box3D::AddToBounds(Frame3D* bBox)
{
  bBox->AddPoint(mMin);
  return bBox->AddPoint(mMax);
}

