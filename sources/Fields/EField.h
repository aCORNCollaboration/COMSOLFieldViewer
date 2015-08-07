//
//  EField.h
//  FieldViewer
//
//  This is a general representation of an electric field that can
//  be put into a list of such fields. It is defined within a
//  rectangular parallelipiped and has methods both to access the
//  data and to manipulate the list.
//  In order to allow the use of the base class as start and
//  end anchors for a double-linked list of EFields this is NOT
//  and abstract class. However, it implements only a trivial
//  field operation.
//
//  Modified BCollett 2/26/14 when I divorced the ides of a listable
//  from the idea of a field.
//  Created by Brian Collett on 2/24/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewer__EField__
#define __FieldViewer__EField__

#include "../Geometry/GeometricObjects.h"
#include "Listable.h"

class EField : public Listable {
protected:
  //
  //  Instance Vars
  //  A field must have a bounding box. Any real field will probably add
  //  some kind of information store.
  //
  Box3D mBounds;
  //
  //  Class var.
  //
  static char sNoName[];
public:
  //
  //  ctors
  //
  EField();
  virtual ~EField();
  //
  //  Field operations.
  //
  virtual Box3D* GetBounds() { return &mBounds; };
  virtual Vector3D FieldAt(const Vector3D& p) const;
  virtual Vector3D FieldAt(const Point3D& p) const;
  //
  //  And ones for names.
  //
  virtual const char* FieldNameAt(const Vector3D& p) const;
  virtual const char* FieldNameAt(const Point3D& p) const;
};

#endif /* defined(__FieldViewer__EField__) */
