//
//  LogFieldMapper.cpp
//  FieldViewerc
//
//  Created by Brian Collett on 7/8/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//
#include <math.h>
#include "LogFieldMapper.h"
//
//  Key function is passed a value and returns a value
//  in range -1.0->1.0.
//  The conversion is a strange one. It extracts the sign
//  since the log is not defined for negative numbers.
//
double LogFieldMapper::Map(double val)
{
  if ((val == 0.0) || isnan(val)) return val;
  int negative = val < 0.0;
  float v = log(fabs(val));
  if (v < 0.0) v = 0.0;
  return (negative) ? -v / mLMax : v / mLMax;
}
//
//  Update has to reset mLMax.
//
void LogFieldMapper::Update()
{
  double max = fabs(mFMax);
  double min = fabs(mFMin);
  mLMax = (max >= min) ? log(max) : log(min);
}