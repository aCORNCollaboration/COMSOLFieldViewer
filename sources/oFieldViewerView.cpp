/*
 *  FieldViewerView.h
 *  FieldViewer
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

#include "FieldViewerApp.h"
#include "FieldViewerDoc.h"
#include "FieldViewerView.h"
#include "GLViewerCanvas.h"



IMPLEMENT_DYNAMIC_CLASS(FieldViewerView, wxView)

bool FieldViewerView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
  mDoc = wxDynamicCast(doc, FieldViewerDoc);
  //
  //  We need to build our frame and install a view into it.
  //
  mFrame = new GLViewerFrame(doc,
                             this,
                             wxGetApp().GetMainFrame(),
                             wxID_ANY);
  int width, height;
  mFrame->GetClientSize(&width, &height);
  mCanvas = new GLViewerCanvas(this, mFrame, wxID_ANY);
  mFrame->SetCanvas(mCanvas);
  
#ifdef __X__
  // X seems to require a forced resize
  int x, y;
  frame->GetSize(&x, &y);
  frame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif
  
  mFrame->Show(true);
  Activate(true);
  
  return true;
}
//
//  This is called by the canvas to render the 3-D representations
//	of the model.
//
void FieldViewerView::Render()
{
  mDoc->RenderModel();
}

// Handled by wxTextWindow
void FieldViewerView::OnDraw(wxDC *WXUNUSED(dc) )
{
}

void FieldViewerView::OnUpdate(wxView *WXUNUSED(sender), wxObject *WXUNUSED(hint) )
{
}

bool FieldViewerView::OnClose(bool deleteWindow)
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
