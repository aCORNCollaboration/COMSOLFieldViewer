//
//  FieldView.cpp
//  FieldViewer
//
//  A FieldView is a representation of field that can display itself
//  in a GLContext. As such it IS a geometric object. In order to
//  store a list of field views in the document it is also a Listable.
//
//  Created by Brian Collett on 2/27/14.
//  BCollett 3/18/14 Add planes explicitly parallel to z.
//  BCollett 7/4/14 Have textures working properly. Connect to
//  canvas so can get info about size of texture needed.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <math.h>
#include <float.h>

#include "FieldViewerDoc.h"
#include "FieldView.h"
#include "RainbowMapper.h"
#include "CoolWarmMapper.h"
#include "LinFieldMapper.h"
#include "LogFieldMapper.h"
//
//  ctors
//
FieldView::FieldView(GLViewerCanvas* theCanvas, EField* f, FieldViewerDoc* d) :
            Listable(), GeometryObject(kGeomView)
{
  mCanvas = theCanvas;
  mField = f;
  mDoc = d;
  mFrame = nullptr;
  mValid = false;
  mFixRange = false;
  mTex = nullptr;
}

FieldView::~FieldView()
{
  if (nullptr != mFrame) {
    delete mFrame;
  }
}

//
//  Use this to specify the view plane.
//  It will return 0 for success and some kind of error code if it fails.
//  It can fail if the view plane is not oriented so that it contains
//  at least one of the axes or if it does not intersect the field at all.
//
int FieldView::ViewPlane(Point3D& p, Vector3D& v)
{
  int dir;    // Direction number, x = 0, y = 1, z = 2.
  int dirn,dirnn;
  //
  //  We have to find the points of intersection of the specified plane
  //  with the bounding box of the Field.
  //  The simple (if very long winded) method that I use here relies on
  //  the facts that the bounding box is oriented parallel to the axes
  //  and that at least one of the axes lies in the view plane. This allows
  //  us to figure out immediately which edges of the bounding box are
  //  transected by the view plane.
  //
  //  Start by finding an axis that lies in the plane. That is easy because
  //  if an axis lies in the plane then that component of the normal vector
  //  will be zero. So we seek the first zero component.
  //
  for (dir = 0; dir < 3; dir++) {
    if (v.mCoords[dir] == 0.0) {
      break;
    }
  }
  if (dir > 2) {
    return 1;    //  NO axis lies in the plane. We have failed.
  }
  //
  //  Now that we know which direction is key we construct the other
  //  two in order.
  //
  dirn = (dir + 1) % 3;
  dirnn = (dir + 2) % 3;
  //
  //  Now that we know which real axis vector lies in the plane we know
  //  that the plane must intersect the two faces perpendicular to that
  //  direction.
  //  We need to test the edges of one of those two faces in order. We
  //  stop when we have found two. We start by constructing the vertices
  //  in order.
  //
  const Real* min = mField->GetBounds()->GetMin().mCoords;
  const Real* max = mField->GetBounds()->GetMax().mCoords;
  Point3D corner[4];
  for (int i = 0; i < 4; i++) {
    corner[i].mCoords[dir] = min[dir];
    corner[i].mCoords[dirn] = (((i/2)&1)==0) ? min[dirn] : max[dirn];
    corner[i].mCoords[dirnn] = ((((i+1)/2)&1)==0) ? min[dirnn] : max[dirnn];
  }
  int nIntersection = 0;
  Point3D frameCorner[4];
  for (int edge = 0; edge < 4; edge++) {
    if (Intersect(p, v,
                  corner[edge], corner[(edge+1)%4],
                  frameCorner[nIntersection])) {
      bool used = false;
      for (int e = 0; e < nIntersection; e++) {
        if (frameCorner[nIntersection] == frameCorner[e]) {
          used = true;
        }
      }
      if (!used) {
        nIntersection++;
        if (nIntersection >= 2) break;
      }
    }
  }
  if (nIntersection != 2) {
    return -1;        // MUST have two intersections.
  }
  //
  //  Right, we have two intersections. They are on the min side in the
  //  dir direction. The other two intersections are on the max side
  //  so we make them from these two. The order is chosen carefully
  //  to make corners adjacent.
  //
  frameCorner[2] = frameCorner[1];
  frameCorner[2].mCoords[dir] = max[dir];
  frameCorner[3] = frameCorner[0];
  frameCorner[3].mCoords[dir] = max[dir];
  //
  //  Now we can construct the FrameRect3D representing the intersection.
  //
  mFrame = new FrameRect3D(frameCorner);
  if (mFrame->IsValid()) {
    mValid = true;
  }
  return 0;
}
//
//  This one works for planes that are explicitly parallel to the z
//  axis and are bounded in the z direction.
//  This time we get ourselves a square at zMin and search that
//  for our intersections.
//
int FieldView::ViewPlane(Point3D& p, double theta, double zMin, double zMax)
{
  const double degree = 3.141592653589 / 180.0;
  //
  //  We know <0,0,1> lies in the plane, figure out what else
  //  does from the angle.
  //
  Vector3D norm(cos(theta * degree), sin(theta * degree), 0.0);
  const Real* min = mField->GetBounds()->GetMin().mCoords;
  const Real* max = mField->GetBounds()->GetMax().mCoords;
  Point3D corner[4];
  for (int i = 0; i < 4; i++) {
    corner[i].mCoords[0] = (((i/2)&1)==0) ? min[0] : max[0];
    corner[i].mCoords[1] = ((((i+1)/2)&1)==0) ? min[1] : max[1];
    corner[i].mCoords[2] = zMin;
  }
  int nIntersection = 0;
  Point3D frameCorner[4];
  for (int edge = 0; edge < 4; edge++) {
    if (Intersect(p, norm,
                  corner[edge], corner[(edge+1)%4],
                  frameCorner[nIntersection])) {
      bool used = false;
      for (int e = 0; e < nIntersection; e++) {
        if (frameCorner[nIntersection] == frameCorner[e]) {
          used = true;
        }
      }
      if (!used) {
        nIntersection++;
        if (nIntersection >= 2) break;
      }
    }
  }
  if (nIntersection != 2) {
    return -1;        // MUST have two intersections.
  }
  //
  //  Right, we have two intersections. They are on the min side in the
  //  dir direction. The other two intersections are on the max side
  //  so we make them from these two. The order is chosen carefully
  //  to make corners adjacent.
  //
  frameCorner[2] = frameCorner[1];
  frameCorner[2].mCoords[2] = zMax;
  frameCorner[3] = frameCorner[0];
  frameCorner[3].mCoords[2] = zMax;
  //
  //  Now we can construct the FrameRect3D representing the intersection.
  //
  mFrame = new FrameRect3D(frameCorner);
  if (mFrame->IsValid()) {
    mValid = true;
  }
  //
  //  Then we can construct the legend. It sits in the same plane as
  //  the mFrame but is thinner and translated by 10% of the mFrame width.
  //
  Vector3D horiz = mFrame->GetHorizontal();
  horiz.Normalize();
  double displacement = mFrame->GetWidth() * 0.1;
  double legendWidth = displacement * 1.3; // 3% of frame
  frameCorner[0] = frameCorner[1] + horiz * displacement;
  frameCorner[1] = frameCorner[0] + horiz * legendWidth;
  frameCorner[3] = frameCorner[2] + horiz * displacement;
  frameCorner[2] = frameCorner[3] + horiz * legendWidth;
  mLegendFrame = new FrameRect3D(frameCorner);
  return 0;
}

//
//  This tells us what kind of view to become and what spacing to use.
//
void FieldView::ViewType(int type, double spacing)
{
//  static char mBuff[128];
  if (mFrame->IsValid()) {
    //
    //  Start by figuring out how big an aray we will need to hold the view.
    //  At this point we will only be pulling the data out of the field and
    //  not color mapping it.
    //  Figure out map dimensions by projecting the frame into the canvas.
    //
    mNDown = (int) floor(mCanvas->Project(mFrame->GetHeight()));
    if (mNDown < 1) {
      mNDown = (int) floor(mCanvas->Project(mFrame->GetHeight()));
    }
    mNAcross = (int) floor(mCanvas->Project(mFrame->GetWidth()));
    mPData = new Point3D[mNAcross * mNDown];
    mFData = new double[mNAcross * mNDown];
    if (!mFixRange) {
      mFMin = DBL_MAX;
      mFMax = -DBL_MAX;
    }
    double width = mFrame->GetWidth();
    spacing = width / mNAcross;
    int i,j;
    double x,y,v;
    for (j = 0; j < mNDown; j++) {
      y = (j + 0.5) * spacing;
      for (i = 0; i < mNAcross; i++, x = ((i + 0.5) * spacing)) {
        Point3D p = mFrame->Map2D(x, y);
//        printf("<%f,%f>->[%f,%f,%f]\n",x,y,
//               p.mX, p.mY, p.mZ);
        Vector3D f = dynamic_cast<EField*>(mField)->FieldAt(p);
        mPData[(int)(j * mNAcross + i)] = p;
        switch (type) {
          case 0:
            v = f.mX;
            break;
            
          case 1:
            v = f.mY;
            break;
            
          case 2:
            v = f.mZ;
            break;
            
          case 3:
            v = f.Length();
            break;
            
          default:
            break;
        }
        mFData[(int)(j * mNAcross + i)] = v;
        if (!mFixRange) {
          if (v < mFMin) mFMin = v;
          if (v > mFMax) mFMax = v;
        }
      }
    }
    eprintf("fmin=%f, fmax=%f\n", mFMin, mFMax);
    //
    //  And then build a texture to put in it.
    //
    assert(mNAcross > 0);
    assert(mNDown > 0);
    mTex = new FieldTexture(mNAcross, mNDown);
    assert(nullptr != mTex);
    FieldMapper* fm;
    if (mDoc->IsLinear()) {
      fm = new LinFieldMapper(mFMin, mFMax);
    } else {
      fm = new LogFieldMapper(mFMin, mFMax);
    }
//    ColorMapper* cm = new RainbowMapper(1);
    ColorMapper* cm;
    if (mDoc->GetNColorCycle() == 1) {
      cm = new CoolWarmMapper(1);
    } else if (mDoc->GetNColorCycle() == 2) {
      cm = new RainbowMapper(1);
    } else {
      cm = new ColorMapper();
    }
    mTex->InstallCMap(cm);
    mTex->InstallFMap(fm);
    mTex->InstallField(mFData);
    //
    //  Texture for the legend.
    //
    double lData[400];
    double max = (fabs(mFMax) > fabs(mFMin)) ? fabs(mFMax) : fabs(mFMin);
    for (j = 0; j < 200; j++) {
      y = max * double(j - 100) / 100.0;
      for (i = 0; i < 2; i++) {
        lData[(int)(j * 2 + i)] = y;
      }
    }
    mLTex = new FieldTexture(2, 200);
    if (mDoc->IsLinear()) {
      fm = new LinFieldMapper(-max, max);
    } else {
      fm = new LogFieldMapper(-max, max);
    }
    if (mDoc->GetNColorCycle() == 1) {
      cm = new CoolWarmMapper(1);
    } else if (mDoc->GetNColorCycle() == 2) {
      cm = new RainbowMapper(1);
    } else {
      cm = new ColorMapper();
    }
    mLTex->InstallCMap(cm);
    mLTex->InstallFMap(fm);
    mLTex->InstallField(lData);
  }
}

//
//	We override Draw so we can tell our FrameRect to draw.
//
void FieldView::Draw()
{
  if (mValid) {
    //
    //  Frame the rectangle varying color by select state.
    //
    if (mSelected) {
      mFrame->SetColor(1.0f, 1.0f, 1.0f);
    } else {
      mFrame->SetColor(0.2f, 0.2f, 0.2f);
    }
    mFrame->Draw();
    //
    //  Draw our textured rectangle.
    //
/*    iprintf("Coloring field with texture %d\n", mTex->Name());
    Call(glPolygonOffset(1.0f, 1.0f));
    Call(glEnable(GL_TEXTURE_2D));
    Call(glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));
    Call(glBindTexture(GL_TEXTURE_2D, mTex->Name()));
    Call(glBegin(GL_QUADS));
      Call(glTexCoord2f(0.0f, 0.0f));
      Call(glVertex3dv(mFrame->BottomLeft().mCoords));
      Call(glTexCoord2f(1.0f, 0.0f));
      Call(glVertex3dv(mFrame->BottomRight().mCoords));
      Call(glTexCoord2f(1.0f, 1.0f));
      Call(glVertex3dv(mFrame->TopRight().mCoords));
      Call(glTexCoord2f(0.0f, 1.0f));
      Call(glVertex3dv(mFrame->TopLeft().mCoords)); */
    Call(glPolygonOffset(1.0f, 1.0f));
    Call(glEnable(GL_TEXTURE_2D));
    Call(glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));
    Call(glBindTexture(GL_TEXTURE_2D, mTex->Name()));
    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f); glVertex3dv(mFrame->BottomLeft().mCoords);
      glTexCoord2f(1.0f, 0.0f); glVertex3dv(mFrame->BottomRight().mCoords);
      glTexCoord2f(1.0f, 1.0f); glVertex3dv(mFrame->TopRight().mCoords);
      glTexCoord2f(0.0f, 1.0f); glVertex3dv(mFrame->TopLeft().mCoords);
    Call(glEnd());
    Call(glDisable(GL_TEXTURE_2D));
    Call(glPolygonOffset(0.0f, 0.0f));
    //
    //  Add the legend.
    //
    mLegendFrame->Draw();
    //
    //  Draw our textured rectangle.
    //
//    iprintf("Coloring legend with texture %d\n", mLTex->Name());
    Call(glEnable(GL_TEXTURE_2D));
    Call(glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));
    Call(glBindTexture(GL_TEXTURE_2D, mLTex->Name()));
    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f); glVertex3dv(mLegendFrame->BottomLeft().mCoords);
      glTexCoord2f(1.0f, 0.0f); glVertex3dv(mLegendFrame->BottomRight().mCoords);
      glTexCoord2f(1.0f, 1.0f); glVertex3dv(mLegendFrame->TopRight().mCoords);
      glTexCoord2f(0.0f, 1.0f); glVertex3dv(mLegendFrame->TopLeft().mCoords);
    Call(glEnd());
    Call(glDisable(GL_TEXTURE_2D));
  }
}
//
//  Override Update.
//
void FieldView::Update()
{
  if (mValid) {
    mFrame->Update();
  }
}
//
//	All geometries must override WriteToFile.
//
bool FieldView::WriteToFile(FILE* ofp)
{
  return false;
}
//
//  Helper.
//  Find the point of intersection of the plane defined by the point p
//  & the normal vector v with the line segment defined by the ordered
//  point pair p1 & p2. If the intersection is within the segment returns
//  true and sets pi to the intersection point.
//
bool FieldView::Intersect(const Point3D& p, const Vector3D& n,
                          const Point3D& p1, const Point3D& p2, Point3D& pi)
{
  Vector3D w = p1 - p;
  Vector3D u = p2 - p1;
  double angle = n.Dot(u);
  if (angle == 0) {
    return false;
  }
  float lambda = -n.Dot(w)/angle;
  if (lambda < 0.0) {
    return false;
  }
  if (lambda > 1.0) {
    return false;
  }
  pi = p1 + u * lambda;
  return true;
}
//
//  Display data as coloured points.
/*
 int i = 0, j = 0;
 RainbowMapper mp(mFMin, mFMax);
 if (mPData != nullptr) {
 glBegin(GL_POINTS);
 for (j = 0; j < mNDown; j++) {
 for (i = 0; i < mNAcross; i++) {
 RGBColour c(mp.Map(mFData[j * mNAcross + i]));
 Call(glColor3d(c.mRed, c.mGreen, c.mBlue));
 Call(glVertex3d(mPData[j * mNAcross + i].mX + 0.1,
 mPData[j * mNAcross + i].mY,
 mPData[j * mNAcross + i].mZ));
 }
 }
 glEnd();
 }
 */
//
//  This allows the viewer to set the data range instead of inferring
//  it.
//
void FieldView::SetDataRange(double vMin, double vMax)
{
  mFMin = vMin;
  mFMax = vMax;
  mFixRange = true;
}

