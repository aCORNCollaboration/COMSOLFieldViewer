/*
 *  FieldViewerApp.cpp
 *  FieldViewer
 *
 *  This is the top level application for 3D scalar/vector
 *  field viewer.
 *  This does very little except set up the main frame and
 *  the document manager to support creating and opening
 *  documents.
 *
 *  Created by Brian Collett on 2/18/14.
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
#include "FieldViewerApp.h"
#include "FieldViewerDoc.h"

IMPLEMENT_APP(FieldViewerApp)

FieldViewerApp::FieldViewerApp(void)
{
    mDocManager = (wxDocManager *) NULL;
}

bool FieldViewerApp::OnInit(void)
{
  //// Create a document manager
  mDocManager = new wxDocManager;
  
  //// Create a template relating .gla documents to their views
  (void) new wxDocTemplate(mDocManager,
                           _T("Model"), _T("*.gla"),
                           _T(""), _T("gla"),
                           _T("Model Doc"), _T("Model View"),
                           CLASSINFO(FieldViewerDoc),
                           CLASSINFO(GLViewerView));
  
  //// Create the main frame window
  mFrame = new wxDocParentFrame((wxDocManager *) mDocManager,
                        (wxFrame *) NULL,
                        wxID_ANY,
                        _T("Field Viewer"),
                        wxPoint(0, 0),
                        wxSize(500, 400),
                        wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE);
  mLogText = new wxTextCtrl(mFrame,
                            wxID_ANY,
                            _T("Welcome to Field Viewer\n"),
                            wxDefaultPosition,
                            wxDefaultSize,
                            wxTE_MULTILINE |
                            wxTE_READONLY);

  //// Give it an icon (this is ignored in MDI mode: uses resources)
#ifdef __WXMSW__
  frame->SetIcon(wxIcon(_T("doc")));
#endif
#ifdef __X__
  frame->SetIcon(wxIcon(_T("doc.xbm")));
#endif
  //
  // Connect the text control to the logging mechanism.
  //
  wxLogTextCtrl* myLog = new wxLogTextCtrl(mLogText);
  wxLog::SetActiveTarget(myLog);
  //myLog->DisableTimestamp();
  wxLogMessage(_T("This is from the logger."));
  //
  //  Then put text in directly.
  //  NOTE that although the logging mechanism adds the newline
  //  for us, AppendText does just what we ask.
  //
  oprintf("oprintf\n");
  iprintf("iprintf\n");
  eprintf("eprintf\n");
  wprintf("wprintf\n");
  //// Make a menubar
  wxMenu *mFileMenu = new wxMenu;
  wxMenu *mEditMenu = (wxMenu *) NULL;
  
  mFileMenu->Append(wxID_OPEN, _T("&Open...\tCtrl-O"));
  
  mFileMenu->AppendSeparator();
  mFileMenu->Append(wxID_EXIT, _T("E&xit\tAlt-X"));
  
  // A nice touch: a history of files visited. Use this menu.
  mDocManager->FileHistoryUseMenu(mFileMenu);
  
  wxMenu *mHelpMenu = new wxMenu;
  mHelpMenu->Append(DOCVIEW_ABOUT, _T("&About\tF1"));
  
  wxMenuBar *menu_bar = new wxMenuBar;
  
  menu_bar->Append(mFileMenu, _T("&File"));
  if (mEditMenu)
      menu_bar->Append(mEditMenu, _T("&Edit"));
  menu_bar->Append(mHelpMenu, _T("&Help"));
  
#ifdef __WXMAC__
    wxMenuBar::MacSetCommonMenuBar(menu_bar);
#endif //def __WXMAC__
  
    //// Associate the menu bar with the frame
    mFrame->SetMenuBar(menu_bar);
    
//    mFrame->Centre(wxBOTH);
    //#ifndef __WXMAC__
    mFrame->Show(true);
    //#endif //ndef __WXMAC__
    
    SetTopWindow(mFrame);
    return true;
}

int FieldViewerApp::OnExit(void)
{
    delete mDocManager;
    return 0;
}

/*
 * Centralised code for creating a document frame.
 * Called from view.cpp, when a view is created.
 */

GLViewerFrame* FieldViewerApp::CreateChildFrame(wxDocument *doc, wxView *view)
{
    //// Make a child frame
    GLViewerFrame *subframe =
    new GLViewerFrame(doc,
                      view,
                      GetMainFrame(),
                      wxID_ANY,
                      _T("Child Frame"),
                      wxPoint(10, 80), wxSize(300, 300),
                      wxDEFAULT_FRAME_STYLE |
                        wxNO_FULL_REPAINT_ON_RESIZE);

    //// Make a menubar
    wxMenu *fileMenu = new wxMenu;
    
    fileMenu->Append(wxID_OPEN, _T("&Open..."));
    fileMenu->Append(wxID_CLOSE, _T("&Close"));
    fileMenu->Append(wxID_SAVE, _T("&Save"));
    fileMenu->Append(wxID_SAVEAS, _T("Save &As..."));
  
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, _T("E&xit"));
    
//    wxMenu *editMenu = (wxMenu *) NULL;
    
  
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(DOCVIEW_ABOUT, _T("&About"));
    
    wxMenuBar *menu_bar = new wxMenuBar;
    
    menu_bar->Append(fileMenu, _T("&File"));
    menu_bar->Append(helpMenu, _T("&Help"));
    
    //// Associate the menu bar with the frame
    subframe->SetMenuBar(menu_bar);

    return subframe;
}

//
//  Routines to print to the logging pane.
//  They share a hidden buffer for building strings.
//
static char sLogBuff[512];

int oprintf(const char* format, ...) { // printf, black
  int retCode = 0;
  wxTextCtrl* text = wxGetApp().LogPane();
  va_list args;
  va_start (args, format);
  retCode = vsprintf(sLogBuff, format, args);
  text->SetDefaultStyle(wxTextAttr(*wxBLACK));
  text->AppendText(sLogBuff);
  return retCode;
}
int iprintf(const char* format, ...) { // printf, blue
  int retCode = 0;
  wxTextCtrl* text = wxGetApp().LogPane();
  va_list args;
  va_start (args, format);
  retCode = vsprintf(sLogBuff, format, args);
  text->SetDefaultStyle(wxTextAttr(*wxBLUE));
  text->AppendText(sLogBuff);
  return retCode;
}
int eprintf(const char* format, ...) { // fprintf(stderr, red
  int retCode = 0;
  wxTextCtrl* text = wxGetApp().LogPane();
  va_list args;
  va_start (args, format);
  retCode = vsprintf(sLogBuff, format, args);
  text->SetDefaultStyle(wxTextAttr(*wxRED));
  text->AppendText(sLogBuff);
  return retCode;
}
int wprintf(const char* format, ...) { // fprintf(stderr, green
  int retCode = 0;
  wxTextCtrl* text = wxGetApp().LogPane();
  va_list args;
  va_start (args, format);
  retCode = vsprintf(sLogBuff, format, args);
  text->SetDefaultStyle(wxTextAttr(*wxGREEN));
  text->AppendText(sLogBuff);
  return retCode;
}



