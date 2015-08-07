/*
 *  MouseTool.h
 *  wxGLAViewer->TrackViewer
 *
 *	A mouse tool is an abstract class that describes the basic
 *	behsvior of the mouse in a application. This is the base
 *	class for tools that provide such useful capabilities as
 *	panning and spinning the view in a 3-D window.
 *	xwWidgets provides a single mouse event and then
 *	requires the user to examine the event data to differentiate
 *	between the various kinds of mouse event. This means that
 *	someone has to make those decisions. I have come up with
 *	three scenarii
 *	1) Tools provide a single handler and do their own event
 *		sorting
 *	2) Tools provide a set of event handlers and rely on
 *		the owning entity to provide the sorting
 *	3) Tools provide a set of event handlers and then
 *		I provide a class function that does the sorting.
 *	I am going to try method 3. 
 *	At first I am going to implement only a subset of
 *	the possible mouse events that seems likely to be
 *	useful. The extension to more events can wait
 *	until I have a use for them.
 *  A mouse tool needs a way to communicate with some display class
 *  that actually responds to the tool events. We define MouseResponder, an
 *  interface class that can be mixed into another class to add
 *  MouseResponder actions to any display class. Unfortunately, there is
 *  no generic action that I can see for MouseResponder so even that has
 *  to be left to descendants.
 *
 *  Created by Brian Collett on 1/8/09.
 *  Revised BC 9/11/2010
 *  Copyright 2009,2010 Brian Collett. All rights reserved.
 *
 */
#ifndef _MouseTool_H
#define _MouseTool_H

#include "wx/event.h"

class MouseResponder {
public:
  //
  //  No instance variables. This makes multiple inheritance MUCH simpler
  //  and makes for trivial ctors.
  //
  MouseResponder() {};
  virtual ~MouseResponder() {};
  //
  //  Then the interface is minimal. Sub-classes will have to add
  //  the actual responses.
  //
  virtual void Click(int x, int y) {};
};

class CMouseTool {
protected:
	//
	//	Instance vars.
  //  Note that I have chose not to put a pointer to the MouseResponder
  //  in here. That way the sub-classes can keep pointers to sub-classes
  //  of MouseResponder and not need to down-cast them evey time they
  //  use them. I do still require that there be a MouseResponder by
  //  refusing to provide a default constructor.
  //
public:
	//
	//	Constructors/Destructor
	//
	CMouseTool(MouseResponder* theResp) {};
	virtual ~CMouseTool() {};
	//
	// Event methods exist to handle most of the events that can
	// be associated with the mouse. The default action for all events
	//	is to ignore the event. Sub-classes should override any of these
	//	events that they find interesting.
	//
	virtual void DoLeftClick(wxMouseEvent& event) {};
	virtual void DoLeftDClick(wxMouseEvent& event) {};
	virtual void DoLeftDrag(wxMouseEvent& event) {};
	virtual void DoLeftRelease(wxMouseEvent& event) {};
	//
	//	Class method to sort between cases. It is called with a pointer
	//	to the currently active tool as well as the event data.
	//
	static void DispatchToolEvent(wxMouseEvent& event, CMouseTool *theTool);
private:
  //
  //  A private default constructor ensures that we cannot be created without
  //  a MouseResponder.
  //
  CMouseTool() {};
};
#endif

