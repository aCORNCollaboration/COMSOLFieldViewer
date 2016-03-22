/*
 *  PolyLine3D.cpp
 *  Simulator1_13_09
 *
 *	A polyline is a connected set of line segments with a single
 *	unified visible representation.
 *
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <math.h>
#include "GeometricObjects.h"
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
//
//	Constructor/Destructor
//
PolyLine3D::PolyLine3D(int nSlots) : 
GeometryObject( kGeomPolyLine) {
	mNumPoint = 0;
	mNumSlot = nSlots;
	mPoints = new Point3D[mNumSlot];
	Update();
}
PolyLine3D::PolyLine3D(PolyLine3D& p) : 
GeometryObject( kGeomPolyLine) {
	mNumPoint = p.mNumPoint;
	mNumSlot = p.mNumPoint;
	mPoints = new Point3D[mNumSlot];
	for (int i = 0; i < mNumPoint; ++i) {
		mPoints[i] = p.mPoints[i];
	}
	
#ifdef UseOpenGL
	mColor = p.mColor;
	SetMaterials(p.mMaterial, 5);
	mFlags = p.mFlags;
	Update();
#endif
}
PolyLine3D::~PolyLine3D() {
	if (mPoints != NULL) {
		delete[] mPoints;
	}
}
//
//	Add a new point.
//
void PolyLine3D::Add(const Point3D& p) {
	if (mNumPoint < mNumSlot-1) {
		mPoints[mNumPoint++] = p;
	}
	Update();
}
//
//	Add a whole polyline. This can handle two cases. In the
//	first case the new polyline starts with the same point
//	that the existing line ends with. In this case the point
//	is NOT duplicated. In the second case the new lines starts
//	at a different point. In this case all points are taken
//	from the new line.
/*
 void PolyLine3D::Add(PolyLine3D& newl) {
 //
 //	Make sure that there is room for the new points.
 //
 if (mNumPoints + newl.mNumPoints <= mNumSlot) {
 //
 //	See if first points match and if so arrange
 //	to skip first point.
 //
 int start = 0;
 if (newl.mPoints[0] == mPoints[mNumPoints-1]) {
 start = 1;
 }
 //
 //	Copy points from newl into this.
 //
 for (; start < newl.mNumPoints; ++start) {
 mPoints[mNumPoints++] = newl.mPoints[start];
 }
 }
 }
 //
 //	This returns a reversed copy of a PolyLine. The new
 //	line will have the same dimensions as the source
 //	but will list the points in reverse order.
 //
 PolyLine3D* PolyLine3D::Reverse() {
 PolyLine3D* newpoly = new PolyLine3D(mNumSlot);
 newpoly->mNumPoints = mNumPoints;
 int i = mNumPoints-1;
 for (int j = 0; i >= 0; ++j, --i) {
 newpoly->mPoints[j] = mPoints[i];
 }
 }
 */
//
//	All geometries must override WriteToFile.
//
bool PolyLine3D::WriteToFile(FILE* ofp) {
	int error = 0;
	if (mNumPoint > 1) {
		if (fprintf(ofp, "polyline %d", mNumPoint) < 1) ++error;
		for (int i = 0; i < mNumPoint; ++ i) {
			if (fprintf(ofp, 
									"%f, %f, %f",
									mPoints[i].mX,
									mPoints[i].mY,
									mPoints[i].mZ) < 0) ++error;
		}
		if (fprintf(ofp,"\n") < 0) ++error;
	}
	return error == 0;
}


#ifdef UseOpenGL
//
//	This will come later!
//
void PolyLine3D::Update() {
	if (mNumPoint > 1) {
		mList.Start();
    mColor.Draw();
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < mNumPoint; ++i) {
			glVertex3f(mPoints[i].mX, mPoints[i].mY, mPoints[i].mZ);
		}
		glEnd();
		mList.End();
	}
}
#endif

//
//  AddToBounds sends in a bounding box and expects us to
//  alter it to include ourselves. 
//  Override to add ourselves to the box.
//
Frame3D* PolyLine3D::AddToBounds(Frame3D* bBox)
{
  if (mNumPoint > 0) {
    for (int i = 0; i < mNumPoint; ++i) {
      bBox->AddPoint(mPoints[i]);
    }
  }
  return bBox;
}


