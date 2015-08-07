//
//  FieldTexture.h
//  FieldViewer
//
//  A field texture is a texture created from a buffer of
//  field data. It represents a view of a rectangular area
//  of the field.
//  Initial version tries to use non-power-of-2 textures
//  which are supposed to be supported in OpenGL 2.
//
//  Created by Brian Collett on 3/10/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewer__FieldTexture__
#define __FieldViewer__FieldTexture__

#include "ColorMapper.h"
#include "FieldMapper.h"

class FieldTexture {
protected:
  //
  //  Instance vars.
  //  Dimensions of the map and storage for the field and
  //  map data.
  //
  int mWidth;
  int mHeight;
  double* mField;
  RGBColour* mTData;
  //
  //  We need a texture name (a number) from OpenGL.
  //
  GLuint mTexName;
  //
  //  Also have a ColorMapper and FieldMapper.
  //
  ColorMapper* mCMap;
  FieldMapper* mFMap;
  //
  //  Have field value limits for FieldMapper.
  //
  double mFMin;
  double mFMax;
  //
  //  This will become true once all the data are in place.
  //
  bool mValid;
public:
  //
  //  ctors
  //
  FieldTexture(int width = -1, int height = -1, double* f = nullptr);
  virtual ~FieldTexture();
  //
  //  Accessor.
  //
  GLuint Name(void) const { return mTexName; };
  //
  //  Install map and field.
  //
  virtual void InstallCMap(ColorMapper* map);
  virtual void InstallFMap(FieldMapper* map);
  virtual void InstallField(double* f);
protected:
  //
  //  Helper should be called any time maps change.
  //
  virtual void Update(void);
};

#endif /* defined(__FieldViewer__FieldTexture__) */

