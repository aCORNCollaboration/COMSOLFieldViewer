/*
 *  Triangle3D.cpp
 *  Simulator1_13_09
 *
 *	Triangles have three Point3Ds as vertices.
 *
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <math.h>
#include "GeometricObjects.h"
//
//	Constructor/Destructor
//
//	Default one builds  an empty triangle
//Triangle3D::Triangle3D() : GeometryObject( kGeomTriangle),
//	mVertices[0](), mVertices[1](), mVertices[2]() {
Triangle3D::Triangle3D() : GeometryObject( kGeomTriangle) {
}
//Triangle3D::Triangle3D(Vertex3D& v1, Vertex3D& v2, Vertex3D& v3) :  
//			GeometryObject( kGeomTriangle),
//			mVertices[0](v1), mVertices[1](v2), mVertices[2](v3) {
Triangle3D::Triangle3D(Vertex3D& v1, Vertex3D& v2, Vertex3D& v3) :  
GeometryObject( kGeomTriangle) {
	mVertices[0] = v1;
	mVertices[1] = v2;
	mVertices[2] = v3;
	Update();
}
Triangle3D::Triangle3D(Point3D& p1, Point3D& p2, Point3D& p3) :  
GeometryObject( kGeomTriangle) {
	mVertices[0] = p1;
	mVertices[1] = p2;
	mVertices[2] = p3;
	Update();
}
Triangle3D::~Triangle3D() {
}
//
//	All geometries must override WriteToFile.
//
bool Triangle3D::WriteToFile(FILE* ofp) {
	mLastError =  fprintf(ofp, "Triangle %g %g %g %g %g %g %g %g %g\n",
														mVertices[0].mX, mVertices[0].mY, mVertices[0].mZ, 
														mVertices[1].mX, mVertices[1].mY, mVertices[1].mZ,
														mVertices[2].mX, mVertices[2].mY, mVertices[2].mZ);
	return mLastError > 0;
}

#ifdef UseOpenGL
//
//	This will come later!
//
void Triangle3D::Update() {
}
#endif

//
//  AddToBounds sends in a bounding box and expects us to
//  alter it to include ourselves. 
//  Override to add ourselves to the box.
//
Frame3D* Triangle3D::AddToBounds(Frame3D* bBox)
{
  bBox->AddPoint(mVertices[0]);
  bBox->AddPoint(mVertices[1]);
  return bBox->AddPoint(mVertices[2]);
}


