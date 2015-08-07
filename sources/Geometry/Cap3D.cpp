/*
 *  Cap3D.cpp
 *  Simulator1_13_09
 *
 *   A cap is a circular flat surface defined by a center point,
 *	a perpendicular vector to set the direction, and an inner
 *	and outer radius. It can be used to cap a tube.
 *
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <math.h>
#include "GeometricObjects.h"
//
//	Constructor/destructor.
//	Cap defaults to origin and pependicular to Z axis.
//
Cap3D::Cap3D(double rmin, double rmax) :
GeometryObject(kGeomCap) {
	mRMin = (GLfloat) rmin;
	mRMax = (GLfloat) rmax;
	mCenter.Set(0.0,0.0,0.0);
	mAxis.Set(0.0,0.0,1.0);
	Update();
}
Cap3D::~Cap3D() {
}
//
//	have to be able to set the center and orientation.
//
void Cap3D::SetCenter(Point3D& newCenter) {
	mCenter = newCenter;
	Update();
}
void Cap3D::SetCenter(double x, double y, double z) {
	mCenter.Set((GLfloat) x, (GLfloat) y, (GLfloat) z);
	Update();
}
void Cap3D::SetOrientation(Vector3D& newAxis) {
	mAxis = newAxis.Normalize();
	Update();
}
void Cap3D::SetOrientation(double nx, double ny, double nz) {
	mAxis.Set(nx, ny, nz);
	mAxis.Normalize();
	Update();
}
//
void Cap3D::SetInnerRadius(double r) {
	mRMin = (GLfloat) r;
	Update();
}
//
//	All geometries must override WriteToFile.
//
bool Cap3D::WriteToFile(FILE* ofp) {
	mLastError = fprintf(ofp,"cap %g %g %g %g %g %g %g %g\n",
											mCenter.mX, mCenter.mY, mCenter.mZ,
											mRMin, mRMax,
											mAxis.mX, mAxis.mY, mAxis.mZ);
	return mLastError > 0;
}

#ifdef UseOpenGL
//
//	and the Update routine that translates the geometry into GL.
//	It will compile the geometry description into an open GL display
//	list that will be called by Draw.
//
void Cap3D::Update() {
	GLUquadricObj *glq = gluNewQuadric();
	mList.Start();
	if (mFlags & kGFWire) { // Wire Frame
		gluQuadricDrawStyle(glq, GLU_LINE);
    mColor.Draw();
	} else {
    RunMaterial();
  }
	
	glPushMatrix();
	glTranslatef(mCenter.mX, mCenter.mX, mCenter.mZ);
	gluDisk(glq, mRMin, mRMax, 20, 10);
	glPopMatrix();
	
	mList.End();
	gluDeleteQuadric(glq);
}
#endif
//
//  AddToBounds sends in a bounding box and expects us to
//  alter it to include ourselves. 
//  Override to add ourselves to the box.
//  Like the ellipsoid we have to add the eight corners of a
//  bounding box. Note that at the moment the axis is taken
//  as the z axis.
//
Frame3D* Cap3D::AddToBounds(Frame3D* bBox)
{
  Vector3D ax1(mRMax, 0, 0);
  Vector3D ax2(0, mRMax, 0);
  bBox->AddPoint(mCenter + ax1 + ax2);
  bBox->AddPoint(mCenter + ax1 - ax2);
  bBox->AddPoint(mCenter - ax1 + ax2);
  bBox->AddPoint(mCenter - ax1 - ax2);
  return bBox;
}


