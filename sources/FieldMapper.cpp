//
//  FieldMapper.cpp
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

#include "FieldMapper.h"

//
//  ctors
//
FieldMapper::FieldMapper(double min, double max)
{
  mFMin = min;
  mFMax = max;
  Update();
}
FieldMapper::FieldMapper(double* lims)
{
  mFMin = lims[0];
  mFMax = lims[1];
  Update();
}
//
//  Methods to alter limits.
//
double FieldMapper::SetMax(double newMax)
{
  double old = mFMax;
  mFMax = newMax;
  Update();
  return old;
}

double FieldMapper::SetMin(double newMin)
{
  double old = mFMin;
  mFMin = newMin;
  Update();
  return old;
}



