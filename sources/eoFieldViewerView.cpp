/*
 *  FieldViewerView.h
 *  FieldViewer
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

bool FieldViewerView::OnCreate(wxDocument *doc, long flags)
{
  mDoc = wxDynamicCast(doc, FieldViewerDoc);
  return GLViewerView::OnCreate(doc, flags);
}
/*
//
//  This is called by the canvas to render the 3-D representations
//	of the model.
//
void FieldViewerView::Render(bool picking)
{
  mDoc->RenderModel(picking);
}
*/