/*
 *  Vertex3D.cpp
 *  Simulator1_13_09
 *
 *	A Vertex is a point with a visible representation.
 *
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <math.h>
#include "GeometricObjects.h"
/*
 *	A vertex is both a Point and a GeometryObject so that it can
 *	represent a visible point.
 */
//
//	Constructor/Destructor
//	Supports copy/assign/destroy
//
Vertex3D::Vertex3D() :  
Point3D(0, 0, 0) {
	Update();
}
Vertex3D::Vertex3D(GLfloat nx, GLfloat ny, GLfloat nz) :  
Point3D( nx, ny, nz) {
	Update();
}

Vertex3D::Vertex3D(Vertex3D& v) :  
//			Point3D(dynamic_cast<Point3D&> (v)) {
Point3D((Point3D&) (v)) {
	Update();
}

Vertex3D::Vertex3D(Point3D& p) :  
Point3D(p) {
	Update();
}

Vertex3D::~Vertex3D() {
}

Vertex3D& Vertex3D::operator=(Vertex3D& v) {
	mX = v.mX;
	mY = v.mY;
	mZ = v.mZ;
	
#ifdef UseOpenGL
	mColor = v.mColor;
	mFlags = v.mFlags;
	SetMaterials(v.mMaterial, 5);
	Update();
#endif
	
	return *this;
}

Vertex3D& Vertex3D::operator=(Point3D& p) {
	mX = p.mX;
	mY = p.mY;
	mZ = p.mZ;
	Update();
	return *this;
}

//
//	Set can move a Vertex.
//
Vertex3D& Vertex3D::MoveTo(GLfloat nx, GLfloat ny, GLfloat nz) {
	mX = nx;
	mY = ny;
	mZ = nz;
	Update();
	return *this;
}
Vertex3D& Vertex3D::MoveTo(Point3D& p) {
	mX = p.mX;
	mY = p.mY;
	mZ = p.mZ;
	Update();
	return *this;
}
//
//	All geometry objects must know how to write and update themselves.
//
bool Vertex3D::WriteToFile(FILE* ofp) {
	//
	//	Vertex prints as a colored point.
	//
	mLastError = fprintf(ofp,"point %f, %f, %f\n", mX, mY, mZ);
	return mLastError > 0;
}

#ifdef UseOpenGL
void Vertex3D::Update() {
	mList.Start();
	mColor.Draw();
	glVertex3f(mX, mY, mZ);
	mList.End();
}
#endif



