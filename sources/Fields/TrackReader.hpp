//
//  TrackReader.hpp
//  FieldViewerc
//
//  Simple function to represent a particle track produced by the
//  simulator.
//  Knows how to read a complete track from an open file.
//
//  Created by Brian Collett on 3/21/16.
//  Copyright © 2016 Brian Collett. All rights reserved.
//

#ifndef SimTrack_hpp
#define SimTrack_hpp

#include <stdio.h>
#include "GeometricObjects.h"

//
//  Our magic number.
//
extern uint32_t gBTrackMagic;

struct BL3TrackHeader {
  uint32_t mMagic;
  uint32_t mNPoint;
  uint32_t mPointSize;
};

class TrackReader
{
protected:
  PolyLine3D* mLine;  // We build it but we don't own it
public:
  TrackReader();
  virtual ~TrackReader();
  //
  //  Buld ourselves from a text file.
  //
 bool LoadFromTextFile(FILE* ifp);
  //
  //  Buld ourselves from a binary file.
  //
  bool LoadFromBinaryFile(FILE* ifp);
  //
  PolyLine3D* GetLine() { return mLine; };
};

#endif /* SimTrack_hpp */
