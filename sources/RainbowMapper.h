//
//  RainbowMapper.h
//  FieldViewer
//
//  A RainbowMapper is a kind of ColorMapper that maps field
//  values onto a sort of rainbow. Positive values map towards
//  the red and negative ones toward blue.
//
//  Created by Brian Collett on 3/10/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewer__RainbowMapper__
#define __FieldViewer__RainbowMapper__

#include "ColorMapper.h"

class RainbowMapper : public ColorMapper {
protected:
  //
  //  Store the largest magnitude diversion from 0.
  //
  double mRange;
  //
  //  Number of colour cycles we use.
  //
  int mNCycle;
public:
  //
  //  ctors
  //
  RainbowMapper(int n) : mNCycle(n), mRange(2.0), ColorMapper() {};
  virtual ~RainbowMapper() {};
  //
  //  Key function is passed a field value and returns
  //  a color triplet.
  //
  virtual RGBColour Map(double val);
protected:
  //
  //  Helper called when any parameter changes.
  //
  virtual void Update(void);

};

#endif /* defined(__FieldViewer__RainbowMapper__) */
