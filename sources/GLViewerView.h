/*
 *  GLViewerView.h
 *  GLViewer
 *
 *  View class for systems that use a GLViewerCanvas to display
 *  3D images of an OpenGL model.
 *
*
 *  Redone BCollett 2/19/14
 *
 */

#ifndef _GLViewerVIEW_H
#define _GLViewerVIEW_H

#include "wx/docview.h"
#include "wx/splitter.h"
#include "wx/listctrl.h"
#include "wx/glcanvas.h"
#include "GLViewerFrame.h"
#include "GLViewerCanvas.h"
#include "Model3D.h"


//class GLViewerCanvas;

class GLViewerView: public wxView
{
public:
  //
  //  The model that we display.
  //
  Model3D* mModel;
  //
  //  Various window elements. We own these.
  //
  GLViewerFrame* mFrame;          // Frame window for the view
  GLViewerCanvas* mGLWind;        // 3-D view of model
  //
  //  ctors.
  //
  GLViewerView();
  ~GLViewerView() {}
  //
  //  Pass-thru to install a menu in the mFrame's menu bar.
  //
  virtual void InsertMenu(wxMenu* theMenu, const wxString& named, int at);
  //
  //  This is called by the canvas to render the 3-D representations
	//	of the model.
  //
  virtual void Render(bool picking);
  //
  //  Replace standard handlers. These are not called through the
  //  event mechanism and so can be virtual.
  //
  virtual bool OnCreate(wxDocument *doc, long flags);
  virtual void OnDraw(wxDC *dc);
  virtual void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
  virtual bool OnClose(bool deleteWindow = true);
  //
  //  Pass-thru to canvas.
  //
  void FocusOn(Frame3D* box, bool keep) { mGLWind->FocusOn(box, keep); };
private:
  //
  //  First the info to allow us to be created by the doc manager.
  //
  DECLARE_DYNAMIC_CLASS(GLViewerView)
	//
	//	Then the event table that makes events work.
	//
  DECLARE_EVENT_TABLE()
};

#endif
