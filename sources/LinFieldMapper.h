//
//  LinFieldMapper.h
//  FieldViewerc
//
//  Created by Brian Collett on 7/8/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewerc__LinFieldMapper__
#define __FieldViewerc__LinFieldMapper__

#include "FieldMapper.h"

class LinFieldMapper : public FieldMapper {
public:
  //
  //  ctors
  //
  LinFieldMapper(double min, double max = 0.0) :
    FieldMapper(min, max) {};
  LinFieldMapper(double lims[2]) : FieldMapper(lims) {};
  virtual ~LinFieldMapper() {};
  //
  //  Key function is passed a field value and returns
  //  a mapped value.
  //  Note that the mapper is expected to coerce the output
  //  into -1.0->1.0, clipping to the bounds.
  //
  virtual double Map(double val);
};
#endif /* defined(__FieldViewerc__LinFieldMapper__) */
