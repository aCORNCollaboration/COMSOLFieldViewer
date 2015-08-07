/*
 *  Tube3D.cpp
 *  Simulator1_13_09
 *
 *   A tube is a cylinder which may or may not have end caps.
 *   Internally it is represented by three numbers that describe
 *   a cylinder oriented along a mythical C axis and then a vector
 *   that describes the orientation of that C axis.
 *
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <math.h>
#include "GeometricObjects.h"
//
//  Constructor?destructor.
//
//  Default orientation is along z.
//
Tube3D::Tube3D(double min, double max, double radius, 
							 bool bottom, bool top) : mAxis(0.0, 0.0, 1.0) , 
							GeometryObject(kGeomTube) 
{
	mCMin = (GLfloat) min;
	mCMax = (GLfloat) max;
	mRadius = (GLfloat) radius;
	mBottomCap = bottom;
	mTopCap = top;
	Update();
}
//
//  With no owned storage destructor has nothing to do.
//
Tube3D::~Tube3D() 
{
}
//
//	have to be able to set the orientation.
//
void Tube3D::SetOrientation(Vector3D& newAxis) 
{
	mAxis = newAxis.Normalize();
	Update();
}
//
//	All geometries must override WriteToFile.
//
bool Tube3D::WriteToFile(FILE* ofp) 
{
	int top = 0, bottom = 0;
	if (mBottomCap) {
		bottom = 1;
	}
	if (mTopCap) {
		top = 1;
	}
	mLastError = fprintf(ofp, "tube %g %g %g %g %g %g %d %d\n",
											mCMin, mCMax, mRadius,
											mAxis.mX, mAxis.mY, mAxis.mZ,
											bottom, top);
	return mLastError > 0;
}

#ifdef UseOpenGL
//
//  and the Update routine that translates the geometry into GL.
//  It will compile the geometry description into an open GL display
//  list that will be called by Draw.
//  At the moment it does NOT use the axis to orient the cylinder and
//  so it lies along the Z axis.
//
void Tube3D::Update() 
{
	GLUquadricObj *glq = gluNewQuadric();
	mList.Start();
	if (mFlags & kGFWire) { // Wire Frame
		gluQuadricDrawStyle(glq, GLU_LINE);
    mColor.Draw();
	} else {
    RunMaterial();
  }
	
	glPushMatrix();
	glTranslatef(0, 0, mCMin);
	gluCylinder(glq, mRadius, mRadius, mCMax - mCMin, 20, 20);
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
Frame3D* Tube3D::AddToBounds(Frame3D* bBox)
{
  Point3D corner(mRadius, mRadius, mCMin);
  bBox->AddPoint(corner);
  corner.Set(mRadius, -mRadius, mCMin);
  bBox->AddPoint(corner);
  corner.Set(-mRadius, mRadius, mCMin);
  bBox->AddPoint(corner);
  corner.Set(-mRadius, -mRadius, mCMin);
  bBox->AddPoint(corner);
  corner.Set(mRadius, mRadius, mCMax);
  bBox->AddPoint(corner);
  corner.Set(mRadius, -mRadius, mCMax);
  bBox->AddPoint(corner);
  corner.Set(-mRadius, mRadius, mCMax);
  bBox->AddPoint(corner);
  corner.Set(-mRadius, -mRadius, mCMax);
  return bBox->AddPoint(corner);
}


