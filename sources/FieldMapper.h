//
//  FieldMapper.h
//  FieldViewer
//
//  A FieldMapper is used to convert a floating point
//  number in the domain FMin to FMax onto the -1.0->1.0 range.
//  This is an abstract class. A real class must provide
//  the mapping.
//
//  Created by Brian Collett on 7/8/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewer__FieldMapper__
#define __FieldViewer__FieldMapper__

#include "math.h"
#include "Geometry/Geometry3d.h"

class FieldMapper {
protected:
  //
  //  Instance vars.
  //  Max and min values.
  //
  double mFMin;
  double mFMax;
public:
  //
  //  ctors
  //
  FieldMapper(double max, double min = 0.0);
  FieldMapper(double lims[2]);
  virtual ~FieldMapper() {};
  //
  //  Alter mapping.
  //
  virtual double SetMax(double newMax);
  virtual double SetMin(double newMin);
  //
  //  Key function is passed a field value and returns
  //  a mapped value.
  //  Note that the mapper is expected to coerce the output
  //  into -1.0->1.0, clipping to the bounds.
  //
  virtual double Map(double val) = 0;
protected:
  //
  //  Helper called when any parameter changes.
  //
  virtual void Update(void) {};
};

#endif /* defined(__FieldViewer__FieldMapper__) */
