/*
 *  GLViewerCanvas.cpp
 *  TrackViewer
 *
 *	The canvas is the wxWidget implementation of the on-screen view of an
 *	OpenGL model. The user interacts with the canvas through a series of
 *  tools that modify the viewpoint. The canvas owns the tools and the
 *  menu to handle them. It does not own anything to render and must ask
 *  the app to do that. Since it needs to talk to the app in order to
 *  install its menu as well as to render any data we keep a pointer
 *  the app as an instance var.
 *
 *  Modified BCollett 3/20/14 The operation of the virtual trackball is
 *  built around the idea that the camera is looking down the z-axis to
 *  the origin and that the y-axis points up with x to the right.
 *  I want to be able to re-orient by object so that its default position
 *  has us looking down x to the origin with z up and y to the right.
 *  To deal with both viewpoints I add a second rotation matrix that 
 *  reorients the object in the view.
 *  Created by Brian Collett on 9/10/2010.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <stdio.h>
#include <math.h>
#include "FieldViewerApp.h"
#include "GLViewerView.h"
#include "GLViewerFrame.h"
#include "GLViewerCanvas.h"

#include "GeometricObjects.h"
//
//extern "C" {
#include  "trackball.h"
//};

// ---------------------------------------------------------------------------
// GLViewerCanvas
// ---------------------------------------------------------------------------

BEGIN_EVENT_TABLE(GLViewerCanvas, wxGLCanvas)
EVT_SIZE(GLViewerCanvas::OnSize)
EVT_PAINT(GLViewerCanvas::OnPaint)
EVT_ERASE_BACKGROUND(GLViewerCanvas::OnEraseBackground)
EVT_MOUSE_EVENTS(GLViewerCanvas::OnMouse)
END_EVENT_TABLE()
//
//  Constructor
//
GLViewerCanvas::GLViewerCanvas(GLViewerView* theView,
                               wxWindow* parent,
                               wxWindowID id,
                               const wxPoint& pos,
                               const wxSize& size,
                               long style,
                               const wxString& name) : mView(theView),
                  wxGLCanvas(parent,
                             id,
                             nullptr, // attrib list
                             pos,
                             size,
                             style|wxFULL_REPAINT_ON_RESIZE,
                             name),
                  GLMouseResponder()
{
  //
  //  Connect to a context
  //
  mContext = new wxGLContext(this);
  //
  // initialize view parameters
  // Trackball requires camera up +ve z axis with y 'up' and x 'right'.
  //
  mCamDir.Set(0.0, 0.0, -1.0);
  mCamDist = 50.0;
  mUp.Set(0.0, 1.0, 0.0);
  mRight.Set(1.0, 0.0, 0.0);
  mViewAngle = 45.0f;
  mAspect = 1.0f;
  mNear = 2.0f;
  mFar = 250.0f;
  trackball(mSpinQuat, 0.0f, 0.0f, 0.0f, 0.0f);
  mInitialized = false;	// True only after OpenGL is up an running.
  //
  // Create tools.
  //
  mTools[0] = mPtrTool = new CGLMouseTool(this);
  mTools[1] = mSpinTool = new CSpinTool(this);
  mTools[2] = mPanTool = new CPanTool(this);
  mTools[3] = mDollyTool = new CDollyTool(this);
  mTools[4] = mZoomTool = new CZoomTool(this);
  mTools[5] = mRegionTool = new CRegionTool(this);
  mCTool = mPtrTool;  // start with default tool
  mRegionActive = false;
  //
  //  Set up the default tool.
  //
  mCTool = mPtrTool;
  mModel = nullptr;

}

GLViewerCanvas::~GLViewerCanvas()
{
  //
  //	Super takes care of deleting menus and menu bars.
  //
  delete mPtrTool;
  delete mSpinTool;
  delete mPanTool;
  delete mDollyTool;
  delete mZoomTool;
}
//
//  Accessor for sizing.
//
void GLViewerCanvas::GetSize(wxSize& theSize)
{
  theSize = GetClientSize();
}
//
//  This is  little utility to set the view parameters to give
//  an optimal view of a particular region of space. We pass in
//  a Frame3D but add a flag that tells us whether to keep the
//  view region centred on the origin.
//  We adjust the object rotation matrix to put the z axis of
//  the frame vertical.
//
void GLViewerCanvas::FocusOn(const Frame3D* box, bool keepCenter)
{
  //
  //  Start by getting the scale and thus the camera distance.
  //  I first find the size of a sphere big enough to contain the box and
  //  then use that to set camDist, near and far.
  //
  float radius = box->Radius();
  mCamDist = 20.0 * radius;
  mNear = mCamDist - 2.0 * radius;
  mFar = mCamDist + 2.0 * radius;
  //
  //  For the viewer's ease I am putting the z axis vertical and
  //  looking down the x axis so that we see a y-z view of the system.
  //
  trackball(mSpinQuat, 0.0f, 0.0f, 0.0f, 0.0f);
  //
  //  We need to figure out where we should center the view.
  //  Note that this is done in rotated coordinates!
  //
  if (keepCenter) {
    mCentre.Set(0.0, 0.0, 0.0);
  } else {
    Point3D mid = box->GetMid();
    mCentre.mX = mid.mY;
    mCentre.mY = mid.mZ;
    mCentre.mZ = mid.mX;
  }
  //
  //  Then we want to figure out the extent of the box in the Y
  //  and Z directions.
  //
  float halfWidth = 0.5 * box->YSpan();
  float halfHeight = 0.5 * box->ZSpan();
  //
  //  Now we figure out the view angle for the two cases.
  //
#define ToDegrees 57.295779513082325
  float vAngle = 2.0 * ToDegrees * atan2(halfHeight, mCamDist);
  float hAngle = 2.0 * ToDegrees * atan2(halfWidth, mCamDist);
  //
  //  Which we base the final angle on depends on the aspect
  //  ratio of the viewed region to that of the window. OpenGL
  //  views the aspect as width/height and sets the view
  //  from the vertical angle. If our aspect ratio < GLratio
  //  then we should use vAngle as it stands. In either case
  //  we increase the angle a little to give us a little room
  //  round the edges.
  //
//  float aspect = hAngle/vAngle;
  if (hAngle/vAngle <= mAspect) {
    mViewAngle = 1.05 * vAngle;
  } else {
    //
    //  We need to use our hAngle and work out the corresponding
    //  vAngle from the GLratio.
    //
    float newAngle = hAngle / mAspect;
    mViewAngle = 1.05 * newAngle;
  }
  Refresh(false);
}
//
//  Paint has to establish the view params and then render
//  the model.
//
void GLViewerCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
  // must always be here
  wxPaintDC dc(this);
  
  SetCurrent(*mContext);
  /*
   #ifndef __WXMOTIF__
   if (!GetContext()) return;
   #endif
   */
  // Initialize OpenGL
  if (!mInitialized) {
    InitGL();
    ResetProjectionMode();
    mInitialized = true;
  }
  
  // Clear
  glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  //
  //  Adjust the direction vectors according to the current
  //  spin.
  //
	//
	//	Set up viewing system based on current view parameters.
	//
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(mViewAngle, mAspect, mNear, mFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  Point3D eye = mCentre + mCamDir * mCamDist;
	gluLookAt(eye.mX, eye.mY, eye.mZ,
            mCentre.mX, mCentre.mY, mCentre.mZ,
            mUp.mX, mUp.mY, mUp.mZ);
  //
  //  Rotate by current spin.
  //
  GLfloat m[4][4];
  build_rotmatrix( m, mSpinQuat );
  glMultMatrixf( &m[0][0] );
  // 	glTranslatef(mCentre[0], mCentre[1], mCentre[2]);
  //
  //  Rotate to my view direction.
  //
  glRotatef(-120.0, 1.0f, 1.0f, 1.0f);
  glRotatef(180.0, 0.0f, 0.0f, 1.0f);

  glDisable(GL_LIGHTING);
  //
  //	Put in a set of axes just so that we have something to look at.

  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  glBegin(GL_LINES);
  glVertex3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(5.0f, 0.0f, 0.0f);
  glEnd();

  glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
  glBegin(GL_LINES);
  glVertex3f(0.0f, -1.0f, 0.0f);
  glVertex3f(0.0f, 5.0f, 0.0f);
  glEnd();

  glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
  glBegin(GL_LINES);
  glVertex3f( 0.0f,  0.0f, -1.0f);
  glVertex3f( 0.0f,  0.0f, 5.0f);
  glEnd();
/*
  //
  //  Put in a rectangle to play with textures.
  //
  GLenum errNo = glGetError();  // Clears error
  glEnable(GL_TEXTURE_2D);
  if ((errNo = glGetError()) != 0) {
    wprintf("GL Error %x\n at %s line %d\n", errNo, __FILE__, __LINE__);
  }
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  if ((errNo = glGetError()) != 0) {
    wprintf("GL Error %x\n at %s line %d\n", errNo, __FILE__, __LINE__);
  }
  glBindTexture(GL_TEXTURE_2D, mTexNum);
  if ((errNo = glGetError()) != 0) {
    wprintf("GL Error %x\n at %s line %d\n", errNo, __FILE__, __LINE__);
  }
//  glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(2.0f, -1.0f,  -1.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, 1.0f,  -1.0f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, 1.0f,  1.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(2.0f, -1.0f,  1.0f);
  glEnd();
  glDisable(GL_TEXTURE_2D);
*/
  //
  //	Get view to display model.
  //
  mModel->Render(false);

  if (mRegionActive) {
    int w, h;
    GetClientSize(&w, &h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f( mRBStart[0], mRBStart[1]);
    glVertex2f( mRBEnd[0], mRBStart[1]);
    glVertex2f( mRBEnd[0], mRBEnd[1]);
    glVertex2f( mRBStart[0], mRBEnd[1]);
    glVertex2f( mRBStart[0], mRBStart[1]);
    glEnd();
  }

  glPopMatrix();
  //
  // Flush makes sure that all commands are finished before we display.
  //
  glFlush();
  //
  // Swap buffers displays the latest view.
  //
  SwapBuffers();
}

//
//	Menu handlers.
//
void GLViewerCanvas::OnSize(wxSizeEvent& event)
{
  // this is also necessary to update the context on some platforms
  event.Skip(true);
  // Reset the OpenGL view aspect
  ResetProjectionMode();
}

void GLViewerCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
  // Do nothing, to avoid flashing on MSW
}
//
//	Mouse event passes most of its work to tools.
//
void GLViewerCanvas::OnMouse(wxMouseEvent& event)
{
	CMouseTool::DispatchToolEvent(event, mCTool);
}
//
//	Mouse movement helpers.
//
void GLViewerCanvas::LeftClick(int x, int y)
{
  if (mCTool == mPtrTool) {
    //
    //  First we get info about the viewport and translate the y coord
    //  from the screen's top down to the GL's bottom up structure.
    //
    GLint viewport[4];
    GLdouble mvmatrix[16],projmatrix[16];
    GLint realy;            // GL's mapping is upside-down from ours.
    glGetIntegerv(GL_VIEWPORT, viewport);
    realy = viewport[3] - GLint(y) - 1;
//    iprintf("Mouse at %d, %d\n",x,realy);
    //
    //  This is the dread picking code!
    //  Start by setting up a buffer to store the hits.
    //
    GLuint pickBuff[100];
    GLint nHit = -1;
    glSelectBuffer(99, pickBuff+1);
    (void) glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);
    glPushMatrix();
    //
    //  We do the projection matrix twice. First time for the coord
    //  transform and then again for the picking operation.
    //
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(mViewAngle, mAspect, mNear, mFar);
    glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
    glLoadIdentity();
    gluPickMatrix(x, realy, 0.1, 0.1, viewport);
    gluPerspective(mViewAngle, mAspect, mNear, mFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Point3D eye = mCentre + mCamDir * mCamDist;
    gluLookAt(eye.mX, eye.mY, eye.mZ,
              mCentre.mX, mCentre.mY, mCentre.mZ,
              mUp.mX, mUp.mY, mUp.mZ);
    //
    //  Rotate by current spin.
    //
    GLfloat m[4][4];
    build_rotmatrix( m, mSpinQuat );
    glMultMatrixf( &m[0][0] );
    // 	glTranslatef(mCentre[0], mCentre[1], mCentre[2]);
    //
    //  Rotate to my view direction.
    //
    glRotatef(-120.0, 1.0f, 1.0f, 1.0f);
    glRotatef(180.0, 0.0f, 0.0f, 1.0f);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
    //
    //	Get view to display model.
    //
    mModel->Render(true);
    
    glPopMatrix();
    //
    // Flush makes sure that all commands are finished before we display.
    //
    glFlush();
    nHit = glRenderMode(GL_RENDER);
    pickBuff[0] = nHit;
//    iprintf("Found %d hits\n", nHit);
    //
    //  Use the same setup to translate the hit point to 3D coordinates.
    //
    Point3D start, end;     // Returned world coords.
    //
    //  A mouse click can really only tell us about a line in 3D space
    //  since we can't tell where along that line the click occurs.
    //
    gluUnProject(GLdouble(x), GLdouble(realy), 0.0, // Screen space pt
                 mvmatrix, projmatrix, viewport,    // Transform info
                 &start.mX, &start.mY, &start.mZ);  // World space pt
//    iprintf("At z=0 would be %f, %f, %f\n", start.mX, start.mY, start.mZ);
    gluUnProject(GLdouble(x), GLdouble(realy), 1.0,
                 mvmatrix, projmatrix, viewport,
                 &end.mX, &end.mY, &end.mZ);  // World space pt
//    iprintf("At z=1.0 would be %f, %f, %f\n", end.mX, end.mY, end.mZ);
    mModel->DoClick(&pickBuff[0], start, end);
  }
}

void GLViewerCanvas::Pan(float dx, float dy)
{
	//
	//	dx and dy are fractional values that must be converted into fractions
	//	of the GL view as measured at the centre of the viewing region.
	//	We can find the dimensions of space in the view direction with
	//	a little trig based on the distance to the view space and the
	//	view angle.
	//
	Real xSpan = 2.0 * mCamDist * tan(mViewAngle * 0.008726646);
  Real ySpan = xSpan / mAspect;
  
//	mCentre += mRight * 10.0 * dx; // Have to do in two stages because of
//  mCentre += mUp * 10.0 * dy;    // re-use of static var by multiply
  mCentre += mRight * xSpan * dx; // Have to do in two stages because of
  mCentre += mUp * ySpan * dy;    // re-use of static var by multiply
	Refresh(false);
}
void GLViewerCanvas::Dolly(float dy)
{
	//
	//	dy is a fractional values that tells us the fraction by
	//	which to change the distance between camera and eye.
	//
	mCamDist -= dy * mCamDist;
  mNear -= dy * mNear;
  mFar -= dy * mFar;
	Refresh(false);
}
void GLViewerCanvas::Zoom(float dy)
{
	//
	//	dy is a fractional value that tells us the fraction by
	//	which to change the view angle.
	//
	mViewAngle -= dy * mViewAngle;
	Refresh(false);
}
void GLViewerCanvas::Spin(float quaternion[4])
{
	add_quats(quaternion,  mSpinQuat, mSpinQuat);
	
	/* orientation has changed, redraw mesh */
	Refresh(false);
}
void GLViewerCanvas::StartRegion(float xs, float ys){
  mRBStart[0] = xs;
  mRBStart[1] = ys;
  mRBEnd[0] = xs;
  mRBEnd[1] = ys;
  mRegionActive = true;
  Refresh(false);
}
void GLViewerCanvas::Region(float xe, float ye){
  mRBEnd[0] = xe;
  mRBEnd[1] = ye;
  iprintf("Box %f,%f to %f,%f\n", mRBStart[0],mRBStart[1], mRBEnd[0], mRBEnd[1]);
  Refresh(false);
}
void GLViewerCanvas::EndRegion(float xe, float ye){
  mRBEnd[0] = xe;
  mRBEnd[1] = ye;
  mRegionActive = false;
  Refresh(false);
}
//
//	Setup/Takedown helpers.
//
void GLViewerCanvas::InitGL()
{
#ifdef GLUseLighting
  static const GLfloat light0_pos[4]   = { -50.0f, 50.0f, 0.0f, 0.0f };
  
  // white light
  static const GLfloat light0_color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
  
  static const GLfloat light1_pos[4]   = {  50.0f, 50.0f, 0.0f, 0.0f };
  
  // cold blue light
  static const GLfloat light1_color[4] = { 0.4f, 0.4f, 1.0f, 1.0f };
#endif
  /*
   *  A little section to explore what we have.
   */
  const GLubyte* glstr = glGetString(GL_VENDOR);
  char myStr[64];
  for (int i = 0; i < 63; i++) {
    char ch = (char) glstr[i];
    myStr[i] = ch;
    if (ch == 0) break;
  }
  iprintf("Vendor = %s\n", myStr);
  glstr = glGetString(GL_VERSION);
  for (int i = 0; i < 63; i++) {
    char ch = (char) glstr[i];
    myStr[i] = ch;
    if (ch == 0) break;
  }
  iprintf("Version = %s\n", myStr);
  /* remove back faces */
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  
  /* speedups */
  glEnable(GL_DITHER);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

#ifdef GLUseLighting

  /* light */
  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_color);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
  glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_color);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);
#endif
  
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  
  //
  //  Build ouselves a texture that we will use later.
  //  We use colors packed into a single byte.
  //
  GLenum errNo;
  if ((errNo = glGetError()) != 0) {
    wprintf("GL Error %d at %s line %d", errNo, __FILE__, __LINE__);
  }
//  static unsigned char texMap2[4] = { 255, 0, 255, 255};
  static unsigned char texMap4[16] = {
    255, 0, 0, 0,
    0, 255, 0, 0,
    0, 0, 255, 0,
    255, 255, 255, 0,
  };
  glGenTextures(1, &mTexNum);
  if ((errNo = glGetError()) != 0) {
    wprintf("GL Error %d at %s line %d", errNo, __FILE__, __LINE__);
  }
  glBindTexture(GL_TEXTURE_2D, mTexNum);
  if ((errNo = glGetError()) != 0) {
    wprintf("GL Error %d at %s line %d", errNo, __FILE__, __LINE__);
  }
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, texMap4);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texMap4);
  if ((errNo = glGetError()) != 0) {
    wprintf("GL Error %d at %s line %d", errNo, __FILE__, __LINE__);
  }
}

void GLViewerCanvas::ResetProjectionMode()
{
  int w, h;
  GetClientSize(&w, &h);
  {
    
    SetCurrent(*mContext);
    glViewport(0, 0, (GLint) w, (GLint) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
		mAspect = (GLfloat)w/h;
    gluPerspective(mViewAngle, mAspect, mNear, mFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }
}

//
//  This transforms a rectangle into view space.
//
double GLViewerCanvas::Project(double dist)
{
  int w, h;
  GetClientSize(&w, &h);
  double range = 0.5 * (mNear + mFar);
  double wHeight = 2.0 * range * tan(0.5 * mViewAngle / ToDegrees);
  return dist * double(h) / wHeight;
}

