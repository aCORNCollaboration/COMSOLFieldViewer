//
//  Rect3D.cpp
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

#include "Geometry3d.h"
//
//  Tolerance for angle test.
//  Default is cos(10mRad);
//
Real Rect3D::cCAngTol = cos(0.01);
//
//  ctors
//
Rect3D::Rect3D(const Point3D* corners)
{
  char mBuff[64];
  //
  //  If we didn't get any data then we can't be valid!
  //
  if (nullptr == corners) {
    mValid = false;
    return;
  }
  //
  //  Copying the corners is easy. The trick comes in verifying
  //  that they describe a flat rectangle.
  //
  for (int i = 0; i < 4; i++) {
    mCorners[i] = corners[i];
    sprintf(mBuff, "In Rect3D C[%d] = <%f, %f, %f>\n",i,mCorners[i].mX, mCorners[i].mY, mCorners[i].mZ);
    wxLogMessage(mBuff);
  }
  mValid = Validate();
}
Rect3D::Rect3D(const Point3D& topLeft, const Point3D& topRight,
       const Point3D& bottomRight, const Point3D& bottomLeft)
{
  mCorners[0] = topLeft;
  mCorners[1] = topRight;
  mCorners[2] = bottomRight;
  mCorners[3] = bottomLeft;
  mValid = Validate();
}
//
//  Copy constructor and assignment operator.
//
Rect3D::Rect3D(const Rect3D& f)
{
  int i;
  //
  //  Copying the corners is easy. The trick comes in verifying
  //  that they describe a flat rectangle.
  //
  for (i = 0; i < 4; i++) {
    mCorners[i] = f.mCorners[i];
  }
  mValid = Validate();
}
Rect3D& Rect3D::operator=(const Rect3D& f)
{
  int i;
  //
  //  Copying the corners is easy. The trick comes in verifying
  //  that they describe a flat rectangle.
  //
  for (i = 0; i < 4; i++) {
    mCorners[i] = f.mCorners[i];
  }
  mValid = Validate();
  return *this;
}
//
//  Accessors
//
Vector3D Rect3D::GetNormal() const
{
  return mHoriz.Cross(mVert);
}
//
//  This allows us to index into the rect as if it were a 2D surface
//  runing from 0->Width in one direction and 0->height in the other.
//
Point3D Rect3D::Map2D(double x, double y)
{
  Point3D p = mCorners[3] + mHoriz * (x / mHoriz.Length());
  return p + mVert * (y / mVert.Length());
}

//
//  Internal helpers.
//  This checks that the four corners define a planar rectangle.
//
bool Rect3D::Validate()
{
  //
  //  Make sure that top and bottom edges are parallel and same length
  //  (to 1 degree and 1%).
  //
  mHoriz = BottomRight() - BottomLeft();
  Vector3D top(TopRight() - TopLeft());
  if (!(mHoriz == top)) {
    return false;     // Leaves object invalid, top and bottom not parallel.
  }
  //
  //  Same for left and right edges.
  //
  mVert = TopLeft() - BottomLeft();
  Vector3D right(TopRight() - BottomRight());
  if (!(mVert == right)) {
    return false;     // Leaves object invalid, left and right not parallel.
  }
  //
  //  Lastly make sure perpendicular.
  //
  if (fabs(mHoriz.Dot(mVert.Norm())/mHoriz.Length()) > cCAngTol) {
    return false;   // Left invalid because not perpendicular
  }
  return true;
}


