//
//  ColorMapper.h
//  FieldViewer
//
//  A ColorMapper is used to convert a floating point
//  number in -1.0->1.0 into a colour for display by OpenGL.
//  A bare colour mapper provides a very simple monochrome
//  mapping. Sub-classes may provide more complex mappings.
//
//  Created by Brian Collett on 3/10/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewer__ColorMapper__
#define __FieldViewer__ColorMapper__

#include <iostream>
#include "Geometry/Geometry3d.h"

class ColorMapper {
protected:
  //
  //  Instance vars.
  //
public:
  //
  //  ctors
  //
  ColorMapper() {};
  virtual ~ColorMapper() {};
  //
  //  Key function is passed a field value and returns
  //  a color triplet.
  //
  virtual RGBColour Map(double val);
protected:
  //
  //  Helper called when any parameter changes.
  //
  virtual void Update(void) {};
};

#endif /* defined(__FieldViewer__ColorMapper__) */
