//
//  FieldView.h
//  FieldViewer
//
//  A FIeldView is a representation of field that can display itself
//  in a GLContext. As such it IS a geometric object. In order to
//  store a list of field views in the document it is also a Listable.
//
//  Created by Brian Collett on 2/27/14.
//  BCollett 3/18/14 Add planes explicitly parallel to z.
//  BCollett 7/4/14 Now textures work. To get size info connect
//  FieldView to Canvas.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewer__FieldView__
#define __FieldViewer__FieldView__

#include <stdio.h>
#include "FieldViewerDoc.h"
#include "Geometry/GeometricObjects.h"
#include "Listable.h"
#include "GLViewerCanvas.h"
#include "EField.h"
#include "FieldTexture.h"

class FieldView : public Listable, public GeometryObject {
public:
  //
  //  Instance vars.
  //  We keep refs (which we do NOT own) to the field we are displaying,
  //  to the GLCanas in which we are displayed, and to the document that
  //  owns us.
  //
  EField* mField;
  GLViewerCanvas* mCanvas;
  FieldViewerDoc* mDoc;
  //
  //  We render into a FrameRect.
  //
  FrameRect3D* mFrame;
  //
  //  Using a FieldTexture.
  //
  FieldTexture* mTex;
  //
  //  And use another pair for the legend.
  //
  FrameRect3D* mLegendFrame;
  FieldTexture* mLTex;
  //
  //  This lets us know when all our structure is in place.
  //
  bool mValid;
  bool mFixRange;
  double* mFData;
  double mFMin, mFMax;
  double mFRange;
  Point3D* mPData;
  int mNAcross, mNDown;
  //
  //  ctors
  //
  FieldView(GLViewerCanvas* theCanvas, EField* f, FieldViewerDoc* d);
  virtual ~FieldView();
  //
  //  Use this to specify the fiew plane.
  //  It will return 0 for success and some kind of error code if it fails.
  //  It can fail if the view plane is not oriented so that it contains
  //  at least one of the axes or if it does not intersect the field at all.
  //
  int ViewPlane(Point3D& p, Vector3D& v);
  //
  //  This one works for planes that are explicitly parallel to the z
  //  axis and are bounded in the z direction.
  //
  int ViewPlane(Point3D& p, double theta, double zMin, double zMax);
  //
  //  This tells us what kind of view to become and what spacing to use.
  //
  void ViewType(int type, double spacing);
  //
  //  This allows the viewer to set the data range instead of inferring
  //  it.
  //
  void SetDataRange(double vMin, double vMax);
	//
	//	We override Draw so we can tell our FrameRect to draw.
	//
	virtual void Draw();
  //
  //  Override Update.
  //
  virtual void Update();
  //
  //  All geometries must override WriteToFile.
  //
  bool WriteToFile(FILE* ofp);
  //
  //  Helper.
  //
  bool Intersect(const Point3D&, const Vector3D&,
                 const Point3D&, const Point3D&, Point3D&);
};

#endif /* defined(__FieldViewer__FieldView__) */
