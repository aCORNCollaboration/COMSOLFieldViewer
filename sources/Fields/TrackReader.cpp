//
//  TrackReader.cpp
//  FieldViewerc
//
//  Simple function to represent a particle track produced by the
//  simulator.
//  Knows how to read a complete track from an open file.
//
//  Created by Brian Collett on 3/21/16.
//  Copyright © 2016 Brian Collett. All rights reserved.
//
#include <stdio.h>
#include <string.h>

#include "TrackReader.hpp"
TrackReader::TrackReader() : mLine(nullptr)
{
  
}
TrackReader::~TrackReader()
{
  //  We don't own it, we can't free it.
}
//
//  Buld ourselves from a text file.
//
bool TrackReader::LoadFromTextFile(FILE* ifp)
{
  char LineBuff[256];
  int nPoint;
  static int sLineNum = 0;
  //
  //  First line should be N:<num points>
  //
  if (fgets(LineBuff, 255, ifp) == nullptr) {
    return false;
  }
  if (strncmp(LineBuff, "N:", 2) != 0) {
    return false;
  }
  if (sscanf(LineBuff+2, "%d", &nPoint) != 1) {
    return false;
  }
  //
  //  Now we can allocate the PolyLine3d.
  //
  mLine = new PolyLine3D(nPoint+2);
  if (nullptr == mLine) {
    return false;
  }
  switch (sLineNum % 3) {
    case 0:
      mLine->SetColor(1.0, 0.0, 0.0);
      break;
      
    case 1:
      mLine->SetColor(0.0, 1.0, 1.0);
      break;
      
    case 2:
      mLine->SetColor(1.0, 1.0, 0.0);
      break;
      
    default:
      break;
  }
  sLineNum++;
  //
  //  Read data into polyline. Note that the file contains velocity
  //  data as well as position date. We only want the first three
  //  values on each line.
  //
  for (int line = 0; line < nPoint; line++) {
    Point3D cPoint;
    double time;
    if (fgets(LineBuff, 255, ifp) == nullptr) {
      delete mLine;
      mLine = nullptr;
      return false;
    }
    int nRead = sscanf(LineBuff, "%lg\t%lg\t%lg\t%lg",
                       &time, &cPoint.mX, &cPoint.mY, &cPoint.mZ);
    if (nRead != 4) {
      fprintf(stderr, "Read too %d of 4 values from track line %d.\n", nRead, line);
      delete mLine;
      mLine = nullptr;
      return false;
    }
    mLine->Add(cPoint);
  }
  //
  //  Eat two spare lines.
  //
  if (fgets(LineBuff, 255, ifp) == nullptr) {
    return false;
  }
  if (fgets(LineBuff, 255, ifp) == nullptr) {
    return false;
  }
  //
  mLine->Update();
  return true;
}
