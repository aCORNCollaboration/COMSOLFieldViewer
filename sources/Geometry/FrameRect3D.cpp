//
//  FrameRect3D.cpp
//  FieldViewer
//
//  Created by Brian Collett on 2/28/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <stdio.h>

extern "C" {
#include "assert.h"
}
#include "GeometricObjects.h"

FrameRect3D::FrameRect3D(const Point3D* corners) :
      Rect3D(corners), GeometryObject(kGeomRect)
{
  AddFlags(kGFWire);
  SetColor(0.0,1.0,1.0);
  Update();
}
FrameRect3D::FrameRect3D(const Point3D& topLeft, const Point3D& topRight,
                         const Point3D& bottomRight, const Point3D& bottomLeft) :
          Rect3D(topLeft, topRight, bottomRight, bottomLeft),
          GeometryObject(kGeomRect)
{
  AddFlags(kGFWire);
  SetColor(0.0,1.0,1.0);
  Update();
}
//
//  Conversion constructors.
//
FrameRect3D::FrameRect3D(const Rect3D& r) :
    Rect3D(r), GeometryObject(kGeomRect)
{
  
}
//
//  Copy constructor and assignment operator.
//
FrameRect3D::FrameRect3D(const FrameRect3D& f) :
      Rect3D(f.mCorners), GeometryObject(kGeomRect)
{
  
}
FrameRect3D& FrameRect3D::operator=(const FrameRect3D& f)
{
  assert(false);
  //
  //  I have not figured out how to do this yet!!!!
  //
  return *this;
}
//
//	All geometries must override WriteToFile.
//
bool FrameRect3D::WriteToFile(FILE* ofp)
{
  return false;
}
//
//	We override Draw for debugging purposes at the moment.
//
void FrameRect3D::Draw()
{
  mList.CallList();
}
//
//	and the Update routine that translates the geometry into GL.
//
#ifdef UseOpenGL
void FrameRect3D::Update()
{
//  char mBuff[64];
	mList.Start();
	if (mFlags & kGFSolid) {	// Solid Box
		RunMaterial();;
		
		glBegin(GL_QUADS);
		glVertex3dv(mCorners[0].mCoords);
		glVertex3dv(mCorners[1].mCoords);
		glVertex3dv(mCorners[2].mCoords);
		glVertex3dv(mCorners[3].mCoords);
		glEnd();
	}
	
	if (mFlags & kGFWire) { // Wire Frame
    mColor.Draw();

		glBegin(GL_LINE_LOOP);
		glVertex3dv(mCorners[0].mCoords);
		glVertex3dv(mCorners[1].mCoords);
		glVertex3dv(mCorners[2].mCoords);
		glVertex3dv(mCorners[3].mCoords);
		glEnd();
/*
    for (int i = 0; i < 4; i++) {
      sprintf(mBuff, "C[%d] = <%f, %f, %f>\n",i,mCorners[i].mX, mCorners[i].mY, mCorners[i].mZ);
      wxLogMessage(mBuff);
    }
    
		glBegin(GL_LINE_LOOP);
		glVertex3f(mCorners[0].mX, mCorners[0].mY, mCorners[0].mZ);
		glVertex3f(mCorners[1].mX, mCorners[1].mY, mCorners[1].mZ);
		glVertex3f(mCorners[2].mX, mCorners[2].mY, mCorners[2].mZ);
		glVertex3f(mCorners[3].mX, mCorners[3].mY, mCorners[3].mZ);
		glEnd();
    
  glBegin(GL_LINE_LOOP);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
		glEnd();

    glBegin(GL_LINES);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glEnd();
*/
	}
	mList.End();
}
#endif
