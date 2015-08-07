//
//  ColorMapper.cpp
//  FieldViewer
//
//  A ColorMapper is used to convert a floating point
//  number into a colour for display by OpenGL.
//  A bare colour mapper provides a very simple monochrome
//  mapping. Sub-classes may provide more complex mappings.
//
//  Created by Brian Collett on 3/10/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#include "ColorMapper.h"

//
//  Key function is passed a field value and returns
//  a color triplet.
//
RGBColour ColorMapper::Map(double val)
{
  double c = 0.5 * (val + 1.0);
  return RGBColour(c, c, c);
}

