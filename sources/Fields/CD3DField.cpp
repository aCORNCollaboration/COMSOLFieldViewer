//
//  CD3DField.cpp
//  FieldViewer
//
//  This is a field built on a 3D COMSOLData interpolated grid
//  field.
//
//  Created by Brian Collett on 2/24/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#include "CD3DField.h"
#include "COMSOLData3D.h"
//
//  ctors
//
CD3DField::CD3DField(CD3Data* d) : EField()
{
  mData = d;  // And now it is ours
  //
  //  Build bounding box.
  //
  mBounds.Set(Point3D(mData->mMin), Point3D(mData->mMax));
  mBounds.AddFlags(kGFWire);
  mBounds.SetColor(1.0, 1.0, 1.0);
}
CD3DField::~CD3DField()
{
  delete mData;
}
//
//  Override.
//  Field operations.
//
Vector3D CD3DField::FieldAt(const Vector3D& p) const
{
  double field[3] = { NAN, NAN, NAN };
  CD3GetEAtPoint(mData, &p.mCoords[0], field);
  return Vector3D(field);
}
Vector3D CD3DField::FieldAt(const Point3D& p) const
{
  double field[3] = { NAN, NAN, NAN };
  CD3GetEAtPoint(mData, &p.mCoords[0], field);
  return Vector3D(field);
}
//
//  Add ones for names.
//
const char* CD3DField::FieldNameAt(const Vector3D& p) const
{
  return CD3GetNameAtPoint(mData, &p.mCoords[0]);
}

const char* CD3DField::FieldNameAt(const Point3D& p) const
{
  return CD3GetNameAtPoint(mData, &p.mCoords[0]);
}

