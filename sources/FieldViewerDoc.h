/*
 *  FieldViewerDoc.h
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
#ifndef _FieldViewerDoc_H
#define _FieldViewerDoc_H

#include "wx/docview.h"
#include "wx/cmdproc.h"
#include "Listable.h"
#include "GLAList.h"
#include "Model3D.h"
#include "Fields/EField.h"
//#include "FieldView.h"

class GLViewerView;


class FieldViewerDoc: public wxDocument, public Model3D
{
private:
  //
  //  We own the model.
  //
  CGLAList* mList;
  //
  //  And a list of fields. I keep a pair of Listables as the
  //  start and end of the double linked list of fields.
  //
  Listable mFieldBase;
  Listable mFieldEnd;
  //
  //  Then the same for a list of field views.
  //
  Listable mFViewBase;
  Listable mFViewEnd;
  //
  //  Array of tracks.
  //
  int mNTrack;
  PolyLine3D** mTracks;
  int mFirstTrack;
  int mLastTrack;
  //
  //  One field can be 'selected' at any time.
  //
  Listable* mCurrentField;
  //
  //  We also store a pointer to the model viewer.
  //
  GLViewerView* mModelView;
  //
  //  We interact with the fields through a menu and a set of variables
  //  that mark the state of the interaction.
  //
  wxMenu* mFieldMenu;
  bool mLinearTransform;
  int mRainbowLevel;
public:
  //
  //  ctors.
  //
  FieldViewerDoc(void);
  virtual ~FieldViewerDoc(void);
  
  virtual bool IsModified(void) const;
  virtual void Modify(bool mod);
  //
  //  Accessors for the mappers.
  //
  bool IsLinear(void) { return mLinearTransform; };
  int GetNColorCycle(void) { return mRainbowLevel; };
  //
  //  Event handlers must be public and must not be virtual.
  //
  void OnMenuFileLoad3D(wxCommandEvent& WXUNUSED(event));
  void OnMenuFileLoad2D(wxCommandEvent& WXUNUSED(event));
  void OnMenuFieldDelete(wxCommandEvent& WXUNUSED(event));
  void OnMenuFieldLinear(wxCommandEvent& WXUNUSED(event));
  void OnMenuFieldLog(wxCommandEvent& WXUNUSED(event));
  void OnMenuFieldR1(wxCommandEvent& WXUNUSED(event));
  void OnMenuFieldR2(wxCommandEvent& WXUNUSED(event));
  void OnMenuFieldR3(wxCommandEvent& WXUNUSED(event));
  void OnMenuTracksLoad(wxCommandEvent& WXUNUSED(event));
  
  //
  //  OnChoosePlane allows the user to select a plane on which to render a
  //  a field.
  //
  void OnMenuChoosePlane(wxCommandEvent& event);
  //
  //  This one is a bit more sensible and handles segments of planes
  //  parallel to z only.
  //
  void OnMenuChoosePlaneZ(wxCommandEvent& WXUNUSED(event));
  //
  //  Override to implement the Model3D methods.
  //  Render the model for anyone who asks and handle a click that
  //  intersected something in our model.
  //
  virtual void Render(bool picking);
  virtual void DoClick(GLuint names[], Point3D start, Point3D end);
  
protected:
  virtual bool DoOpenDocument(const wxString& filename);
  virtual bool DoSaveDocument(const wxString& filename);
  virtual wxSTD istream& LoadObject(wxSTD istream& stream) { return stream; };
  virtual wxSTD ostream& SaveObject(wxSTD ostream& stream) { return stream; };
  //
  //  LoadField3D is given a filename and tries to read that file in as
  //  a 3D field.
  //
  virtual bool Load3DField(const char* filename);
  //
  //  LoadField2D is the same for a 2D field.
  //
  virtual bool Load2DField(const char* filename);
  //
  //  LoadTracks is given a filename and tries to read that file in as
  //  a text track file.
  //
  virtual bool LoadTracks(const char* filename);
  //
  //  Helper to maintain current field.
  //  Return previous current field.
  //
  Listable* SelectField(Listable* nextCurrent);
  //
  //  Helpers for DoClick.
  //
  Listable* FindView(int num);
  int FindViewName(Listable* target);
  //
  //
  //  These are dialog helpers. They run dialogs and extract their imformation
  //  so that the main dialog method can do its work _after_ the dialog box
  //  closes.
  //  The pointers will only be updated if the function returns true.
  //
  virtual bool GetPlaneInfo(Point3D* pPt, Vector3D* pVec,
                            int* pType, double* pSpacing);
  //
  //  Event and dynamic creation support.
  //
  DECLARE_EVENT_TABLE()
  DECLARE_DYNAMIC_CLASS(FieldViewerDoc);
};


#endif //_FieldViewerDoc_H