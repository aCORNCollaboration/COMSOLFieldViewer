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

#include "RainbowMapper.h"

//
//  Key function is passed a field value and returns
//  a color triplet.
//  Field values have been through a mapper and are guaranteed
//  to be on -1->1 OR NaN.
//
RGBColour RainbowMapper::Map(double val)
{
  float sr, er, sg, eg, sb, eb; // start and end components
  float f;
  //  static double val = -1.0;
  if (isnan(val)) {
    return RGBColour(1.0f, 1.0f, 1.0f);
  } else if (val == 0.0) {
    return RGBColour(0.0f, 0.0f, 0.0f);
  } else if (val < -0.75) {
    f = 4*(val + 1.0);
    sr = 0.5f;
    er = 0.0f;
    sg = 0.0f;
    eg = 0.5f;
    sb = 0.5f;
    eb = 1.0f;
  } else if (val < -0.5) {
    f = 4*(val + 0.75);
    sr = 0.0f;
    er = 0.0f;
    sg = 0.5f;
    eg = 1.0f;
    sb = 1.0f;
    eb = 1.0f;
  } else if (val < -0.25) {
    f = 4*(val + 0.5);
    sr = 0.0f;
    er = 0.0f;
    sg = 1.0f;
    eg = 1.0f;
    sb = 1.0f;
    eb = 0.0f;
  } else if (val < -0.0) {
    f = 4*(val + 0.25);
    sr = 0.0f;
    er = 0.3f;
    sg = 1.0f;
    eg = 0.3f;
    sb = 0.0f;
    eb = 0.3f;
  } else if (val < 0.25) {
    f = 4 * val;
    sr = 0.3f;
    er = 0.7f;
    sg = 0.3f;
    eg = 0.7f;
    sb = 0.3f;
    eb = 0.0f;
  } else if (val < 0.5) {
    f = 4*(val - 0.25);
    sr = 0.7f;
    er = 0.7f;
    sg = 0.7f;
    eg = 0.3f;
    sb = 0.0f;
    eb = 0.0f;
  } else if (val < 0.75) {
    f = 4*(val - 0.5);
    sr = 0.7f;
    er = 1.0f;
    sg = 0.3f;
    eg = 0.0f;
    sb = 0.0f;
    eb = 0.0f;
  } else  {
    f = 4*(val - 0.75);
    sr = 1.0f;
    er = 0.5f;
    sg = 0.0f;
    eg = 0.0f;
    sb = 0.0f;
    eb = 1.0f;
  }
  return RGBColour(sr+f*(er-sr), sg+f*(eg-sg),sb+f*(eb-sb));
  /*
   * This one splits the range into 8 parts and linear maps
   * within parts.
*
  float chroma = fabs(val);
  float hue = (val >= 0) ? 240.0f * chroma : -240.0f * (chroma - 1);
  float luma = 0.299 + (0.5-0.299)*val;
  float hp = hue / 60.0f;
  float x = chroma * (1 - fabs(fmod(hp, 2.0) - 1.0));
  float rp, gp, bp;
  int idx = floor(hp/6.0);
  switch (idx) {
    case 0:
      rp = chroma;
      gp = x;
      bp = 0;
      break;
      
    case 1:
      rp = x;
      gp = chroma;
      bp = 0;
      break;
      
    case 2:
      rp = chroma;
      gp = x;
      bp = 0;
      break;
      
    case 3:
      rp = 0;
      gp = chroma;
      bp = x;
      break;
      
    case 4:
      rp = x;
      gp = 0;
      bp = chroma;
      break;
      
    case 5:
      rp = chroma;
      gp = 0;
      bp = x;
      break;
      
    default:
      rp = gp = bp = 0.0;
      break;
  }
  float m = luma - (0.3 * rp + 0.59*gp + 0.11*bp);
  return RGBColour(rp+m, gp+m, bp+m);
 */
  
/*
  float red = 0.5, green = 0.5, blue = 0.5, h;
//  double c = val / mRange;
  double hue = 180.0 * (val + 1.0) * mNCycle;
  if (val > 0) {
    h = val / 2.0;
    red = red - h;
    green = green - h;
    blue = blue + h;
  } else if (val < 0.0) {
    h = -val / 2.0;
    red = red + h;
    green = green - h;
    blue = blue - h;
  }
  return RGBColour(red, green, blue);
/
  return RGBColour(0.5+c, 0.5*(1.0+sin(4.7124*c)), 0.5-c);
  double white = 0.0;
  double total_color = 1.0;
  int hue_sector = ((int)(floor(hue/120.0))) % 3;
  double hue_factor = (hue - hue_sector*120.0)/120.0;
  switch (hue_sector) {
    case 0:
      blue = 0.0;
      green = hue_factor * total_color;
      red = total_color - green;
      break;
      
    case 1:
      red = 0.0;
      blue = hue_factor * total_color;
      green = total_color - blue;
      break;
      
    case 2:
      green = 0.0;
      red = hue_factor * total_color;
      blue = total_color - red;
      break;
      
    default:
      red = green = blue = 0.0;
      break;
  }
  return RGBColour(white+red, white+green, white+blue);
*/
}
//
//  Helper called when any parameter changes.
//
void RainbowMapper::Update(void)
{
}
