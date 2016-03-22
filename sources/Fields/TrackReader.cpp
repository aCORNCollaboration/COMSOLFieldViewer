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

static int sLineNum = 0;

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
//
//  Buld ourselves from a binary file.
//  We read a chunk consisting of a short header followed by a block
//  of position data. The header tells us how much data to read.
//
//
//  Our magic number.
//
uint32_t gBTrackMagic = 'Trak';

bool TrackReader::LoadFromBinaryFile(FILE* ifp)
{
  BL3TrackHeader head;
  size_t nRead = fread(&head, sizeof(BL3TrackHeader), 1, ifp);
  if (nRead != 1) {
    fprintf(stderr, "TrackReader::LoadFromBinaryFile read header failed.\n");
    return false;
  }
  if (head.mMagic  != gBTrackMagic) {
    fprintf(stderr, "TrackReader::LoadFromBinaryFile gMagic failed.\n");
    return false;
  }
  if (head.mPointSize != 3 * sizeof(float)) {
    fprintf(stderr, "TrackReader::LoadFromBinaryFile wrong size data.\n");
    return false;
  }
  int nPoint = head.mNPoint;
  float* coords = new float[nPoint * 3];
  if (nullptr == coords) {
    fprintf(stderr, "TrackReader::LoadFromBinaryFile can't alloc coords.\n");
    return false;
  }
  mLine = new PolyLine3D(nPoint+2);
  if (nullptr == mLine) {
    fprintf(stderr, "TrackReader::LoadFromBinaryFile can't alloc PLine.\n");
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
  nRead = fread(coords, sizeof(float), nPoint * 3, ifp);
  if (nRead != nPoint * 3) {
    fprintf(stderr, "TrackReader::LoadFromBinaryFile read data failed.\n");
    return false;
  }
/*  for (int pt = 0; pt < nPoint; pt++) {
    fprintf(stdout, "%g %g %g\n", coords[3*pt + 0], coords[3*pt + 1], coords[3*pt + 2]);
  }*/
  fprintf(stdout, "\n");
  for (int point = 0; point < nPoint; point++) {
    mLine->Add(Point3D(&coords[point * 3]));
  }
  mLine->Update();
//  delete[] coords;
  sLineNum++;
  return true;
}

