//
//  CD3DField.h
//  FieldViewer
//
//  This is a field built on a 3D COMSOLData interpolated grid
//  field.
//
//  Created by Brian Collett on 2/24/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewer__CD3DField__
#define __FieldViewer__CD3DField__

#include "Listable.h"
#include "EField.h"
#include "COMSOLData3D.h"

class CD3DField : public EField {
public:
  //
  //  We add a CD3Data member to hold the field info.
  //
  CD3Data *mData;
public:
  //
  //  ctors
  //
  CD3DField(CD3Data* d);
  virtual ~CD3DField();
  //
  //  Override.
  //  Field operations.
  //
  virtual Vector3D FieldAt(const Vector3D& p) const;
  virtual Vector3D FieldAt(const Point3D& p) const;
  //
  //  And ones for names.
  //
  virtual const char* FieldNameAt(const Vector3D& p) const;
  virtual const char* FieldNameAt(const Point3D& p) const;
};

#endif /* defined(__FieldViewer__CD3DField__) */
