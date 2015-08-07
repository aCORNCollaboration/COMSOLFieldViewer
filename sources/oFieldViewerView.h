/*
 *  FieldViewerView.h
 *  FieldViewer2
 *
 *  The View class for my document based electrostatic viewer
 *  application. This owns and manipulates the top level window that
 *  represents the document on the screen. It divides that window
 *  into several panes to display the 3-D view as well as text tree
 *  views of the objects and rendering displayed.
 *
 *  Created by Brian Collett on 9/22/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */

#ifndef _FieldViewerVIEW_H
#define _FieldViewerVIEW_H

#include "wx/docview.h"
#include "GLViewerView.h"
#include "GLViewerFrame.h"

class GLViewerCanvas;
class FieldViewerDoc;

class FieldViewerView : public GLViewerView
{
public:
  GLViewerFrame* mFrame;
  GLViewerCanvas* mCanvas;
  FieldViewerDoc* mDoc;
  
  FieldViewerView(): GLViewerView() { \
    mFrame = (GLViewerFrame *) NULL; mCanvas = NULL; }
  ~FieldViewerView() {}
  
  virtual bool OnCreate(wxDocument* doc, long flags);
  void OnDraw(wxDC *dc);
  void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
  bool OnClose(bool deleteWindow = true);
  //
  //  This is called by the canvas to render the 3-D representations
	//	of the model.
  //
  virtual void Render();
  
private:
  DECLARE_DYNAMIC_CLASS(FieldViewerView)
};

#endif
