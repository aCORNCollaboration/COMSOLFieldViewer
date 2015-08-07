//
//  LogFieldMapper.h
//  FieldViewerc
//
//  Created by Brian Collett on 7/8/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewerc__LogFieldMapper__
#define __FieldViewerc__LogFieldMapper__

#include "FieldMapper.h"

class LogFieldMapper : public FieldMapper {
protected:
  //
  //  Instance var is log of max value.
  //
  double mLMax;
public:
  //
  //  ctors
  //
  LogFieldMapper(double min, double max = 0.0) :
    FieldMapper(min, max) { Update(); };
  LogFieldMapper(double lims[2]) : FieldMapper(lims) { Update(); };
  virtual ~LogFieldMapper() {};
  //
  //  Key function is passed a field value and returns
  //  a mapped value.
  //  Note that the mapper is expected to coerce the output
  //  into -1.0->1.0, clipping to the bounds.
  //
  virtual double Map(double val);
protected:
  //
  //  Helper called when any parameter changes.
  //
  virtual void Update(void);
};
#endif /* defined(__FieldViewerc__LogFieldMapper__) */
