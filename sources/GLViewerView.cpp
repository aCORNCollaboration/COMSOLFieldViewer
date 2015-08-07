/*
 *  GLViewerView.h
 *  GLViewer2
 *
 *  The View class for my document based electrostatic viewer
 *  application. This owns and manipulates the top level window that
 *  represents the document on the screen. It divides that window
 *  into several panes to display the 3-D view as well as text tree
 *  views of the objects and rendering displayed.
 *
 *	The View can access the model directly, though it does not own
 *	it, and the view is responsible for displaying the model and for
 *	interacting with the user to modify the views. This seems to make
 *	sense as there is nothing that the viewer can do that will actually
 *	alter the model.
 *	The viewer maintains the View menu and (indirectly) the Tool menu
 *	and the View keeps an array of the current representations of the
 *	model.
 *
 *  Created by Brian Collett on 9/22/10.
 *	Updated 3/12/12
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include "GLViewerView.h"
#include "GLViewerFrame.h"
#include "GLViewerCanvas.h"

IMPLEMENT_DYNAMIC_CLASS(GLViewerView, wxView)
//
//  Event table.
//
BEGIN_EVENT_TABLE(GLViewerView, wxView)
END_EVENT_TABLE()
//
//  ctor.
//
GLViewerView::GLViewerView(): wxView()
{
  mFrame = (GLViewerFrame *) nullptr;
  mGLWind = nullptr;
}

bool GLViewerView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
  int width, height;

  const wxDocParentFrame* theParent =
    wxDynamicCast( wxGetApp().GetMainFrame(), wxDocParentFrame);
  const wxView* theView = wxDynamicCast(this, wxView);
  const wxDocument* theDoc = wxDynamicCast(doc, wxDocument);
  mFrame = new GLViewerFrame(theDoc,
                             theView,
                             theParent,
                             wxID_ANY,
                             wxString(_T("")),
                             wxDefaultPosition, wxSize(1000,800),
                             wxDEFAULT_FRAME_STYLE);
  mFrame->GetClientSize(&width, &height);
  mFrame->SetTitle(_T("GLViewerView"));
  //
  //  Get a 3-D view for the pane.
  //
  mGLWind = new GLViewerCanvas(this, mFrame);
  mGLWind->Install(dynamic_cast<Model3D*>(doc));
  mFrame->SetCanvas(mGLWind);
  mGLWind->Show(true);
  
  
#ifdef __X__
  // X seems to require a forced resize
  int x, y;
  mFrame->GetSize(&x, &y);
  mFrame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif
  //
  //  Make the whole thing visible and bring it to the front.
  //
  mFrame->Show(true);
  Activate(true);
  return true;
}
//
//  This is called by the canvas to render the 3-D representations
//	of the model.
//
void GLViewerView::Render(bool picking)
{
}

//
void GLViewerView::OnDraw(wxDC *WXUNUSED(dc) )
{
}

void GLViewerView::OnUpdate(wxView *WXUNUSED(sender), wxObject *WXUNUSED(hint))
{
  mGLWind->Refresh();
}

bool GLViewerView::OnClose(bool deleteWindow)
{
  if (!GetDocument()->Close())
    return false;
  
  Activate(false);
  
  if (deleteWindow)
  {
    delete mFrame;
    return true;
  }
  return true;
}

void GLViewerView::InsertMenu(wxMenu* theMenu, const wxString& named, int at)
{
  wxLogMessage("View:InsertMenu at %d\n", at);
  mFrame->InsertMenu(theMenu, named, at);
}


