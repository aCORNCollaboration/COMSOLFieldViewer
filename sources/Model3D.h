//
//  Model3D.h
//  FieldViewerc
//
//  Model3D is a simple mix-in class that is used for document classes
//  that provide 3D models to the viewing system. It declares two
//  methods that the class must provide, a method to render the
//  model and a method to respond to clicks in the model.
//
//  The click routine gets passed information about the line along
//  which the click took place and an array of object names that lay
//  under the click.
//  The array belongs to the caller and MUST NOT be saved.
//
//  As a mix-in this is an abstract class and its methods must be
//  implemented by its descendent.
//
//  Created by Brian Collett on 7/17/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewerc__Model3D__
#define __FieldViewerc__Model3D__

#include "GeometricObjects.h"

class Model3D
{
public:
  Model3D() {};
  virtual ~Model3D(void) {};
  //
  virtual void Render(bool picking) {};
  virtual void DoClick(GLuint names[], Point3D start, Point3D end) = 0;
};

#endif /* defined(__FieldViewerc__Model3D__) */
