/*
 *  MouseTool.cpp
 *  TrackViewer
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
 * 
 *  Created by Brian Collett on 1/8/09.
 *  Copyright 2009 Brian Collett. All rights reserved.
 *
 */
#include "MouseTool.h"
//
//	Start with the class method that sorts out the various events
//	and dispatches them to the current tool.
//
void CMouseTool:: DispatchToolEvent(wxMouseEvent& event, CMouseTool* theTool)
{
	//
	//	Because the wxMouseEvent is opaque I cannot do a switch on
	//	the event type but must painfully test my way through the
	//	cases that I care about.
	//
	if (event.LeftDClick()) {
		theTool->DoLeftDClick(event);
	} else if (event.LeftDown()) {
		theTool->DoLeftClick(event);
	} else if (event.LeftUp()) {
		theTool->DoLeftRelease(event);
	} else if (event.Dragging()) {
		//
		//	Now should decide which buttons are down during drag
		//	but for now I don't care and will treate the
		//	cases all the same.
		//
		theTool->DoLeftDrag(event);
	} else if (event.Moving()) {
	} else if (event.Entering()) {
	} else if (event.Leaving()) {
	}
  //
  //  Now we are done with the event make sure others get a chance.
  //
  event.Skip();
}

