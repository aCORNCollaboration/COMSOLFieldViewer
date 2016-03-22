/*
 *  GLViewerFrame.cpp
 *  test
 *
 *  This is the frame for an GLViewer document. The frame provides the
 *  visible window frame and owns and handles the menus, status bar, and
 *  tool bar (if I can get one to work). The owning view can install
 *  panes inside the frame.
 *  This is based on work from the TrackViewerApp.
 *
 *  Modified BCollett 7/3/14 to implement the ToolBar. This also
 *  means making the bitmaps for the buttons into class variables
 *  so that they can be shared by all instances of the frame.
 *  Modified BCollett 2/19/2014 for FieldViewer. I need this to be
 *  dynamically created by the document system so I have to add a
 *  default constructor. That means that I need to implement 2-stage
 *  construction.
 *  Created by Brian Collett on 9/22/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <string.h>
#include "wx/filename.h"
#include "FieldViewerApp.h"
#include "GLViewerFrame.h"

#define BC_ADD_TOOLBAR

// ---------------------------------------------------------------------------
// GLViewerFrame
// ---------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(GLViewerFrame, wxDocChildFrame)

//
//  Event table.
//
BEGIN_EVENT_TABLE(GLViewerFrame, wxDocChildFrame)
//  EVT_MENU(wxID_OPEN, GLViewerFrame::OnMenuFileOpen)
//  EVT_MENU(wxID_EXIT, GLViewerFrame::OnMenuFileExit)
//  EVT_MENU(wxID_HELP, GLViewerFrame::OnMenuHelpAbout)
//  EVT_MENU(bcID_DATA_SHOWFULL, GLViewerFrame::OnMenuDataShowFull)
//  EVT_MENU(bcID_DATA_NEXT10, GLViewerFrame::OnMenuDataNext10)
EVT_MENU(bcID_TOOL_PTR, GLViewerFrame::OnMenuToolPtr)
EVT_MENU(bcID_TOOL_SPIN, GLViewerFrame::OnMenuToolSpin)
EVT_MENU(bcID_TOOL_PAN, GLViewerFrame::OnMenuToolPan)
EVT_MENU(bcID_TOOL_DOLLY, GLViewerFrame::OnMenuToolDolly)
EVT_MENU(bcID_TOOL_ZOOM, GLViewerFrame::OnMenuToolZoom)
EVT_MENU(bcID_TOOL_REGION, GLViewerFrame::OnMenuToolRegion)
END_EVENT_TABLE()
//
//  Class variables hold bitmaps.
//
wxBitmap* GLViewerFrame::sPtrSel = nullptr;
wxBitmap* GLViewerFrame::sPtrDis = nullptr;
wxBitmap* GLViewerFrame::sSpinSel = nullptr;
wxBitmap* GLViewerFrame::sSpinDis = nullptr;
wxBitmap* GLViewerFrame::sPanSel = nullptr;
wxBitmap* GLViewerFrame::sPanDis = nullptr;
wxBitmap* GLViewerFrame::sDollySel = nullptr;
wxBitmap* GLViewerFrame::sDollyDis = nullptr;
wxBitmap* GLViewerFrame::sZoomSel = nullptr;
wxBitmap* GLViewerFrame::sZoomDis = nullptr;

//
//  Constructors. Default just does minimal and expects that
//  Create will do the real work.
//
GLViewerFrame::GLViewerFrame()
{
  mFileMenu = nullptr;
  mHelpMenu = nullptr;
  mMenuBar = nullptr;
  mStatusBar = nullptr;
  mToolBar = nullptr;
}
//
//  Full contructor just passes work onto Create.
//
GLViewerFrame::GLViewerFrame(const wxDocument* doc,
                             const wxView* view,
                             const wxDocParentFrame* frame,
                             const wxWindowID id, const wxString& title,
                             const wxPoint& pos, const wxSize& size,
                             long type)
{
  Create(doc, view, frame, id, title, pos, size,type);
}
//
//  Create builds menus and tool bar.
//
bool GLViewerFrame::Create(const wxDocument* doc,
                           const wxView* view,
                           const wxDocParentFrame* frame,
                           const wxWindowID id, const wxString& title,
                           const wxPoint& pos, const wxSize& size,
                           long type)
{
  //
  //  Build super first.
  //
  wxFrame* f = wxDynamicCast(frame, wxFrame);
  if (!wxDocChildFrame::Create((wxDocument*) doc, (wxView* ) view,
                    f, id, title,
                               pos, size, type)) {
    return false;
  }

  //
  //	Setup menu system.
  //  Make the "File" menu
  //
  mFileMenu = new wxMenu;
  mFileMenu->Append(wxID_OPEN, wxT("&Open...\tCtrl-O"));
//  mFileMenu->Append(bcID_FILE_LOAD3D, wxT("&Load 3D Field...\tCtrl-L"));
  mFileMenu->Append(wxID_CLOSE, wxT("&Close\tCtrl-W"));
  mFileMenu->AppendSeparator();
//  mFileMenu->Enable(bcID_FILE_SELPLANE, false);
  mFileMenu->Append(wxID_SAVE, wxT("&Save...\tCtrl-S"));
  mFileMenu->Append(wxID_SAVEAS, wxT("Save &As...\tCtrl-Shift-S"));
  mFileMenu->AppendSeparator();
  mFileMenu->Append(wxID_EXIT, wxT("E&xit\tCtrl-Q"));
  mFileMenu->Enable(wxID_SAVEAS, false);
  mFileMenu->Enable(wxID_SAVE, false);
  //
  // Build and install the "Tool" menu
  //
  mToolMenu = new wxMenu;
  mToolMenu->AppendCheckItem(bcID_TOOL_PTR, wxT("&Ptr\tCtrl-R"),
                             wxT("Pointer tool"));
  mToolMenu->AppendCheckItem(bcID_TOOL_SPIN, wxT("&Spin\tCtrl-P"),
                             wxT("Spin tool"));
  mToolMenu->AppendCheckItem(bcID_TOOL_PAN, wxT("&Pan\tCtrl-A"),
                             wxT("Pan tool"));
  mToolMenu->AppendCheckItem(bcID_TOOL_DOLLY, wxT("&Dolly\tCtrl-D"),
                             wxT("Dolly tool"));
  mToolMenu->AppendCheckItem(bcID_TOOL_ZOOM, wxT("&Zoom\tCtrl-Z"),
                             wxT("Zoom tool"));
  mToolMenu->AppendCheckItem(bcID_TOOL_REGION, wxT("Se&lect"),
                             wxT("Select region tool"));
  mToolMenu->Check(bcID_TOOL_PTR, true);
  mToolMenu->Check(bcID_TOOL_SPIN, false);
  mToolMenu->Check(bcID_TOOL_PAN, false);
  mToolMenu->Check(bcID_TOOL_DOLLY, false);
  mToolMenu->Check(bcID_TOOL_ZOOM, false);
  mToolMenu->Check(bcID_TOOL_REGION, false);
  //
  // Build and install the "Tracks" menu
  //
  mTracksMenu = new wxMenu;
  mTracksMenu->Append(bcID_TRACKS_LOAD, wxT("&Load tracks...\tCtrl-L"));
  //
  // Make the "Help" menu
  //
  mHelpMenu = new wxMenu;
  mHelpMenu->Append(wxID_HELP, _T("&About\tF1"));
  //
  // Build menu bar.
  //
  mMenuBar = new wxMenuBar;
  mMenuBar->Append(mFileMenu, wxT("&File"));
  mMenuBar->Append(mToolMenu, wxT("&Tool"));
  mMenuBar->Append(mTracksMenu, wxT("&Tracks"));
  mMenuBar->Append(mHelpMenu, wxT("&Help"));
  
  SetMenuBar(mMenuBar);
  //
  //	Give ourselves a status bar and (optionally) a tool bar.
  //	Even though I am using a text only toolbar I still need to
  //	have some kind of bitmap for the AddTool method so I create
  //	and empty 1x1 bitmap to use in all bitmap locations.
  //
  mStatusBar = CreateStatusBar(1, wxSB_NORMAL, 
                               bcID_STATUS_BAR, wxT("Status"));
  
#ifdef BC_ADD_TOOLBAR
  //
  //  Load bitmaps if needed.
  //
  if (sPtrSel == nullptr) {
    wxImage::AddHandler( new wxPNGHandler );
    sPtrSel = new wxBitmap(wxT("PtrSel.png"), wxBITMAP_TYPE_PNG);
    sPtrDis = new wxBitmap(wxT("PtrDis.png"), wxBITMAP_TYPE_PNG);
    sSpinSel = new wxBitmap(wxT("SpinSel.png"), wxBITMAP_TYPE_PNG);
    sSpinDis = new wxBitmap(wxT("SpinDis.png"), wxBITMAP_TYPE_PNG);
    sPanSel = new wxBitmap(wxT("PanSel.png"), wxBITMAP_TYPE_PNG);
    sPanDis = new wxBitmap(wxT("PanDis.png"), wxBITMAP_TYPE_PNG);
    sDollySel = new wxBitmap(wxT("DollySel.png"), wxBITMAP_TYPE_PNG);
    sDollyDis = new wxBitmap(wxT("DollyDis.png"), wxBITMAP_TYPE_PNG);
    sZoomSel = new wxBitmap(wxT("ZoomSel.png"), wxBITMAP_TYPE_PNG);
    sZoomDis = new wxBitmap(wxT("ZoomDis.png"), wxBITMAP_TYPE_PNG);
  }
  //
  //  Build bar.
  //
  mToolBar = CreateToolBar();

  mToolBar->AddRadioTool(bcID_TOOL_PTR, wxT(" Ptr "),
                         *sPtrSel, *sPtrDis,
                         wxT("Pointer tool"),
                         wxT("Pointer tool does nothing!"));

  mToolBar->AddRadioTool(bcID_TOOL_SPIN, wxT("Spin "),
                         *sSpinSel, *sSpinDis,
                         wxT("Spin tool"),
                         wxT("Spin tool rotates model in view"));
  
  mToolBar->AddRadioTool(bcID_TOOL_PAN, wxT(" Pan "),
                         *sPanSel, *sPanDis,
                         wxT("Pan tool"),
                         wxT("Pan tool moves model in view"));
  
  mToolBar->AddRadioTool(bcID_TOOL_DOLLY, wxT("Dolly"),
                         *sDollySel, *sDollyDis,
                         wxT("Dolly tool"),
                         wxT("Dolly tool adjusts distance to model."));
  
  mToolBar->AddRadioTool(bcID_TOOL_ZOOM, wxT("Zoom "),
                         *sZoomSel, *sZoomDis,
                         wxT("Zoom tool"),
                         wxT("Zoom tool alters field of view."));
  mToolBar->Realize();
#else
  mToolBar = NULL;
#endif
  return true;
}
//
//  Provide a mechanism for others to insert menus in our menu bar.
//
void GLViewerFrame::InsertMenu(wxMenu* theMenu, const wxString& named, int at)
{
  wxLogMessage("GLViewerFrame:InsertMenu at %d\n", at);
  mMenuBar->Insert(at, theMenu, named);
}
//
//  This one allows an outsider to tell us to enable or disable
//  a file manu item.
//
void GLViewerFrame::EnableFileItem(int menuId, bool enable)
{
  mFileMenu->Enable(menuId, enable);
}
//
//  Install our Canvas.
//  Bind its tool menu handlers to the menu events.
//
void GLViewerFrame::SetCanvas(GLViewerCanvas* newCanvas)
{
  mCanvas = newCanvas;
}
//
//  Menu handlers.
//
void GLViewerFrame::OnMenuToolPtr(wxCommandEvent& WXUNUSED(event))
{
  mCanvas->SetTool(kGLToolPtr);
  mToolMenu->Check(bcID_TOOL_PTR, true);
  mToolMenu->Check(bcID_TOOL_SPIN, false);
  mToolMenu->Check(bcID_TOOL_PAN, false);
  mToolMenu->Check(bcID_TOOL_DOLLY, false);
  mToolMenu->Check(bcID_TOOL_ZOOM, false);
  mToolMenu->Check(bcID_TOOL_REGION, false);
}

void GLViewerFrame::OnMenuToolSpin(wxCommandEvent& WXUNUSED(event))
{
  mCanvas->SetTool(kGLToolSpin);
  mToolMenu->Check(bcID_TOOL_PTR, false);
  mToolMenu->Check(bcID_TOOL_SPIN, true);
  mToolMenu->Check(bcID_TOOL_PAN, false);
  mToolMenu->Check(bcID_TOOL_DOLLY, false);
  mToolMenu->Check(bcID_TOOL_ZOOM, false);
  mToolMenu->Check(bcID_TOOL_REGION, false);
}

void GLViewerFrame::OnMenuToolPan(wxCommandEvent& WXUNUSED(event))
{
  mCanvas->SetTool(kGLToolPan);
  mToolMenu->Check(bcID_TOOL_PTR, false);
  mToolMenu->Check(bcID_TOOL_SPIN, false);
  mToolMenu->Check(bcID_TOOL_PAN, true);
  mToolMenu->Check(bcID_TOOL_DOLLY, false);
  mToolMenu->Check(bcID_TOOL_ZOOM, false);
  mToolMenu->Check(bcID_TOOL_REGION, false);
}

void GLViewerFrame::OnMenuToolDolly(wxCommandEvent& WXUNUSED(event))
{
  mCanvas->SetTool(kGLToolDolly);
  mToolMenu->Check(bcID_TOOL_PTR, false);
  mToolMenu->Check(bcID_TOOL_SPIN, false);
  mToolMenu->Check(bcID_TOOL_PAN, false);
  mToolMenu->Check(bcID_TOOL_DOLLY, true);
  mToolMenu->Check(bcID_TOOL_ZOOM, false);
  mToolMenu->Check(bcID_TOOL_REGION, false);
}

void GLViewerFrame::OnMenuToolZoom(wxCommandEvent& WXUNUSED(event))
{
  mCanvas->SetTool(kGLToolZoom);
  mToolMenu->Check(bcID_TOOL_PTR, false);
  mToolMenu->Check(bcID_TOOL_SPIN, false);
  mToolMenu->Check(bcID_TOOL_PAN, false);
  mToolMenu->Check(bcID_TOOL_DOLLY, false);
  mToolMenu->Check(bcID_TOOL_ZOOM, true);
  mToolMenu->Check(bcID_TOOL_REGION, false);
}

void GLViewerFrame::OnMenuToolRegion(wxCommandEvent& WXUNUSED(event))
{
  mCanvas->SetTool(kGLToolRegion);
  mToolMenu->Check(bcID_TOOL_PTR, false);
  mToolMenu->Check(bcID_TOOL_SPIN, false);
  mToolMenu->Check(bcID_TOOL_PAN, false);
  mToolMenu->Check(bcID_TOOL_DOLLY, false);
  mToolMenu->Check(bcID_TOOL_ZOOM, false);
  mToolMenu->Check(bcID_TOOL_REGION, true);
}
