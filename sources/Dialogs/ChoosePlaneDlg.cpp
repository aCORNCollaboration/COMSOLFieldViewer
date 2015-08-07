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
 *
 */
#include "wx/wxprec.h"
#include "wx/dialog.h"
#include "wx/statline.h"
#include "wx/choice.h"
#include "wx/valgen.h"

#include "ChoosePlaneDlg.h"
//
//	wx macros to define the class and its event table.
//
IMPLEMENT_CLASS( ChoosePlaneDlg, wxDialog );

BEGIN_EVENT_TABLE( ChoosePlaneDlg, wxDialog )
END_EVENT_TABLE()
//
///////////////////////////// CTORS ///////////////////////
//
//	These use the 2-step method no matter how the system was called
//	from the outside. It is rarely neccessary to alter these. Instead
//	alter Init and CreateControls, where the real work is done.
//
//	Default constructor just initialises data variables.
//
ChoosePlaneDlg::ChoosePlaneDlg()
{
	Init();
}
//
//	Full constructor initialises and calls create.
//
ChoosePlaneDlg::ChoosePlaneDlg(wxWindow* parent,
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
bool ChoosePlaneDlg::Create(wxWindow* parent,
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
void ChoosePlaneDlg::Init()
{
  mPointX = 0.0;
  mPointY = 0.0;
  mPointZ = 0.0;
  mDirX = 1.0;
  mDirY = 0.0;
  mDirZ = 0.0;
}
//
//	CreateControls builds the guts of the dialog and connects the
//	controls to the variables.
//	Note that we do NOT normally keep control pointers around outside
//	CreateControls since we interact only through events.
//
void ChoosePlaneDlg::CreateControls()
{
	mPXStr = new wxString(wxT("0.0"));
	mPYStr = new wxString(wxT("0.0"));
	mPZStr = new wxString(wxT("0.0"));
	mDXStr = new wxString(wxT("1.0"));
	mDYStr = new wxString(wxT("0.0"));
	mDZStr = new wxString(wxT("0.0"));
	mGrdStr = new wxString(wxT("0.1"));
//  wxString* vStr = new wxString(wxT("0"));
  wxString viewStrings[] = {
    wxT("Bx"),
    wxT("By"),
    wxT("Bz"),
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
                                          wxT("Direction"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          0);
	wxTextCtrl* dxBox = new wxTextCtrl(this,
                                     bcID_DX,
                                     wxT("1.0"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_RIGHT,
                                     wxTextValidator(wxFILTER_NUMERIC, mDXStr));
	wxStaticText* xLabel = new wxStaticText(this,
                                          wxID_STATIC,
                                          wxT("x"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          0);
	wxBoxSizer* boxx = new wxBoxSizer(wxHORIZONTAL);
  boxx->Add(xLabel, wxALIGN_LEFT);
  boxx->Add(dxBox, wxALIGN_RIGHT);
	wxTextCtrl* dyBox = new wxTextCtrl(this,
                                     bcID_DY,
                                     wxT("0.0"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_RIGHT,
                                     wxTextValidator(wxFILTER_NUMERIC, mDYStr));
	wxTextCtrl* dzBox = new wxTextCtrl(this,
                                     bcID_DZ,
                                     wxT("0.0"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_RIGHT,
                                     wxTextValidator(wxFILTER_NUMERIC, mDZStr));
	wxBoxSizer* boxd = new wxBoxSizer(wxVERTICAL);
	boxd->Add(labeld, 0, wxALIGN_LEFT | wxALL, 5);
	boxd->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL, 0); // Spacer
	boxd->Add(boxx, 0, wxALIGN_RIGHT | wxALL, 5);
	boxd->Add(dyBox, 0, wxALIGN_RIGHT | wxALL, 5);
	boxd->Add(dzBox, 0, wxALIGN_RIGHT | wxALL, 5);
	//
	//	Put into a horizontal sizer.
	//
	wxBoxSizer* box1 = new wxBoxSizer(wxHORIZONTAL);
	box1->Add(boxp, 1, wxALIGN_LEFT | wxALL, 10);
	box1->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL | wxALL, 0);
	box1->Add(boxd, 1, wxALIGN_RIGHT | wxALL, 10);
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
	wxBoxSizer* boxc = new wxBoxSizer(wxHORIZONTAL);
  boxc->Add(boxg, wxALIGN_LEFT);
	boxc->Add(100, 5, 1, wxALIGN_CENTER_HORIZONTAL | wxEXPAND, 0); // Spacer
  boxc->Add(vChoice, wxALIGN_LEFT);
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
Point3D ChoosePlaneDlg::GetPosition(void)
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
  return Point3D(mPointX, mPointY, mPointZ);
}

Vector3D ChoosePlaneDlg::GetDirection(void)
{
	double tmp;
	if (mDXStr->ToDouble(&tmp)) {
		mDirX = tmp;
	} else {
		mDirX = 0.0;
	}
	if (mDYStr->ToDouble(&tmp)) {
		mDirY = tmp;
	} else {
		mDirY = 0.0;
	}
	if (mDZStr->ToDouble(&tmp)) {
		mDirZ = tmp;
	} else {
		mDirZ = 0.0;
	}
  return Vector3D(mDirX, mDirY, mDirZ);
}

int ChoosePlaneDlg::GetType(void)
{
  return mComponent;
}

float ChoosePlaneDlg::GetGridH(void)
{
	double tmp;
	if (mGrdStr->ToDouble(&tmp)) {
		mGridh = tmp;
	} else {
		mGridh = 0.1;
	}
  return mGridh;
}




