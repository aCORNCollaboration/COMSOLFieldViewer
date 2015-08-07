/*
 *  Ellipsoid3D.cpp
 *  Simulator1_13_09
 *
 *	An Ellipsoid is described by three vectors that form the three
 *	semi-axes of the ellipse and a point for the center.
 *	It also has an overall attribute set.
 *
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <math.h>
#include "GeometricObjects.h"
//
//	Constructors make a variety of default ellipsoids.
//	Default is a sphere of given radius centered at
//	the origin.
//	Next builds ellipsoids with axes parallel to coord
//	axes.
//	Last is general one.
//
Ellipsoid3D::Ellipsoid3D(GLfloat radius) :  
GeometryObject( kGeomEllipse) {
	mOrigin.mX = 0.0f;
	mOrigin.mY = 0.0f;
	mOrigin.mZ = 0.0f;
	mAxes[0].mX = radius;
	mAxes[0].mY = 0.0f;
	mAxes[0].mZ = 0.0f;
	mAxes[1].mX = 0.0f;
	mAxes[1].mY = radius;
	mAxes[1].mZ = 0.0f;
	mAxes[2].mX = 0.0f;
	mAxes[2].mY = 0.0f;
	mAxes[2].mZ = radius;
	Update();
}
Ellipsoid3D::Ellipsoid3D(GLfloat xLen, GLfloat yLen, GLfloat zLen) :  
GeometryObject( kGeomEllipse) {
	mOrigin.mX = 0.0f;
	mOrigin.mY = 0.0f;
	mOrigin.mZ = 0.0f;
	mAxes[0].mX = xLen;
	mAxes[0].mY = 0.0f;
	mAxes[0].mZ = 0.0f;
	mAxes[1].mX = 0.0f;
	mAxes[1].mY = yLen;
	mAxes[1].mZ = 0.0f;
	mAxes[2].mX = 0.0f;
	mAxes[2].mY = 0.0f;
	mAxes[2].mZ = zLen;
	Update();
}
Ellipsoid3D::Ellipsoid3D(Vector3D& v1, Vector3D& v2, Vector3D& v3) :  
GeometryObject( kGeomEllipse) {
	mOrigin.mX = 0.0f;
	mOrigin.mY = 0.0f;
	mOrigin.mZ = 0.0f;
	mAxes[0] = v1;
	mAxes[1] = v2;
	mAxes[2] = v3;
	Update();
}
Ellipsoid3D::Ellipsoid3D(Ellipsoid3D& e) :  
GeometryObject( kGeomEllipse) {
	mOrigin = e.mOrigin;
	mAxes[0] = e.mAxes[0];
	mAxes[1] = e.mAxes[1];
	mAxes[2] = e.mAxes[2];
	
#ifdef UseOpenGL
	mColor = e.mColor;
	SetMaterials(e.mMaterial, 5);
	mFlags = e.mFlags;
	Update();
#endif
}
Ellipsoid3D::~Ellipsoid3D() {
}
Ellipsoid3D& Ellipsoid3D::operator=(Ellipsoid3D& e) {
	mOrigin = e.mOrigin;
	mAxes[0] = e.mAxes[0];
	mAxes[1] = e.mAxes[1];
	mAxes[2] = e.mAxes[2];
	
#ifdef UseOpenGL
	mColor = e.mColor;
	SetMaterials(e.mMaterial, 5);
	mFlags = e.mFlags;
	Update();
#endif
	
	return *this;
}
//
//	Can move the ellipsoid to a new centre in various
//	ways.
//
Ellipsoid3D& Ellipsoid3D::MoveTo(Point3D& p) {
	mOrigin = p;
	Update();
	return *this;
}
Ellipsoid3D& Ellipsoid3D::MoveTo(GLfloat nx, GLfloat ny, GLfloat nz) {
	mOrigin.Set(nx, ny, nz);
	Update();
	return *this;
}
Ellipsoid3D& Ellipsoid3D::MoveBy(Vector3D& v) {
	mOrigin += v;
	Update();
	return *this;
}
//
//	WriteToFile is barely implemented for the Ellipsoid
//	yet as gla does not support ellipsoids.
//	Write out as a sphere.
//
bool Ellipsoid3D::WriteToFile(FILE* ofp) {
	//
	//	Figure average size.
	//
	Real r1 = mAxes[0].Length();
	Real r2 = mAxes[0].Length();
	Real r3 = mAxes[2].Length();
	double radius = (r1 + r2 + r3) / 3.0;
	mLastError = fprintf(ofp, "sphere %g %g %g %g \n",
												radius,
												mOrigin.mX, mOrigin.mY, mOrigin.mZ);
	return mLastError > 0;
}

#ifdef UseOpenGL
//
//	Translate into a GLU quadric object in a list.
//
void Ellipsoid3D::Update() {
	GLUquadricObj *glq = gluNewQuadric();
	mList.Start();
	if (mFlags & kGFWire) { // Wire Frame
		gluQuadricDrawStyle(glq, GLU_LINE);
    mColor.Draw();
	} else {
    RunMaterial();
  }
	
	glPushMatrix();
	glTranslatef(mOrigin.mX, mOrigin.mY, mOrigin.mZ);
	gluSphere(glq, mAxes[0].mX, 20, 20);
	glPopMatrix();
	
	mList.End();
	gluDeleteQuadric(glq);
}
#endif

//
//  AddToBounds sends in a bounding box and expects us to
//  alter it to include ourselves. 
//  Override to add ourselves to the box.
//  This is somewhat nasty because of the generality of an
//  ellipsoid. We have to construct all 8 corners of the 
//  general bounding box and add them to the bounds.
//
Frame3D* Ellipsoid3D::AddToBounds(Frame3D* bBox)
{
  Point3D corner = mOrigin + mAxes[0] + mAxes[1] + mAxes[2];
  bBox->AddPoint(corner);
  corner = mOrigin + mAxes[0] + mAxes[1] - mAxes[2];
  bBox->AddPoint(corner);
  corner = mOrigin + mAxes[0] - mAxes[1] + mAxes[2];
  bBox->AddPoint(corner);
  corner = mOrigin + mAxes[0] - mAxes[1] - mAxes[2];
  bBox->AddPoint(corner);
  corner = mOrigin - mAxes[0] + mAxes[1] + mAxes[2];
  bBox->AddPoint(corner);
  corner = mOrigin - mAxes[0] + mAxes[1] - mAxes[2];
  bBox->AddPoint(corner);
  corner = mOrigin - mAxes[0] - mAxes[1] + mAxes[2];
  bBox->AddPoint(corner);
  corner = mOrigin - mAxes[0] - mAxes[1] - mAxes[2];
  return bBox->AddPoint(corner);
}

