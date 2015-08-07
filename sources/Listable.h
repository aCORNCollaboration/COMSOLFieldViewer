//
//  Listable.h
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
//  BCollett 7/4/14 Add idea of being selected with vars and
//  methods to support.
//  Copyright (c) 2014 Brian Collett. All rights reserved.
//

#ifndef __FieldViewer__Listable__
#define __FieldViewer__Listable__


class Listable {
public:
  //
  //  Instance Vars
  //
  Listable* mNext;
  Listable* mPrev;
  bool mSelected;
  //
public:
  //
  //  ctors
  //
  Listable();
  virtual ~Listable();
  //
  //  List operations.
  //
  virtual void Append(Listable* f);
  virtual void InsertBefore(Listable* f);
  virtual void Delete(void);
  //
  //  Selection operations.
  //
  virtual void Select(void) { mSelected = true; };
  virtual void Deselect(void) { mSelected = false; };
};


#endif /* defined(__FieldViewer__Listable__) */
