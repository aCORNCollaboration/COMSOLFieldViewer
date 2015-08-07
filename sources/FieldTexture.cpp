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
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "FieldViewerApp.h"
#include "assert.h"
#include "Geometry/GeometricObjects.h"
#include "FieldTexture.h"

//
//  ctors
//
FieldTexture::FieldTexture(int width , int height, double* f)
{
  mWidth = 2;
  mHeight = 5;
  mWidth = width;
  mHeight = height;
  mCMap = nullptr;
  mFMap = nullptr;
  mFMin = 0.0;
  mFMax = 0.0;
  mField = nullptr;
  mTData = nullptr;
  mValid = false;
  glGenTextures(1, &mTexName);
  if ((mWidth * mHeight > 0) && (f != nullptr)) {
    InstallField(f);
  }
}
FieldTexture::~FieldTexture()
{
  if (mCMap != nullptr) {
    delete mCMap;
  }
  if (mFMap != nullptr) {
    delete mFMap;
  }
  if (mTData != nullptr) {
    delete mTData;
  }
  if (mField != nullptr) {
    delete mField;
  }
}
//
//  Install maps and field.
//  Note that we own the field but not the maps.
//
void FieldTexture::InstallCMap(ColorMapper* map)
{
  assert(map != nullptr);
  mCMap = map;
}
void FieldTexture::InstallFMap(FieldMapper* map)
{
  assert(map != nullptr);
  mFMap = map;
}
void FieldTexture::InstallField(double* f)
{
  int size = mWidth * mHeight;
  oprintf("%d wide by %d high\n", mWidth, mHeight);
  int i;
//  int j;
  assert(size > 0);
  assert(f != nullptr);
  assert(mCMap != nullptr);
  assert(mFMap != nullptr);
  if (mTData != nullptr) {
    delete mTData;
  }
  mTData = new RGBColour[size];
  if (mField != nullptr) {
    delete mField;
  }
  mField = f;
//
  mFMin = FLT_MAX;
  mFMax = -FLT_MAX;
  for (i = 0; i < size; ++i) {
    float v = f[i];
    if (v < mFMin) {
      mFMin = v;
    }
    if (v > mFMax) {
      mFMax = v;
    }
  }
  Update();
}
//
//  Helper should be called any time maps change.
//
void FieldTexture::Update(void)
{
  int size = mWidth * mHeight;
//  double scale = 2.0 / double(size);
  for (int i = 0; i < size; i++) {
    double v = mField[i];
    v = mFMap->Map(v);
    RGBColour col = mCMap->Map(v);
    mTData[i] = col;
//    mTData[i] = mCMap->Map(mFMap->Map(mField[i]));
//    mTData[i] = mCMap->Map(scale * double(i) - 1.0);
  }
  Call(glBindTexture(GL_TEXTURE_2D, mTexName));
  Call(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR)); // Linear Filtering
  Call(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR)); // Linear Filtering
//  Call(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST)); // Linear Filtering
  Call(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  Call(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                    mWidth, mHeight,
                    0, GL_RGB, GL_FLOAT, mTData));
}
//
//  Point mapped display removed 7/9/14
//
  //    mTData[i] = mCMap->Map(f[i]);
  //    mTData[i] = RGBColour(float(i)/float(size), float(i)/float(size), float(i)/float(size));
/*
 i = 1;
 for (j = 1; j <= mHeight; j++) {
 float rval =float(j)/float(mHeight);
 //    iprintf("%d %f\n",i, rval);
 mTData[(mHeight - j)*mWidth+(mWidth-i)] = RGBColour(rval, 0.0f, 0.0f);
 }
 for (i = 1; i <= mWidth; i++) {
 mTData[mWidth-i] = RGBColour(0.0f, float(i)/float(mWidth), 0.0f);
 }
 */
/*    int negative = f[i] < 0.0;
 float v = log(fabs(f[i]));
 if (v < 0.0) v = 0.0;
 float cv = fmax * v / lmax;
 mTData[i] = (negative) ? mCMap->Map(cv) : mCMap->Map(-cv); */


