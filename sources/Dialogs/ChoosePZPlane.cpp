/*
 *  ChoosePZPlaneDlg.cpp
 *  FieldViewer
 *
 *	Dialog box presented to a user when they want to select a portion
 *  of a plane parallel to the the z axis to show a field.
 *
 *  Converted frm SoundTool:AddSinDlg BCollett 2/26/14
 *  Created by Brian Collett on 6/28/13.
 *  Copyright 2013 Brian Collett. All rights reserved.
 *
 */
#include <float.h>
#include "wx/wxprec.h"
#include "wx/dialog.h"
#include "wx/statline.h"
#include "wx/choice.h"
#include "wx/checkbox.h"
#include "wx/valgen.h"

#include "ChoosePZPlane.h"
//
//	wx macros to define the class and its event table.
//
IMPLEMENT_CLASS( ChoosePZPlaneDlg, wxDialog );

BEGIN_EVENT_TABLE( ChoosePZPlaneDlg, wxDialog )
END_EVENT_TABLE()
//
///////////////////////// CLASS VARS///////////////////
//
//  We save copies of the default dialog entries because
//  the most common use modifies the previous only slightly.
//
float ChoosePZPlaneDlg::sPointX = 0.0;
float ChoosePZPlaneDlg::sPointY = 0.0;
float ChoosePZPlaneDlg::sPointZ = 0.0;
float ChoosePZPlaneDlg::sThetaX = 0.0;
float ChoosePZPlaneDlg::sMinZ = 25.0;
float ChoosePZPlaneDlg::sMaxZ = 30.0;
float ChoosePZPlaneDlg::sMinV = -FLT_MAX;
float ChoosePZPlaneDlg::sMaxV = FLT_MAX;
float ChoosePZPlaneDlg::sGridh = 0.1;
int ChoosePZPlaneDlg::sComponent = 0;
//
///////////////////////////// CTORS ///////////////////////
//
//	These use the 2-step method no matter how the system was called
//	from the outside. It is rarely neccessary to alter these. Instead
//	alter Init and CreateControls, where the real work is done.
//
//	Default constructor just initialises data variables.
//
ChoosePZPlaneDlg::ChoosePZPlaneDlg()
{
	Init();
}
//
//	Full constructor initialises and calls create.
//
ChoosePZPlaneDlg::ChoosePZPlaneDlg(wxWindow* parent,
                               wxWindowID id,
                               const wxString& title,
                               const wxPoint& pos,
                               const wxSize& size,
                               long style,
                               const wxString& name)
{
	Init();
	Create(parent, id, title, pos, size, style, name);
  
}
//
//	Create completes the process.
//
bool ChoosePZPlaneDlg::Create(wxWindow* parent,
                            wxWindowID id,
                            const wxString& title,
                            const wxPoint& pos,
                            const wxSize& size,
                            long style,
                            const wxString& name)
{
	//
	//	If you want to set extra styles then do it here, before
	//	call parental create.
	//
	SetExtraStyle( 0L );
	//
	//	Call parent's create.
	//
	if (!wxDialog::	Create(parent, id, title, pos, size, style, name)) {
		return false;
	}
	//
	//	Build the contents.
	//
	CreateControls();
	//
	//	Centre on parent (or screen).
	//
	Centre();
	return true;
}
//
/////////////////////////// PUBLIC METHODS /////////////////////
//
//
/////////////////////////// EVENT HANDLERS /////////////////////
//
//
//////////////////////// INTERNAL USE HELPERS //////////////////
//
//	Initialization and setup.
//	Init takes care of the instance variables.
//
void ChoosePZPlaneDlg::Init()
{
  mPointX = sPointX;
  mPointY = sPointY;
  mPointZ = sPointZ;
  mThetaX = sThetaX;
  mMinZ = sMinZ;
  mMaxZ = sMaxZ;
  mMinV = sMinV;
  mMaxV = sMaxV;
  mComponent = sComponent;
  mGridh = sGridh;
  mFixRange = false;
}
//
//	CreateControls builds the guts of the dialog and connects the
//	controls to the variables.
//	Note that we do NOT normally keep control pointers around outside
//	CreateControls since we interact only through events.
//
void ChoosePZPlaneDlg::CreateControls()
{
  wxString fmt(wxT("%.2g"));
	mPXStr = new wxString();
	mPYStr = new wxString();
	mPZStr = new wxString();
	mTXStr = new wxString();
	mMinZStr = new wxString();
	mMaxZStr = new wxString();
  mMinVStr = new wxString();
  mMaxVStr = new wxString();
	mGrdStr = new wxString();
  mPXStr->Printf(fmt, mPointX);
  mPYStr->Printf(fmt, mPointY);
  mPZStr->Printf(fmt, mPointZ);
  mTXStr->Printf(fmt, mThetaX);
  mMinZStr->Printf(fmt, mMinZ);
  mMaxZStr->Printf(fmt, mMaxZ);
  mMinVStr->Printf(fmt, mMinV);
  mMaxVStr->Printf(fmt, mMaxV);
  mGrdStr->Printf(fmt, mGridh);
//  wxString* vStr = new wxString(wxT("0"));
  wxString viewStrings[] = {
    wxT("Bx"),
    wxT("By"),
    wxT("Bz"),
    wxT("Br"),
    wxT("|B|")
  };
	//
	//	We are going to need three static text labels and three text
	//	input controls specialised to numbers.
	//
	//	I am going to at least experiment with using sizers. Each of the
	//	input fields consists of a label and a box beside it so that a
	//	horizontal box sizer makes sense there. There are three of these
	//	and they should stack vertically so that means a vertical box sizer
	//	to put them together.
	//	The book seems to create things from the top down but my mind
	//	works more bottom up so that is the way I am going.
	//
	//	First the three coords of the point.
	//
	wxStaticText* labela = new wxStaticText(this,
																					wxID_STATIC,
																					wxT("Point"),
																					wxDefaultPosition,
																					wxDefaultSize,
																					0);
	wxTextCtrl* pxBox = new wxTextCtrl(this,
                                     bcID_PX,
                                     wxT("0.0"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_RIGHT,
                                     wxTextValidator(wxFILTER_NUMERIC, mPXStr));
	wxTextCtrl* pyBox = new wxTextCtrl(this,
                                     bcID_PY,
                                     wxT("0.0"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_RIGHT,
                                     wxTextValidator(wxFILTER_NUMERIC, mPYStr));
	wxTextCtrl* pzBox = new wxTextCtrl(this,
                                     bcID_PZ,
                                     wxT("0.0"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_RIGHT,
                                     wxTextValidator(wxFILTER_NUMERIC, mPZStr));
	wxBoxSizer* boxp = new wxBoxSizer(wxVERTICAL);
	boxp->Add(labela, 0, wxALIGN_LEFT | wxALL, 5);
	boxp->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL, 0);
	boxp->Add(pxBox, 0, wxALIGN_RIGHT | wxALL, 5);
	boxp->Add(pyBox, 0, wxALIGN_RIGHT | wxALL, 5);
	boxp->Add(pzBox, 0, wxALIGN_RIGHT | wxALL, 5);
	//
	//	Then the direction
	//
	wxStaticText* labeld = new wxStaticText(this,
                                          wxID_STATIC,
                                          wxT("Plane spec"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          0);
	wxTextCtrl* dxBox = new wxTextCtrl(this,
                                     bcID_DX,
                                     wxT("1.0"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_RIGHT,
                                     wxTextValidator(wxFILTER_NUMERIC, mTXStr));
	wxStaticText* xLabel = new wxStaticText(this,
                                          wxID_STATIC,
                                          wxT("Theta"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          0);
	wxBoxSizer* boxx = new wxBoxSizer(wxHORIZONTAL);
  boxx->Add(xLabel, wxALIGN_LEFT);
  boxx->Add(dxBox, wxALIGN_RIGHT);
  wxStaticText* zMinLabel = new wxStaticText(this,
                                          wxID_STATIC,
                                          wxT("Min Z"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          0);
	wxTextCtrl* zMinBox = new wxTextCtrl(this,
                                     bcID_DY,
                                     wxT("0.0"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_RIGHT,
                                     wxTextValidator(wxFILTER_NUMERIC, mMinZStr));
  wxBoxSizer* boxzmin = new wxBoxSizer(wxHORIZONTAL);
  boxzmin->Add(zMinLabel, wxALIGN_LEFT);
  boxzmin->Add(zMinBox, wxALIGN_RIGHT);
  wxStaticText* zMaxLabel = new wxStaticText(this,
                                             wxID_STATIC,
                                             wxT("Max Z"),
                                             wxDefaultPosition,
                                             wxDefaultSize,
                                             0);
	wxTextCtrl* zMaxBox = new wxTextCtrl(this,
                                     bcID_DZ,
                                     wxT("0.0"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_RIGHT,
                                     wxTextValidator(wxFILTER_NUMERIC, mMaxZStr));
  wxBoxSizer* boxzmax = new wxBoxSizer(wxHORIZONTAL);
  boxzmax->Add(zMaxLabel, wxALIGN_LEFT);
  boxzmax->Add(zMaxBox, wxALIGN_RIGHT);
	wxBoxSizer* boxd = new wxBoxSizer(wxVERTICAL);
	boxd->Add(labeld, 0, wxALIGN_LEFT | wxALL, 5);
	boxd->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL, 0); // Spacer
	boxd->Add(boxx, 0, wxALIGN_RIGHT | wxALL, 5);
	boxd->Add(boxzmin, 0, wxALIGN_RIGHT | wxALL, 5);
	boxd->Add(boxzmax, 0, wxALIGN_RIGHT | wxALL, 5);
  //
  //  And finally the data range
  //
 /* wxStaticText* labelv = new wxStaticText(this,
                                          wxID_STATIC,
                                          wxT("Data Range"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          0);*/
  mSetRangeCheck = new wxCheckBox(this,
                                  bcID_RANGE_CHECK,
                                  wxT("&Range"),
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  0);
  //
  wxTextCtrl* vMaxBox = new wxTextCtrl(this,
                                     bcID_VMAX,
                                     wxT("1.0"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_RIGHT,
                                     wxTextValidator(wxFILTER_NUMERIC, mMaxVStr));
  wxStaticText* vMaxLabel = new wxStaticText(this,
                                          wxID_STATIC,
                                          wxT("Max"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          0);
  //
  wxBoxSizer* boxv1 = new wxBoxSizer(wxHORIZONTAL);
  boxv1->Add(vMaxLabel, wxALIGN_LEFT);
  boxv1->Add(vMaxBox, wxALIGN_RIGHT);
  //
  wxStaticText* vMinLabel = new wxStaticText(this,
                                             wxID_STATIC,
                                             wxT("Min"),
                                             wxDefaultPosition,
                                             wxDefaultSize,
                                             0);
  wxTextCtrl* vMinBox = new wxTextCtrl(this,
                                     bcID_VMIN,
                                     wxT("0.0"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_RIGHT,
                                     wxTextValidator(wxFILTER_NUMERIC, mMinVStr));
  wxBoxSizer* boxv2 = new wxBoxSizer(wxHORIZONTAL);
  boxv2->Add(vMinLabel, wxALIGN_LEFT);
  boxv2->Add(vMinBox, wxALIGN_RIGHT);
  //
  wxBoxSizer* boxv = new wxBoxSizer(wxVERTICAL);
  boxv->Add(mSetRangeCheck, 0, wxALIGN_RIGHT | wxALL, 0);
  boxv->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL, 0); // Spacer
  boxv->Add(boxv2, 0, wxALIGN_RIGHT | wxALL, 0);
  boxv->Add(boxv1, 0, wxALIGN_RIGHT | wxALL, 0);
	//
	//	Put into a horizontal sizer.
	//
	wxBoxSizer* box1 = new wxBoxSizer(wxHORIZONTAL);
	box1->Add(boxp, 1, wxALIGN_LEFT | wxALL, 0);
//	box1->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL | wxALL, 0);
	box1->Add(boxd, 1, wxALIGN_CENTRE_HORIZONTAL | wxALL, 0);
//  box1->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  box1->Add(boxv, 1, wxALIGN_TOP | wxALL, 0);
  //	box1->Add(boxp, 1, wxALIGN_BOTTOM | wxALL, 10);
  //
  //  Box with the grid size and map type entries.
  //
	wxTextCtrl* gridBox = new wxTextCtrl(this,
                                       bcID_GRD,
                                       wxT("0.01"),
                                       wxDefaultPosition,
                                       wxDefaultSize,
                                       wxTE_RIGHT,
                                       wxTextValidator(wxFILTER_NUMERIC, mGrdStr));
	wxStaticText* gLabel = new wxStaticText(this,
                                          wxID_STATIC,
                                          wxT("h"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          0);
	wxBoxSizer* boxg = new wxBoxSizer(wxHORIZONTAL);
  boxg->Add(gLabel, wxALIGN_LEFT);
  boxg->Add(gridBox, wxALIGN_RIGHT);
  //
  //  Choice of field to plot.
  //
  wxChoice* vChoice = new wxChoice(this,
                                   bcID_VTYPE,
                                   wxDefaultPosition,
                                   wxSize(80, -1),
                                   WXSIZEOF(viewStrings),
                                   viewStrings,
                                   0);
  vChoice->SetSelection(mComponent);
	wxBoxSizer* boxc = new wxBoxSizer(wxHORIZONTAL);
  boxc->Add(boxg, wxALIGN_LEFT);
	boxc->Add(100, 5, 1, wxALIGN_CENTER_HORIZONTAL | wxEXPAND, 0); // Spacer
  boxc->Add(vChoice, wxALIGN_RIGHT);
	//
	//	Wrap that all in an outer sizer
	//
	wxBoxSizer* box2 = new wxBoxSizer(wxVERTICAL);
	box2->Add(box1, 0,  wxALIGN_TOP | wxALL, 10);
  wxStaticLine* sep = new wxStaticLine(this,
                                       wxID_STATIC,
                                       wxDefaultPosition,
                                       wxDefaultSize,
                                       wxLI_HORIZONTAL);
  wxStaticLine* sep2 = new wxStaticLine(this,
                                        wxID_STATIC,
                                        wxDefaultPosition,
                                        wxDefaultSize,
                                        wxLI_HORIZONTAL);
  box2->Add(sep, 0, wxGROW | wxALL);
  box2->Add(boxc, 0, wxALIGN_CENTER_VERTICAL);
  box2->Add(sep2, 0, wxGROW | wxALL);
	box2->Add(CreateButtonSizer(wxOK | wxCANCEL), wxALIGN_BOTTOM | wxALL, 10);
  //
  //  This is used to extract data from the choice.
  //
  FindWindow(bcID_VTYPE)->SetValidator(wxGenericValidator(& mComponent));
	this->SetSizer(box2);
}
//
//	Accessors have to get data out of string.
//
Point3D ChoosePZPlaneDlg::GetPosition(void)
{
	double tmp;
	if (mPXStr->ToDouble(&tmp)) {
		mPointX = tmp;
	} else {
		mPointX = 0.0;
	}
	if (mPYStr->ToDouble(&tmp)) {
		mPointY = tmp;
	} else {
		mPointY = 0.0;
	}
	if (mPZStr->ToDouble(&tmp)) {
		mPointZ = tmp;
	} else {
		mPointZ = 0.0;
	}
  sPointX = mPointX;
  sPointY = mPointY;
  sPointZ = mPointZ;
  return Point3D(mPointX, mPointY, mPointZ);
}

double ChoosePZPlaneDlg::GetAngle(void)
{
	double tmp;
	if (mTXStr->ToDouble(&tmp)) {
		mThetaX = tmp;
	} else {
		mThetaX = 0.0;
	}
  sThetaX = mThetaX;
  return mThetaX;
}

double ChoosePZPlaneDlg::GetMinZ(void)
{
	double tmp;
	if (mMinZStr->ToDouble(&tmp)) {
		mMinZ = tmp;
	} else {
		mMinZ = 0.0;
	}
  sMinZ = mMinZ;
  return mMinZ;
}

double ChoosePZPlaneDlg::GetMaxZ(void)
{
	double tmp;
	if (mMaxZStr->ToDouble(&tmp)) {
		mMaxZ = tmp;
	} else {
		mMaxZ = 0.0;
	}
  sMaxZ = mMaxZ;
  return mMaxZ;
}

double ChoosePZPlaneDlg::GetMinV(void)
{
  double tmp;
  if (mMinVStr->ToDouble(&tmp)) {
    mMinV = tmp;
  } else {
    mMinV = 0.0;
  }
  sMinV = mMinV;
  return mMinV;
}

double ChoosePZPlaneDlg::GetMaxV(void)
{
  double tmp;
  if (mMaxVStr->ToDouble(&tmp)) {
    mMaxV = tmp;
  } else {
    mMaxV = 0.0;
  }
  sMaxV = mMaxV;
  return mMaxV;
}

int ChoosePZPlaneDlg::GetType(void)
{
  sComponent = mComponent;
  return mComponent;
}

float ChoosePZPlaneDlg::GetGridH(void)
{
	double tmp;
	if (mGrdStr->ToDouble(&tmp)) {
		mGridh = tmp;
	} else {
		mGridh = 0.1;
	}
  sGridh = mGridh;
  return mGridh;
}

bool ChoosePZPlaneDlg::GetFixRange(void)
{
  mFixRange = mSetRangeCheck->GetValue();
  return mFixRange;
}



