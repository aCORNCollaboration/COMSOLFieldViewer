/*
 *  GLMouseTools.h
 *  TrackViewer
 *
 *	A MouseTool is a specialised kind of mouse tool that
 *	connects to some kind of GLCanvas in order to influence the
 *	display parameters.
 *  I defines its own sub-class of MouseResponder to support the
 *  tools and then defines a set of sub-classes to handle the various
 *  display tasks.
 *
 *  Created by Brian Collett on 1/8/09.
 *  Expanded and revised BC 9/11/2010.
 *  Copyright 2009 Brian Collett. All rights reserved.
 *
 */
#ifndef _GLMouseTools_H
#define _GLMouseTools_H
#include "MouseTool.h"
//
//  First the MouseResponder that adds an interface that the user of a
//  GLMouseTool must implement.
//  Note that there is no expectation that one would ever create a simple
//  instance of a MouseResponder. It is intended to be mixed into a more
//  complex class, probably some kind of window class.
//
class GLMouseResponder : public MouseResponder {
public:
  //
  //  No instance variables. This makes multiple inheritance MUCH simpler
  //  and makes for trivial ctors.
  //
  GLMouseResponder() {};
  virtual ~GLMouseResponder() {};
  //
  //  Then the interface of things that the various tools want to be able
  //  to ask the display class to do.
  //  All have null bodies which can be overridden to provide additional
  //  functionality in a real use of the class.
  //
  virtual void LeftClick(int x, int y) {};
  virtual void LeftDClick(int x, int y) {};
  virtual void GetSize(wxSize& theSize) {};
  virtual void Spin(float quaternion[4]) {};
  virtual void Pan(float dx, float dy) {};
  virtual void Dolly(float inOut) {};
  virtual void Zoom(float inOut) {};
  virtual void StartRegion(float xs, float ys) {};
  virtual void Region(float xe, float ye) {};
  virtual void EndRegion(float xe, float ye) {};
};
//
//  The top-level class provides the instance var to hold a pointer to
//  the responder and ensures that it is a GLMouseResponder.
//
class CGLMouseTool : public CMouseTool {
protected:
  //
  //  We don't own this so we don't throw it away.
  //  By owning it at this level we get to know that it is a GLMouseResponder
  //  and not just any old responder.
  //
  GLMouseResponder* mResponder;
  //
	//	All the tools like to track the initial click.
  //  Although wxWidgets returns them as longs we keep them
  //  as ints because they could never get too large.
  //
	int mStartX;
	int mStartY;
public:
	//
	//	Constructor/Destructor
	//	Since we don't own the responder we cant dispose of it.
	//
	CGLMouseTool(GLMouseResponder* theResp) : mResponder(theResp),
                                            CMouseTool(theResp) {};
	virtual ~CGLMouseTool() {};
  //
  //  The tools share quite a bit of code. The special actions are handled
  //  in the LeftDrag method which is left to the sub-class.
  //
	virtual void DoLeftClick(wxMouseEvent& event);
	virtual void DoLeftDClick(wxMouseEvent& event);
	virtual void DoLeftRelease(wxMouseEvent& event);
};
//
//  The the individual tools.
//  The mouse tool interacts with a GL canvas to rotate the view
//	using a trackball algorithm from SGI.
//
class CSpinTool : public CGLMouseTool {
protected:
	//
	//	Instance variables.
	//
public:
	//
	//	Constructor/Destructor
  //
	CSpinTool(GLMouseResponder* theResp) : CGLMouseTool(theResp) {};
	virtual ~CSpinTool() {};
	//
	//	Drag handler does all the tool specific work.
	//
	virtual void DoLeftDrag(wxMouseEvent& event);
};
//
//	This is a GL mouse tool that moves the GL viewpoint
//	to pan across the scene.
//
class CPanTool : public CGLMouseTool {
protected:
	//
	//	Instance variables.
	//
public:
	//
	//	Constructor/Destructor
  //
	CPanTool(GLMouseResponder* theResp) : CGLMouseTool(theResp) {};
	virtual ~CPanTool() {};
	//
	//	Drag handler does all the tool specific work.
	//
	virtual void DoLeftDrag(wxMouseEvent& event);
};
//
//	This is a GL mouse tool that changes the distance between
//	viewer and image.
//
class CDollyTool : public CGLMouseTool {
protected:
	//
	//	Instance variables.
	//
public:
	//
	//	Constructor/Destructor
  //
	CDollyTool(GLMouseResponder* theResp) : CGLMouseTool(theResp) {};
	virtual ~CDollyTool() {};
	//
	//	Drag handler does all the tool specific work.
	//
	virtual void DoLeftDrag(wxMouseEvent& event);
};
//
//	This is a GL mouse tool that changes the GL field of
//	view to zoom the image.
//
class CZoomTool : public CGLMouseTool {
protected:
	//
	//	Instance variables.
  //
public:
	//
	//	Constructor/Destructor
  //
	CZoomTool(GLMouseResponder* theResp) : CGLMouseTool(theResp) {};
	virtual ~CZoomTool() {};
	//
	//	Drag handler does all the tool specific work.
	//
	virtual void DoLeftDrag(wxMouseEvent& event);
};

//
//	This one drags a rubber band rectangle on the screen and
//  passes it to the responder.
//
class CRegionTool : public CGLMouseTool {
protected:
  //
  //	Instance variables.
  //
public:
  //
  //	Constructor/Destructor
  //
  CRegionTool(GLMouseResponder* theResp) : CGLMouseTool(theResp) {};
  virtual ~CRegionTool() {};
  //
  //	Drag handler tells canvas where to draw box.
  //
  virtual void DoLeftDrag(wxMouseEvent& event);
  //
  //  We need to override click and release so we can tell the canvas
  //  where to start and stop.
  //
  virtual void DoLeftClick(wxMouseEvent& event);
  virtual void DoLeftRelease(wxMouseEvent& event);

};


#endif

