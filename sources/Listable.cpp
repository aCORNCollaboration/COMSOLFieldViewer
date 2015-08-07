//
//  Listable.cpp
//  FieldViewer
//
//  This is the base class for some kind of item that can be stored in
//  a double-linked list.
//  Note that it can make a lot of sense to use a pair of bare
//  listables as the ends of a list since that makes insertion and
//  deletion completely symmetric with no need to special case the
//  terminals.
//
//  Created by Brian Collett on 2/26/14.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#include "Listable.h"
//
//  ctors
//
Listable::Listable()
{
  mNext = nullptr;
  mPrev = nullptr;
}
Listable::~Listable()
{
  
}
//
//  List operations.
//
void Listable::Append(Listable* f)
{
  f->mNext = mNext;
  f->mPrev = this;
  (f->mNext)->mPrev = f;
  mNext = f;
}
void Listable::InsertBefore(Listable* f)
{
  f->mNext = this;
  f->mPrev = mPrev;
  (f->mPrev)->mNext = f;
  mPrev = f;
}
void Listable::Delete()
{
  mNext->mPrev = mPrev;
  mPrev->mNext = mNext;
}
