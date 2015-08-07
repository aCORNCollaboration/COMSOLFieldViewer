/*
 *  FieldViewerDoc.cpp
 *  FieldViewer
 *
 *  This class is the heart of FieldViewer. It connects to
 *  the file that holds the model and it owns the model and
 *  any fields installed in that model.
 *
 *  Created by Brian Collett on 2/18/14.
 *  BCollett 7/7/14 Adding support for Field menu and different renderings
 *  of the field.
 *  BCollett 7/17/14 Add support for Pointer click handling through the
 *  Pointer3D class.
 *
 */

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/filename.h"
#include "wx/filefn.h"


#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#if wxUSE_STD_IOSTREAM
#include "wx/ioswrap.h"
#else
#include "wx/txtstrm.h"
#endif

#include <stdio.h>

#include "FieldViewerDoc.h"
#include "GLViewerView.h"

#include "Scanner/CSymbolTable.h"
#include "Scanner/CTextScanner.h"
#include "COMSOLData3D.h"
#include "Dialogs/ChoosePlaneDlg.h"
#include "Dialogs/ChoosePZPlane.h"
#include "GLAList.h"
#include "CD3DField.h"
#include "FieldView.h"
#include "ReadField.h"


IMPLEMENT_DYNAMIC_CLASS(FieldViewerDoc, wxDocument)

//
//  Event table.
//
BEGIN_EVENT_TABLE(FieldViewerDoc, wxDocument)
EVT_MENU(bcID_FILE_LOAD3D, FieldViewerDoc::OnMenuFileLoad3D)
EVT_MENU(bcID_FIELD_SELPLANE, FieldViewerDoc::OnMenuChoosePlane)
EVT_MENU(bcID_FIELD_SELPLANEZ, FieldViewerDoc::OnMenuChoosePlaneZ)
EVT_MENU(bcID_FIELD_DELETE, FieldViewerDoc::OnMenuFieldDelete)
EVT_MENU(bcID_FIELD_LINEAR, FieldViewerDoc::OnMenuFieldLinear)
EVT_MENU(bcID_FIELD_LOG, FieldViewerDoc::OnMenuFieldLog)
EVT_MENU(bcID_FIELD_R1, FieldViewerDoc::OnMenuFieldR1)
EVT_MENU(bcID_FIELD_R2, FieldViewerDoc::OnMenuFieldR2)
EVT_MENU(bcID_FIELD_R3, FieldViewerDoc::OnMenuFieldR3)
END_EVENT_TABLE()

//
//  ctors.
//
FieldViewerDoc::FieldViewerDoc(void)
{
  CGLAList::InitClass(20);
  mList = nullptr;
  mFieldBase.mNext = &mFieldEnd;
  mFieldBase.mPrev = nullptr;
  mFieldEnd.mPrev = &mFieldBase;
  mFieldEnd.mNext = nullptr;
  mFViewBase.mNext = &mFViewEnd;
  mFViewBase.mPrev = nullptr;
  mFViewEnd.mPrev = &mFViewBase;
  mFViewEnd.mNext = nullptr;
  mCurrentField = nullptr;
  mFieldMenu = nullptr;
  mLinearTransform = true;
  mRainbowLevel = 1;
}
FieldViewerDoc::~FieldViewerDoc(void)
{
  Listable* next = nullptr;
  if (mList) delete mList;
  for (Listable* f = mFieldBase.mNext; f != &mFieldEnd; f = next) {
    next = f->mNext;
    delete f;
  }
  for (Listable* f = mFViewBase.mNext; f != &mFViewEnd; f = next) {
    next = f->mNext;
    delete f;
  }
}
//
//  Since text windows have their own method for
//  saving to/loading from files, we override
//  DoSave/OpenDocument instead of Save/LoadObject
//
bool FieldViewerDoc::DoSaveDocument(const wxString& filename)
{
//    FieldViewerView *view = (FieldViewerView *)GetFirstView();
//    return view->textsw->SaveFile(filename);
  return true;
}

bool FieldViewerDoc::DoOpenDocument(const wxString& filename)
{
//  bool success = false;
  mModelView = wxDynamicCast(GetFirstView(), GLViewerView);
/*
  //
  //  Split the name into file part and directory part.
  //  CD to the directory and then open the file with only
  //  the local part of the name. This way subsequent
  //  file operations will be in the right directory.
  //
  wxFileName fullPath(filename);
  success = wxSetWorkingDirectory(fullPath.GetPath());
  //
  //  Open the file for reading.
  //
  FILE* ifp = fopen(fullPath.GetFullName(), "rt");
 */
  FILE* ifp = fopen(filename, "rt");
  if (ifp == nullptr) {
    return false;
  }
  //
  //	Create a TextScanner to analyse the file.
  //
  CTextScanner scan(ifp);
  //
  //	Pass that to the display list. It will read the
  //	file and create the OpenGL display list from the
  //	contents of the file.
  //
  mList = new CGLAList(&scan);
  mList->Create();
  //
  //  Create will stop when it hits an end directive. The rest of
  //  the file should contain a description of a nested set of fields.
  //
  CD3Data* fData = new CD3Data();
  if (ParseFieldSet(fData, ifp)) {
    CD3DField* f = new CD3DField(fData);
    mFieldBase.Append(f);
    mModelView->FocusOn(f->GetBounds(), false);
    UpdateAllViews();
  } else {
    wxLogMessage("Attempt to load model fields failed.");
    fclose(ifp);
    return false;
  }
  fclose(ifp);
  //
  //  At this point we can be certain that the frame exists. Create
  //  a field menu an install it.
  //
  mFieldMenu = new wxMenu;
  mFieldMenu->Append(bcID_FIELD_SELPLANE, wxT("Plot &field\tCtrl-F"));
  mFieldMenu->Append(bcID_FIELD_SELPLANEZ, wxT("Plot &Z plane\tCtrl-Z"));
  mFieldMenu->Append(bcID_FIELD_DELETE, wxT("Delete field\tCtrl-X"));
  mFieldMenu->AppendSeparator();
//  mFieldMenu->AppendRadioItem(bcID_FIELD_LINEAR, wxT("L&inear map\tCtrl-I"));
//  mFieldMenu->AppendRadioItem(bcID_FIELD_LOG, wxT("L&og map\tCtrl-G"));
  mFieldMenu->AppendCheckItem(bcID_FIELD_LINEAR, wxT("L&inear map\tCtrl-I"));
  mFieldMenu->Check(bcID_FIELD_LINEAR, true);
  mFieldMenu->AppendCheckItem(bcID_FIELD_LOG, wxT("L&og map\tCtrl-G"));
  mFieldMenu->Check(bcID_FIELD_LOG, false);
  mFieldMenu->AppendSeparator();
  mFieldMenu->AppendRadioItem(bcID_FIELD_R1, wxT("Heat map\tCtrl-A"));
  mFieldMenu->AppendRadioItem(bcID_FIELD_R2, wxT("Rainbow\tCtrl-B"));
  mFieldMenu->AppendRadioItem(bcID_FIELD_R3, wxT("Grey map\tCtrl-D"));
  mModelView->mFrame->InsertMenu(mFieldMenu, wxT("Field"),2);
  return true;
}
//
//  Load3DField is given a filename and tries to read that file in as
//  a 3D field.
//  Modified 3/13/14 to build from a binary file not a text file.
//
bool FieldViewerDoc::Load3DField(const char* filename)
{
  CD3Data* fData = new CD3Data();
  FILE* ifp = fopen(filename, "rb");
  if (nullptr == ifp) {
    wxLogMessage("Failed to open file.");
    return false;
  }
  bool success = CD3ReadBinary(fData, ifp);
  if (!success) {
    wxLogMessage("CD3ReadBinary failed.");
    return false;
  }
  fclose(ifp);
  //
  //  So we have the field data. Build the bounding box and
  //  package it all up as a GridField.
  //
  CD3DField* f = new CD3DField(fData);
  mFieldBase.Append(f);
  UpdateAllViews();
  mModelView->mFrame->EnableFileItem(bcID_FIELD_SELPLANE, true);
  return true;
}
//
//  Load2DField does the same for 2D field.
//
bool FieldViewerDoc::Load2DField(const char* filename)
{
  return false;
}

//
//  Render the model for anyone who asks.
//
void FieldViewerDoc::Render(bool picking)
{
  GLuint viewNum = 0;
  Listable* l;
  EField* f = nullptr;
  FieldView* v = nullptr;
  if (!picking) {
    if (mList != nullptr) {
      mList->CallList();
    }
    for (l = mFieldBase.mNext; l != &mFieldEnd; l = l->mNext) {
      if (nullptr != (f = dynamic_cast<EField*>(l))) {
        f->GetBounds()->Draw();
      }
    }
  }
  for (l = mFViewBase.mNext; l != &mFViewEnd; l = l->mNext) {
    if (nullptr != (v = dynamic_cast<FieldView*>(l))) {
      if (picking) {
        iprintf("View %d\n",viewNum);
        glLoadName(viewNum++);
      }
      v->Draw();
    }
  }
}
//
//  Since FieldViewer can only LOOK AT fields and models
//  we never have to worry about things being modified.
//  IsModified can always return false and Modify does
//  nothing at all.
//
bool FieldViewerDoc::IsModified(void) const
{
  return false;
}

void FieldViewerDoc::Modify(bool mod)
{
}
//
//  OnMenuFileLoad3D prompts the user to select a field file
//  and then adds that field to the model.
//
void FieldViewerDoc::OnMenuFileLoad3D(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog openFileDialog(mModelView->mFrame,
                              _("Load binary field file"),
                              "",
                              "",
                              "COMSOL files (*.bin)|*.bin",
                              wxFD_OPEN|wxFD_FILE_MUST_EXIST);
  if (openFileDialog.ShowModal() == wxID_CANCEL) {
    return;     // the user changed their mind...
  }
  //
  //  All is well. Let's try this thing.
  //  Hide the dialog first though.
  //
  openFileDialog.Show(0);
  if (!Load3DField(openFileDialog.GetPath())) {
    wxMessageBox("Unable to open field file.");
    return;
  }
  mModelView->mFrame->EnableFileItem(bcID_FIELD_SELPLANE, true);
}
//
//  OnMenuFileLoad2D prompts the user to select a field file
//  and then adds that field to the model.
//
void FieldViewerDoc::OnMenuFileLoad2D(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog openFileDialog(mModelView->mFrame, _("Load field file"), "", "",
                              "test files (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
  if (openFileDialog.ShowModal() == wxID_CANCEL)
    return;     // the user changed their mind...
  if (!Load2DField(openFileDialog.GetPath())) {
    wxMessageBox("Unable to open field file.");
  }
}
//
//  OnChoosePlane allows the user to select a plane on which to render a
//  a field.
//
void FieldViewerDoc::OnMenuChoosePlane(wxCommandEvent& event)
{
  Point3D p;
  Vector3D v;
  int type;
  double spacing;
  //
  //	Extract the data from the dialog.
  //
  if (GetPlaneInfo(&p, &v, &type, &spacing)) {
    v.Normalize();
    FieldView* fv = new FieldView(mModelView->mGLWind,
                                  dynamic_cast<EField*>(mFieldBase.mNext),
                                  dynamic_cast<FieldViewerDoc*>(this));

    SelectField(fv);
    if (fv->ViewPlane(p, v) == 0) {
      fv->ViewType(type, spacing);
      mFViewBase.Append(fv);
      UpdateAllViews();
    } else {
      wxMessageBox(wxT("Plane does not intersect bounds of field."));
    }
  }
}
//
//  This one is a bit more sensible and handles segments of planes
//  parallel to z only.
//
void FieldViewerDoc::OnMenuChoosePlaneZ(wxCommandEvent& WXUNUSED(event))
{
	ChoosePZPlaneDlg theDlg(mModelView->mFrame,
                          bcID_CHOOSE_PLANEZ,
                          wxT("Choose a plane section to plot"));
	int retVal = theDlg.ShowModal();
	if (retVal == wxID_CANCEL) {
		return;
	} else if (retVal == wxID_OK) {
		//
		//	Extract the data from the dialog.
		//
    Point3D p = theDlg.GetPosition();
    double angle = theDlg.GetAngle();
    double zMin = theDlg.GetMinZ();
    double zMax = theDlg.GetMaxZ();
    FieldView* fv = new FieldView(mModelView->mGLWind,
                                  dynamic_cast<EField*>(mFieldBase.mNext),
                                  this);
    SelectField(fv);
    if (theDlg.GetFixRange()) {
      fv->SetDataRange(theDlg.GetMinV(), theDlg.GetMaxV());
    }
    if (fv->ViewPlane(p, angle, zMin, zMax) == 0) {
      int type = theDlg.GetType();
      double spacing = theDlg.GetGridH();
      iprintf("Spacing = %f\n", spacing);
      fv->ViewType(type, spacing);
      mFViewBase.Append(fv);
      UpdateAllViews();
    } else {
      wxMessageBox(wxT("Plane does not intersect bounds of field."));
    }
	}
}
void FieldViewerDoc::OnMenuFieldDelete(wxCommandEvent& WXUNUSED(event))
{
  if (mCurrentField != nullptr) {
    mCurrentField->Delete();
    delete mCurrentField;
    mCurrentField = nullptr;
  }
  UpdateAllViews();
}
void FieldViewerDoc::OnMenuFieldLinear(wxCommandEvent& WXUNUSED(event))
{
  mLinearTransform = true;
  mFieldMenu->Check(bcID_FIELD_LINEAR, true);
  mFieldMenu->Check(bcID_FIELD_LOG, false);
  UpdateAllViews();
}
void FieldViewerDoc::OnMenuFieldLog(wxCommandEvent& WXUNUSED(event))
{
  mLinearTransform = false;
  mFieldMenu->Check(bcID_FIELD_LINEAR, false);
  mFieldMenu->Check(bcID_FIELD_LOG, true);
  UpdateAllViews();
}
void FieldViewerDoc::OnMenuFieldR1(wxCommandEvent& WXUNUSED(event))
{
  mRainbowLevel = 1;
  mFieldMenu->Check(bcID_FIELD_R1, true);
  mFieldMenu->Check(bcID_FIELD_R2, false);
  mFieldMenu->Check(bcID_FIELD_R3, false);
  UpdateAllViews();
}

void FieldViewerDoc::OnMenuFieldR2(wxCommandEvent& WXUNUSED(event))
{
  mRainbowLevel = 2;
  mFieldMenu->Check(bcID_FIELD_R1, false);
  mFieldMenu->Check(bcID_FIELD_R2, true);
  mFieldMenu->Check(bcID_FIELD_R3, false);
  UpdateAllViews();
}

void FieldViewerDoc::OnMenuFieldR3(wxCommandEvent& WXUNUSED(event))
{
  mRainbowLevel = 3;
  mFieldMenu->Check(bcID_FIELD_R1, false);
  mFieldMenu->Check(bcID_FIELD_R2, false);
  mFieldMenu->Check(bcID_FIELD_R3, true);
  UpdateAllViews();
}

//
//  These are dialog helpers. They run dialogs and extract their imformation
//  so that the main dialog method can do its work _after_ the dialog box
//  closes.
//  The pointers will only be updated if the function returns true.
//
bool FieldViewerDoc::GetPlaneInfo(Point3D* pPt,
                                  Vector3D* pVec,
                                  int* pType,
                                  double* pSpacing)
{
	ChoosePlaneDlg theDlg(mModelView->mFrame, bcID_CHOOSE_PLANE,
                        wxT("Choose a plane to plot"));
	int retVal = theDlg.ShowModal();
	if (retVal == wxID_CANCEL) {
		return false;
	} else if (retVal == wxID_OK) {
		//
		//	Extract the data from the dialog.
		//
    *pPt = theDlg.GetPosition();
    *pVec = theDlg.GetDirection();
    *pType = theDlg.GetType();
    *pSpacing = theDlg.GetGridH();
    return true;
  }
  return false;
}

//
//  Helper to maintain current field.
//  Return previous current field.
//
Listable* FieldViewerDoc::SelectField(Listable* nextCurrent)
{
  Listable* oldCurrent = mCurrentField;
  if (oldCurrent != nullptr) {
    oldCurrent->Deselect();
  }
    if (nextCurrent){
      mCurrentField = nextCurrent;
      nextCurrent->Select();
    }
  return oldCurrent;
}

//
//  Private helper for DoClick. It is given a view number and
//  translates it to the corresponding Listable.
//
Listable* FieldViewerDoc::FindView(int num)
{
  //
  //  For each name we have to walk the list of views to
  //  translate the name into a View*.
  //
  int viewNum = 1;
  for (Listable* l = mFViewBase.mNext; l != &mFViewEnd; l = l->mNext) {
    if (viewNum++ == num) {
      return l;
    }
  }
  return nullptr;
}
//
//  This one does the opposite. It finds the number given the
//  listable or 0 if the listable is not there.
//
int FieldViewerDoc::FindViewName(Listable* target)
{
  int viewNum = 1;
  if (target == nullptr) return 0;
  for (Listable* l = mFViewBase.mNext; l != &mFViewEnd; l = l->mNext) {
    if (l == target) {
      return viewNum;
    }
    viewNum++;
  }
  return 0;
}
//
//  Handler for clicks in the 3D view.
//
void FieldViewerDoc::DoClick(GLuint names[], Point3D start, Point3D end)
{
  //
  //  First we have to worry about what has been clicked.
  //  We should only have been called if _something_ was hit.
  //  If the current selection is not in the list then we select
  //  the first item on the list.
  //
  int selName = FindViewName(mCurrentField);
  if (selName == 0) { // No sel or not in list
    Listable* newSel = FindView(names[1]);
    SelectField(newSel);
  }
  FieldView* view = (FieldView*) mCurrentField;
  if (nullptr == view) return;
  //
  //  Right, now we find the intersection of the selected view with the
  //  the click line.
  //  Start by the vector u that points along the line and the vector
  //  w that points from the min corner of the plane to the start of the line.
  //
  Vector3D u = end - start;
  Vector3D w = start - view->mFrame->BottomLeft();
  //
  //  Then need dot of u and w with plane normal.
  //
  Vector3D norm = view->mFrame->GetNormal().Norm();
  Real D = u.Dot(norm);
  Real N = -w.Dot(norm);
  if (fabs(D) < 1e-3) {
    //
    //  Line parallel to plane. We can't have an intersection.
    //  This ought to be impossible.
    //
    wprintf("Can't find intersection point in plane parallel to line of sight.\n");
    return;
  }
  Real lambda = N/D;
  Point3D ip =  start + u * lambda;
  const char* name = view->mField->FieldNameAt(ip);
  iprintf("Click at %f,%f,%f in field \n%s\n", ip.mX, ip.mY,ip.mZ, name);
  Vector3D E = view->mField->FieldAt(ip);
  iprintf("E = %f,%f,%f\n", E.mX, E.mY, E.mZ);
  
}

