/*
 *  Line3D.cpp
 *  Simulator1_13_09
 *
 *	This is a line segment with a visible representation.
 *
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <math.h>
#include "GeometricObjects.h"
/*
 *	A line runs between two vertices. Both the vertices and
 *	the line can have their own attributes.
 */
//
//	Constructor/Destructor
//
Line3D::Line3D(float nx, float ny, float nz) : 
GeometryObject( kGeomLine) {
	mEnds[0].Set(nx,ny,nz);
	mEnds[1].Set(nx,ny,nz);
	Update();
}
Line3D::Line3D(Point3D& from, Point3D& to) : 
GeometryObject( kGeomLine) {
	mEnds[0] = from;
	mEnds[1] = to;
	Update();
}

Line3D::Line3D(Point3D& start, Vector3D& arrow) :  
GeometryObject( kGeomLine) {
	mEnds[0] = start;
	mEnds[1] = start + arrow;
	Update();
}

Line3D::~Line3D() {
}
//
//	Endpoint modifiers.
//
void Line3D::SetStart(float nx, float ny, float nz) {
	mEnds[0].Set(nx,ny,nz);
	Update();
}
void Line3D::SetStart(Point3D& p) {
	mEnds[0] = p;
	Update();
}
void Line3D::SetEnd(float nx, float ny, float nz) {
	mEnds[1].Set(nx,ny,nz);
	Update();
}
void Line3D::SetEnd(Point3D& p) {
	mEnds[1] = p;
	Update();
}
//
//	Output routines are required.
//
bool Line3D::WriteToFile(FILE* ofp) {
	mLastError = fprintf(ofp, "line %g %g %g %g %g %g\n",
												mEnds[0].mX, mEnds[0].mY, mEnds[0].mZ, 
												mEnds[1].mX, mEnds[1].mY, mEnds[1].mZ);
	return mLastError > 0;
}

#ifdef UseOpenGL
void Line3D::Update() {
	mList.Start();
	glBegin(GL_LINES);
  mColor.Draw();
	glVertex3f(mEnds[0].mX, mEnds[0].mY, mEnds[0].mZ);
	glVertex3f(mEnds[1].mX, mEnds[1].mY, mEnds[1].mZ);
	glEnd();
	mList.End();
}
#endif
//
//  AddToBounds sends in a bounding box and expects us to
//  alter it to include ourselves. 
//  Override to add ourselves to the box.
//
Frame3D* Line3D::AddToBounds(Frame3D* bBox)
{
  bBox->AddPoint(mEnds[0]);
  return bBox->AddPoint(mEnds[1]);
}

