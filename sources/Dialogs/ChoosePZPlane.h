/*
 *  ChoosePZPlaneDlg.h
 *  FieldViewer
 *
 *	Dialog box presented to a user when they want to select a portion
 *  of a plane parallel to the the z axis to show a field.
 *
 *  BCollett 8/6/15 Add value limiting fields.
 *  Converted frm SoundTool:AddSinDlg BCollett 2/26/14
 *  Created by Brian Collett on 6/28/13.
 *  Copyright 2013 Brian Collett. All rights reserved.
 *
 */
#ifndef ChoosePZPlaneDlg_H
#define ChoosePZPlaneDlg_H

#include "wx/dialog.h"
#include "wx/textctrl.h"
#include "wx/stattext.h"
#include "wx/valtext.h"
#include "wx/checkbox.h"
#include "wx/sizer.h"
#include "FieldViewerApp.h" // For Control IDs
#include "../Geometry/Geometry3d.h"


class ChoosePZPlaneDlg : public wxDialog {
	DECLARE_CLASS( ChoosePZPlaneDlg );
	DECLARE_EVENT_TABLE()
protected:
  //
  ///////////////////////// CLASS VARS///////////////////
  //
  //  We save copies of the default dialog entries because
  //  the most common use modifies the previous only slightly.
  //
  static float sPointX;
  static float sPointY;
  static float sPointZ;
  static float sThetaX;
  static float sMinZ;
  static float sMaxZ;
  static float sMinV;
  static float sMaxV;
  static float sGridh;
  static int sComponent;

  ///////////////////////// INSTANCE VARS///////////////////
  //
  //  We select a plane by specifying a point (three floats) and
  //  a direction (three floats). We also need a grid spacing
  //  (a float) and an index to select the field component to plot.
  //
  float mPointX;
  float mPointY;
  float mPointZ;
  float mThetaX;
  float mMinZ;
  float mMaxZ;
  float mMinV;
  float mMaxV;
  float mGridh;
  int mComponent;
  bool mFixRange;
  //
  //	I also need strings for each to make the validators work.
  //  And a checkbox for the range.
  //
  wxString* mPXStr;
  wxString* mPYStr;
  wxString* mPZStr;
  wxString* mTXStr;
  wxString* mMinZStr;
  wxString* mMaxZStr;
  wxString* mMinVStr;
  wxString* mMaxVStr;
  wxString* mGrdStr;
  wxCheckBox* mSetRangeCheck;
public:
	//
	///////////////////////////// CTORS ///////////////////////
	//
	ChoosePZPlaneDlg();
	ChoosePZPlaneDlg(wxWindow* parent,
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
	double GetAngle(void);
  double GetMinZ(void);
  double GetMaxZ(void);
  double GetMinV(void);
  double GetMaxV(void);
	int GetType(void);
  float GetGridH(void);
  bool GetFixRange(void);
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

