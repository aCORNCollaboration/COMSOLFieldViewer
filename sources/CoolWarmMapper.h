//
//  CoolWarmMapper.h
//  FieldViewer
//
//  A CoolWarmMapper is a kind of ColorMapper that maps field
//  values onto a sort of rainbow. Positive values map towards
//  the red and negative ones toward blue.
//
//  Created by Brian Collett on 3/10/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewer__CoolWarmMapper__
#define __FieldViewer__CoolWarmMapper__

#include "ColorMapper.h"

class CoolWarmMapper : public ColorMapper {
protected:
  static GLfloat sTable[];
public:
  //
  //  ctors
  //
  CoolWarmMapper(int n) : ColorMapper() {};
  virtual ~CoolWarmMapper() {};
  //
  //  Key function is passed a field value and returns
  //  a color triplet.
  //
  virtual RGBColour Map(double val);
protected:
};

#endif /* defined(__FieldViewer__CoolWarmMapper__) */
