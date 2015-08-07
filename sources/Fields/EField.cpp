//
//  EField.cpp
//  FieldViewer
//
//  Created by Brian Collett on 2/24/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//
#include <cmath>
#include "EField.h"

 char EField::sNoName[] = "No Field";
//
//  ctors
//
EField::EField() : mBounds(), Listable()
{
}
EField::~EField()
{
}
//  Field operations.
//
Vector3D EField::FieldAt(const Vector3D& p) const
{
  return Vector3D(nan(""),nan(""),nan(""));
}
Vector3D EField::FieldAt(const Point3D& p) const
{
  return Vector3D(nan(""),nan(""),nan(""));
}
//
//  Andones for names
//
const char* EField::FieldNameAt(const Vector3D& p) const
{
  return sNoName;
}
const char* EField::FieldNameAt(const Point3D& p) const
{
  return sNoName;
}
