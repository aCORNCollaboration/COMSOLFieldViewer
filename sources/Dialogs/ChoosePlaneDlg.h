/*
 *  ChoosePlaneDlg.h
 *  FieldViewer
 *
 *	Dialog box presented to a user when they want to select a plane
 *  on which to show a field quantity.
 *
 *  Converted frm SoundTool:AddSinDlg BCollett 2/26/14
 *  Created by Brian Collett on 6/28/13.
 *  Copyright 2013 Brian Collett. All rights reserved.
 *  BCollett 8/10/15 Add persistent dialog entries and radial
 *  field component.
 *
 */
#ifndef ChoosePlaneDlg_H
#define ChoosePlaneDlg_H

#include "wx/dialog.h"
#include "wx/textctrl.h"
#include "wx/stattext.h"
#include "wx/valtext.h"
#include "wx/sizer.h"
#include "FieldViewerApp.h" // For Control IDs
#include "../Geometry/Geometry3d.h"


class ChoosePlaneDlg : public wxDialog {
	DECLARE_CLASS( ChoosePlaneDlg );
	DECLARE_EVENT_TABLE()
protected:
  //
  /////////////////////////  CLASS VARS  ///////////////////
  //
  //  We store the previous versions of the instance vars in class
  //  vars because new calls are almost always very like old ones.
  //
  static float sPointX;
  static float sPointY;
  static float sPointZ;
  static float sDirX;
  static float sDirY;
  static float sDirZ;
  static float sGridh;
  static int sComponent;
  //
  ///////////////////////// INSTANCE VARS///////////////////
  //
  //  We select a plane by specifying a point (three floats) and
  //  a direction (three floats). We also need a grid spacing
  //  (a float) and an index to select the field component to plot.
  //
  float mPointX;
  float mPointY;
  float mPointZ;
  float mDirX;
  float mDirY;
  float mDirZ;
  float mGridh;
  int mComponent;
  //
  //	I also need strings for each to make the validators work.
  //
  wxString* mPXStr;
  wxString* mPYStr;
  wxString* mPZStr;
  wxString* mDXStr;
  wxString* mDYStr;
  wxString* mDZStr;
  wxString* mGrdStr;
public:
	//
	///////////////////////////// CTORS ///////////////////////
	//
	ChoosePlaneDlg();
	ChoosePlaneDlg(wxWindow* parent,
						wxWindowID id,
						const wxString& title,
						const wxPoint& pos = wxDefaultPosition,
						const wxSize& size = wxDefaultSize,
						long style = wxDEFAULT_DIALOG_STYLE,
						const wxString& name = "Choose Plane");
	bool Create(wxWindow* parent,
							wxWindowID id,
							const wxString& title,
							const wxPoint& pos = wxDefaultPosition,
							const wxSize& size = wxDefaultSize,
							long style = wxDEFAULT_DIALOG_STYLE,
							const wxString& name = "CHoose Plane");
	//
	////////////////////////// ACCESSORS ////////////////////
	//
	Point3D GetPosition(void);
	Vector3D GetDirection(void);
	int GetType(void);
  float GetGridH(void);
	//
	/////////////////////////// PUBLIC METHODS /////////////////////
	//
	//
	/////////////////////////// EVENT HANDLERS /////////////////////
	//
	//
	//////////////////////// INTERNAL USE HELPERS //////////////////
	//
	//
	//	Initialization and setup.
	//
	void Init();
	void CreateControls();
};

#endif

