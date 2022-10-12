/*
 *  GLMouseTools.cpp
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
#include "GLMouseTools.h"
//
//	I first need  the trackball implementation from the Penguin sample to
//	provide a spin control for the viewer.
//
extern "C" {
#include "trackball.h"
}
//
//  Then the implementation methods.
//
//  First the base tool. It provides the basic click actions, which it passes
//  to its responder, who may well ignore them.
//
void CGLMouseTool::DoLeftClick(wxMouseEvent& event) {
  mStartX = (int) event.GetX();
  mStartY = (int) event.GetY();
  mResponder->LeftClick(mStartX, mStartY);
}
void CGLMouseTool::DoLeftDClick(wxMouseEvent& event) {
  mStartX = (int) event.GetX();
  mStartY = (int) event.GetY();
  mResponder->LeftDClick(mStartX, mStartY);
}
void CGLMouseTool::DoLeftRelease(wxMouseEvent& WXUNUSED(event)) {
}
//
//  The Spin tool rotates the view in its drag tool.
//
void CSpinTool::DoLeftDrag(wxMouseEvent& event) {
  wxSize sz(0,0);
  mResponder->GetSize(sz);

  /* drag in progress, simulate trackball */
  float spin_quat[4];
  trackball(spin_quat,
            (2.0*mStartX - sz.GetWidth()) / sz.GetWidth(),
            -(sz.GetHeight() - 2.0*mStartY) / sz.GetHeight(),
            (2.0*event.GetX() - sz.GetWidth())    / sz.GetWidth(),
            -(sz.GetHeight() - 2.0*event.GetY())    / sz.GetHeight());
  mStartX = event.GetX();
  mStartY = event.GetY();
  mResponder->Spin(spin_quat);
}
//
//  The Pan tool moves the image around.
//
void CPanTool::DoLeftDrag(wxMouseEvent& event) {
  //
  //	I compute the fractional motion in the viewport and
  //	use that to compute a fractional motion in the model
  //	space.
  //
  wxSize sz;
  mResponder->GetSize(sz);
  float dx = ((float) (event.GetX() - mStartX)) / ((float) sz.GetWidth());
  float dy = ((float) (event.GetY() - mStartY)) / ((float) sz.GetHeight());
  //
  //	That fraction was easy. The viewport one is a bit harder and requires
  //	serious amounts of GL info so I pass that up to the Canvas.
  //
  mResponder->Pan(dx, dy);
  mStartX = event.GetX();
  mStartY = event.GetY();
}
//
//  The dolly tool moves the image in or out.
//
void CDollyTool::DoLeftDrag(wxMouseEvent& event) {
  //
  //	I compute the fractional motion in the viewport and
  //	use that to compute a fractional motion in the model
  //	space.
  //
  wxSize sz;
  mResponder->GetSize(sz);
  float dy = ((float) (event.GetY() - mStartY)) / ((float) sz.GetHeight());
  //
  //	That fraction was easy. The viewport one is a bit harder and requires
  //	serious amounts of GL info so I pass that up to the Canvas.
  //
  mResponder->Dolly(dy);
  mStartY = event.GetY();
}
//
//  The zoom tool expects to change the viewing angle.
//
void CZoomTool::DoLeftDrag(wxMouseEvent& event) {
  //
  //	I compute the fractional motion in the viewport and
  //	use that to compute a fractional motion in the model
  //	space.
  //
  wxSize sz;
  mResponder->GetSize(sz);
  float dy = ((float) (event.GetY() - mStartY)) / ((float) sz.GetHeight());
  //
  //	That fraction was easy. The viewport one is a bit harder and requires
  //	serious amounts of GL info so I pass that up to the Canvas.
  //
  mResponder->Zoom(dy);
  mStartY = event.GetY();
}


//
//	This one drags a rubber band rectangle on the screen and
//  passes it to the responder.
//  NOTE that unlike the other tools this does not alter the
//  view until it ends.
//
void CRegionTool::DoLeftDrag(wxMouseEvent& event) {
  //
  //	Draw a rectangle from (startX, startY) to (x,y).
  //  For this we have to temporarily replace the projection
  //  matrix with an identity matrix
  //
  mResponder->Region(event.GetX(), event.GetY());
  //
  //	That fraction was easy. The viewport one is a bit harder and requires
  //	serious amounts of GL info so I pass that up to the Canvas.
  //
}
//
//  We need to override click and release so we can tell the canvas
//  where to start and stop.
//
void CRegionTool::DoLeftClick(wxMouseEvent& event)
{
  mResponder->StartRegion(event.GetX(), event.GetY());
}
void CRegionTool::DoLeftRelease(wxMouseEvent& event)
{
  mResponder->EndRegion(event.GetX(), event.GetY());
}

