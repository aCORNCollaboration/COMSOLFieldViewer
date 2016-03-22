/*
 *  FieldViewerApp.h
 *  ESViewer2
 *
 *  This is the top level application for 3D scalar/vector
 *  field viewer.
 *  This does very little except set up the main frame and
 *  the document manager to support creating and opening
 *  documents.
 *
 *  Created by Brian Collett on 2/18/14.
 *  BCollett 7/1/14 Add routine to print to logging window.
 *
 */

#ifndef _FIELDVIEWERAPP_H
#define _FIELDVIEWERAPP_H

#include "wx/app.h"
#include "wx/docview.h"
#include <stdarg.h>   // Variable args for printing

#define DOCVIEW_CUT     1
#define DOCVIEW_ABOUT   wxID_ABOUT


class wxDocManager;
class GLViewerFrame;
class MainFrame;

// Define a new application
class FieldViewerApp : public wxApp
{
  static char sLogBuff[512];
protected:
  wxDocManager* mDocManager;
  wxMenu *mFileMenu;
  wxMenu *mEditMenu;
  wxMenu *mHelpMenu;
  wxDocParentFrame* mFrame;
  wxTextCtrl* mLogText;
public:
  FieldViewerApp(void);
  bool OnInit(void);
  int OnExit(void);
  //
  //  Accessor for the logging pane.
  //
  wxTextCtrl* LogPane() { return mLogText; };
  
  GLViewerFrame *CreateChildFrame(wxDocument *doc, wxView *view);
  
  wxDocParentFrame* GetMainFrame() { return mFrame; };
  
};

DECLARE_APP(FieldViewerApp);

//
//	Identifiers for our objects.
//
enum {
  bcID_TOOL_PTR = wxID_HIGHEST+1,
  bcID_TOOL_SPIN,
  bcID_TOOL_PAN,
  bcID_TOOL_DOLLY,
  bcID_TOOL_ZOOM,
  bcID_TOOL_REGION,
  bcID_VIEW_HEDGEHOG,
  bcID_VIEW_ELINES,
  bcID_STATUS_BAR,
  bcID_TOOLBAR,
  bcID_MAINFRAME,
  bcID_GLVIEW,
  bcID_TLISTVIEW,
  bcID_FILE_LOAD3D,
  bcID_FILE_LOAD2D,
  bcID_FIELD_SELPLANE,
  bcID_FIELD_SELPLANEZ,
  bcID_FIELD_DELETE,
  bcID_FIELD_LINEAR,
  bcID_FIELD_LOG,
  bcID_FIELD_R1,
  bcID_FIELD_R2,
  bcID_FIELD_R3,
  bcID_TRACKS_LOAD,
  bcID_CHOOSE_PLANE,
  bcID_CHOOSE_PLANEZ,
  bcID_PX,
  bcID_PY,
  bcID_PZ,
  bcID_DX,
  bcID_DY,
  bcID_DZ,
  bcID_RANGE_CHECK,
  bcID_VMAX,
  bcID_VMIN,
  bcID_GRD,
  bcID_VTYPE
};

//
//  Routines to print to the logging pane.
//
int oprintf(const char* format, ...); // printf, black
int iprintf(const char* format, ...); // printf, blue
int eprintf(const char* format, ...); // fprintf(stderr, red
int wprintf(const char* format, ...); // fprintf(stderr, green

#endif
