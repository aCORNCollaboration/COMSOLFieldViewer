//
//  LinFieldMapper.cpp
//  FieldViewerc
//
//  Created by Brian Collett on 7/8/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#include "LinFieldMapper.h"
//
//  Key function is passed a field value and returns
//  a color triplet.
//
double LinFieldMapper::Map(double val)
{
  if (isnan(val)) return val;
  double c = 2.0 * (val - mFMin)/(mFMax-mFMin) - 1.0;
  if (c > 1.0) c = 1.0;
  if (c < -1.0) c = -1.0;
  if (val == 0.0) c = 0.0;
  return c;
}
