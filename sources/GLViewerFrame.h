/*
 *  GLViewerFrame.h
 *  test
 *
 *  This is the frame for an GLViewer document. The frame provides the
 *  visible window frame and owns and handles the menus, status bar, and
 *  tool bar (if I can get one to work). The owning view can install
 *  panes inside the frame.
 *
 *  Reworked BCollett 2/20/14 moving the tools tool menu into the
 *  frame and letting it pass commands down to the GLCanvas inside.
 *  Created by Brian Collett on 9/22/10 based on work from the TrackViewerApp.
 *
 */
#ifndef _GLViewerFrame_H
#define _GLViewerFrame_H

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
#error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

#ifdef __DARWIN__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
//
//	Include those portions of the wxWidgets system that I need.
//
#include "wx/defs.h"
#include "wx/app.h"
#include "wx/menu.h"
#include "wx/dcclient.h"
#include "wx/wfstream.h"
#include "wx/docview.h"
#include "GLViewerCanvas.h"

#include "FieldViewerApp.h"
//
//	The window is a light sub-class of wxDocChildFrame that provides menu,
//  status bar, and tool bar facilities to the view.
//
class GLViewerFrame: public wxDocChildFrame
{
protected:
  //
  //  Class variables to hold the bitmaps.
  //
  static wxBitmap* sPtrSel;
  static wxBitmap* sPtrDis;
  static wxBitmap* sSpinSel;
  static wxBitmap* sSpinDis;
  static wxBitmap* sPanSel;
  static wxBitmap* sPanDis;
  static wxBitmap* sDollySel;
  static wxBitmap* sDollyDis;
  static wxBitmap* sZoomSel;
  static wxBitmap* sZoomDis;
  //
  GLViewerCanvas* mCanvas;
  //
  //  Most of the instance variables can safely be public.
  //
public:
  //
  //	Our menus and bars.
  //
  wxMenu *mFileMenu;
  wxMenu *mToolMenu;
  wxMenu *mHelpMenu;
  wxMenuBar *mMenuBar;
  wxStatusBar *mStatusBar;
  wxToolBar *mToolBar;
protected:
public:
  //
  //  ctors.
  //
  GLViewerFrame();
  GLViewerFrame(const wxDocument* doc, const wxView* view,
                const wxDocParentFrame *frame, const wxWindowID id,
                const wxString& title = wxString(_T("")),
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long type = wxDEFAULT_FRAME_STYLE);
  virtual ~GLViewerFrame() {};
  //
  //  Create does the real work.
  //
  bool Create(const wxDocument* doc,
              const wxView* view,
              const wxDocParentFrame* frame,
              const wxWindowID id, const wxString& title = wxString(_T("")),
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long type = wxDEFAULT_FRAME_STYLE);
  //
  //  Provide a mechanism for others to insert menus in our menu bar.
  //
  void InsertMenu(wxMenu* theMenu, const wxString& named, int at);
  //
  //  Install our Canvas.
  //
  void SetCanvas(GLViewerCanvas* newCanvas);
  //
  //  This one allows an outsider to tell us to enable or disable
  //  a file manu item.
  //
  void EnableFileItem(int menuId, bool enable);
  
protected:
  //
  //	Helpers such as menu handlers. Note that these can never be virtual
  //  as the wx event mechanism bypasses the C++ inheritance tree.
  //
  void OnMenuHelpAbout(wxCommandEvent& event);
  //
  //  Tool menu handlers. Must not be virtual.
  //
  void OnMenuToolPtr(wxCommandEvent& event);
  void OnMenuToolSpin(wxCommandEvent& event);
  void OnMenuToolPan(wxCommandEvent& event);
  void OnMenuToolDolly(wxCommandEvent& event);
  void OnMenuToolZoom(wxCommandEvent& event);
  void OnMenuToolRegion(wxCommandEvent& event);
  //
  //	Then the event table that makes it all work and add the RTTI info
  //  so that we can be created by name.
  //
  DECLARE_EVENT_TABLE()
  DECLARE_DYNAMIC_CLASS(GLViewerFrame);
};


#endif // _GLViewerFrame_H


