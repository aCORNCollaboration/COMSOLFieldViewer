/*
 *  FieldViewerView.h
 *  FieldViewer2
 *
 *  The View class for my document based electrostatic viewer
 *  application. This owns and manipulates the top level window that
 *  represents the document on the screen.
 *  This is a tiny extension of GLViewerView.
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
  FieldViewerDoc* mDoc;
  
  FieldViewerView(): GLViewerView() { mDoc = NULL; }
  ~FieldViewerView() {};
  
  virtual bool OnCreate(wxDocument* doc, long flags);
/*
  //
  //  This is called by the canvas to render the 3-D representations
	//	of the model.
  //
  virtual void Render(bool picking);
*/ 
private:
  DECLARE_DYNAMIC_CLASS(FieldViewerView)
};

#endif
