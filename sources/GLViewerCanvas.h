/*
 *  GLViewerCanvas.h
 *  FieldViewer
 *
 *  The canvas is the wxWidget implementation of the on-screen view of an
 *  OpenGL model. The user interacts with the canvas through a series of
 *  tools that modify the viewpoint. The canvas owns the tools and the
 *  menu to handle them. It does not own anything to render and must ask
 *  the app to do that. Since it needs to talk to the app in order to
 *  install its menu as well as to render any data we keep a pointer
 *  the app as an instance var.
 *
 *  BCollett 2/20/14 Take the menu away and give it to the enclosing Frame
 *  (which already owns the menu bar).
 *  Created by Brian Collett on 9/10/2010 inspired by the Penguin example.
 *
 */
#ifndef _GLViewerCANVAS_H
#define _GLViewerCANVAS_H
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

#include "wx/glcanvas.h"
//
//  Need the app to get all of wxwidgets.
//
#include "FieldViewerApp.h"
#include "GLMouseTools.h"
#include "Model3D.h"
#include "GeometricObjects.h"

//
//  Enum for the different types of tool.
//
enum GLTool {
  kGLToolPtr = 0,
  kGLToolSpin,
  kGLToolPan,
  kGLToolDolly,
  kGLToolZoom,
  kGLToolRegion
};

class GLViewerView;
//
//	Here is the class definition.
//
class GLViewerCanvas : public wxGLCanvas, GLMouseResponder
{
protected:
  //
  //  Instance data.
  //  Start with the view to which we belong, the Context
  //  in which we render, and the model that will do the rendering.
  //
  GLViewerView* mView;
  wxGLContext* mContext;
  Model3D* mModel;
  //
  //	We keep a rather complete set of parameters that describe the
  //	transformation between 3-D space and the the 2-D screen as well
  //	as a pointer to the owning frame (which we do not own).
  //
  Vector3D mCamDir;       // Normalised vector to position of the camera
  Vector3D mUp;           // Camera up direction vector (normalised)
  Vector3D mRight;        // Normalised vector to right of view
  Point3D mCentre;        // Position in space at which we look
  GLfloat mSpinQuat[4];   // Quaternion describing orientation of space
  GLfloat mViewAngle;     // Sets the field of view
  GLfloat mAspect;        // Aspect ratio of viewport
  GLfloat mCamDist;       // Distance from centre to camera
  GLfloat mNear;          // Near plane of view frustum (must be >0)
  GLfloat mFar;           // Far plane of view frustum (must be > mNear)
  GLuint mTexNum;
  bool mInitialized;      // True once GL is properly set up

  //
  //	We keep the tools.
  //
  CGLMouseTool* mPtrTool;
  CSpinTool*    mSpinTool;
  CPanTool*     mPanTool;
  CDollyTool*   mDollyTool;
  CZoomTool*    mZoomTool;
  CRegionTool*  mRegionTool;
  CGLMouseTool* mTools[6];
  CGLMouseTool* mCTool;     // Pointer to current tool
  //
  //  And the rubber-band box
  //
  GLfloat mRBStart[2];
  GLfloat mRBEnd[2];
  bool mRegionActive;
public:
  GLViewerCanvas(GLViewerView* theView,
                 wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxT("GLA Viewer Canvas"));

  ~GLViewerCanvas();
  //
  //  Accessors for view parameters.
  //
  GLfloat GetViewAngle(void) { return mViewAngle; };
  GLfloat GetAspectRatio(void) { return mAspect; };
  GLfloat GetNearDist(void) { return mNear; };
  GLfloat GetFarDist(void) { return mFar; };
  virtual void GetSize(wxSize& theSize);
  //
  //  Install click responder.
  //
  void Install(Model3D* m) { mModel = m; };
  //
  //  This transforms a distance into view space.
  //
  double Project(double dist);
  //
  //  This is  little utility to set the view parameters to give
  //  an optimal view of a particular region of space. We pass in
  //  a Frame3D but add a flag that tells us whether to keep the
  //  view region centred on the origin.
  //
  virtual void FocusOn(Frame3D* box, bool keepCenter = true);
  virtual void FocusOn(Frame3D& box, bool keepCenter = true) { \
    FocusOn(&box, keepCenter); };
  //
  //  And ones to affect the tool itself.
  //
  void SetTool(GLTool theTool) { mCTool = mTools[theTool]; };
  CGLMouseTool* GetTool() { return mCTool; };
  //
  //	Mouse movement helpers to override and thus implement GLMouseResponder.
  //
  virtual void LeftClick(int x, int y);
  virtual void Spin(float quaternion[4]);
  virtual void Pan(float dx, float dy);
  virtual void Dolly(float inOut);
  virtual void Zoom(float inOut);
  virtual void StartRegion(float xs, float ys);
  virtual void Region(float x, float y);
  virtual void EndRegion(float xe, float ye);

protected:
  //
  //  Other handlers, likewise.
  //
  void OnPaint(wxPaintEvent& event);
  void OnSize(wxSizeEvent& event);
  void OnEraseBackground(wxEraseEvent& event);
  void OnMouse(wxMouseEvent& event);
  
private:
  void InitGL();
  void ResetProjectionMode();
  
  DECLARE_EVENT_TABLE()
};

#endif // _GLViewerCANVAS_H


