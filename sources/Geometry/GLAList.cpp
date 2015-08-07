/*
 *	GLAList.cpp
 *
 *	A CGLAList is a special kind of DisplayList that gets its
 *	contents from a text file with the extension .gla.
 *	It can parse a file with the syntax
 *
 *	color <r> <g> <b>
 *	line <xs> <ys> <zs> <xe> <ye> <ze>
 *	polyline <n> <x1> <y1> <z1> .... <yn> <zn>
 *	sphere <r> <x> <y> <z>
 *	box <xs> <ys> <zs> <xe> <ye> <ze>
 *	translate <dx> <dy> <dz>
 *  cylinder 
 *  cap
 *  fields ---GLA list ignores everything after this!
 *
 *	Where the values in angle brackets are floating point numbers.
 *
 *	Each line has the form of a keyword followed by a list
 *	of real numbers. The meanings of those numbers are
 *	assigned by different keywords.
 *
 *	BCollett 9/99
 *	This gets text from a TextScanner that it expects to be ready
 *	for use.
 *	BCollett 9/10/99 add spheres and boxes.
 *	BCollett 1/26/04 Redesign with argument list and class symbol table.
 *  BCollett 3/14/14 Add support for caps and fields for FieldViewer.
 */

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <stdio.h>
#include "FieldViewerApp.h"
#include "GLAList.h"
#include "../Scanner/CTextScanner.h"

#ifdef DEBUG
#undef DEBUG
#endif

#ifdef DEBUG
static char gMsgBuff[1024];
#endif
//
//	Class variables.
//
CSymbolTable* CGLAList::gSymTab = NULL;
GLfloat* CGLAList::gArguments = NULL;
int CGLAList::gMaxNArgs = 0;
//
//	Class initialiser makes sure that class vars get set up.
//
bool CGLAList::InitClass(int nArg) {
	//
	//	Build a symbol table and initialise it with the OpenGL
	//	commands that we recognise.
	//
	gSymTab = new CSymbolTable();
	if (NULL == gSymTab) {
		return false;
	}
	gSymTab->InsertName("COLOR", kGLColor);
	gSymTab->InsertName("POINT", kGLPoint);
	gSymTab->InsertName("LINE", kGLLine);
	gSymTab->InsertName("POLYLINE", kGLPolyLine);
	gSymTab->InsertName("SPHERE", kGLSphere);
	gSymTab->InsertName("TRIANGLE", kGLTriangle);
	gSymTab->InsertName("BOX", kGLBox);
	gSymTab->InsertName("TRANSLATE", kGLTranslate);
	gSymTab->InsertName("CYLINDER", kGLCylinder);
	gSymTab->InsertName("CAP", kGLCap);
	gSymTab->InsertName("cap", kGLCap);
	gSymTab->InsertName("cylinder", kGLCylinder);
	gSymTab->InsertName("color", kGLColor);
	gSymTab->InsertName("point", kGLPoint);
	gSymTab->InsertName("line", kGLLine);
	gSymTab->InsertName("polyline", kGLPolyLine);
	gSymTab->InsertName("sphere", kGLSphere);
	gSymTab->InsertName("triangle", kGLTriangle);
	gSymTab->InsertName("box", kGLBox);
	gSymTab->InsertName("translate", kGLTranslate);
	gSymTab->InsertName("END", kGLEnd);
	gSymTab->InsertName("end", kGLEnd);
	//
	//	Get space for arguments and save number.
	//
	gArguments = new GLfloat[nArg];
	if (NULL == gArguments) {
		return false;
	}
	gMaxNArgs = nArg;
	return true;
}
//
//	ReleaseClass frees the class storage.
//
void CGLAList::ReleaseClass() {
	delete gSymTab;
	delete[] gArguments;
}
//
//	Constructor must be passed a TextScanner. It may be NULL
//	in which case you should call InstallScanner before
//	calling Create.
//
CGLAList::CGLAList(CTextScanner* newScan) : CDisplayList(kDLCompile) {
	mScan = newScan;
}
//
//	Destructor does nothing as we own no storage..
//
CGLAList::~CGLAList() {
}
//
//	Override the abstract Create function to make this
//	a real class.
//	Create works its way through the text in the scanner one
//	line at a time. Each line must contain a command and a set
//	of arguments. Create examines the command and then calls
//	on helper functions to do the right things with the arguments.
//
bool CGLAList::Create() {
	if (mScan == NULL) {	// Quit if there is no scanner
		return false;
	}
	// 
	//	Work through the file.
	//	Each line should begin with a command and then be
	//	followed by a set of numeric arguments.
	//
	Lexeme* cLex;
	CSymbol* cSym = NULL;
  bool finished = false;
	Start();
	while ((cLex = mScan->NextLex())->lex != LEof) {
    if (cLex->lex == LNewLine || cLex->lex==LSpace) continue;
		if (cLex->lex == LWord) {	// Hope we have a command
#ifdef DEBUG
eprintf("Word %s ", cLex->sVal);
wxLogMessage(gMsgBuff);
#endif
			cSym = gSymTab->LookUpWord(cLex->sVal);
			//
			//	Now read in the arguments.
			//
			int nArg = GetArgList();
#ifdef DEBUG
      eprintf("Found %d arguments",nArg);
      wxLogMessage(gMsgBuff);
				for (int ii = 0; ii < nArg; ++ii) {
          eprintf(" %f", gArguments[ii]);
          wxLogMessage(gMsgBuff);
				}
      wxLogMessage("\r\n");
#endif
			if (cSym != NULL) {
				//
				//	Found a valid command, dispatch a function
				//	to handle it.
				//
#ifdef DEBUG
        eprintf("matches symbol %d\r\n",cSym->mSym);
        wxLogMessage(gMsgBuff);
#endif
				switch (cSym->mSym) {
					case kGLColor:
						if (nArg >= 3) {
							GLfloat color[4];
							color[0] = gArguments[0];
							color[1] = gArguments[1];
							color[2] = gArguments[2];
							if (nArg > 3) {
								color[3] = gArguments[3];
							} else {
								color[3] = 1.0f;
							}
							glMaterialfv(GL_FRONT, GL_AMBIENT, color);
							glMaterialfv(GL_FRONT, GL_SPECULAR, color);
              glColor3fv(color);
						}
						break;

					case kGLPoint:
						if (nArg >= 3) {
							glBegin(GL_POINTS);
							glVertex3fv(gArguments);
							glEnd();
							mBounds->AddPoint3fv(&gArguments[0]);
						}
						break;

					case kGLTranslate:
						if (nArg >= 3) {
							glTranslatef(gArguments[0],gArguments[1],gArguments[2]);
						}
						break;

					case kGLLine:
						BuildLine(nArg);
						break;

					case kGLPolyLine:
						BuildPolyLine(nArg);
						break;

					case kGLSphere:
						BuildSphere(nArg);
						break;

					case kGLBox:
						BuildBox(nArg);
						break;

					case kGLTriangle:
						BuildTriangle(nArg);
						break;
            
          case kGLCylinder:
            BuildCylinder(nArg);
            break;

          case kGLCap:
            BuildCap(nArg);
            break;
            
          case kGLEnd:
            finished = true;
            break;
            
					default:
            eprintf("CGLAList.Create: Unimplemented GLA verb %s.\r\n", cSym->mName);
						break;
				}
			} else {
        eprintf("CGLAList.Create: %s is an unrecognised GLA verb.\r\n", cLex->sVal);
			}
		} else {	// Did not find a command
			//
			//	Print a message.
			//
      eprintf("CGLAList.Create: Cound not find a command on line %lu.\r\n", mScan->LineNumber());
      eprintf("%s", mScan->GetLine());
		}
    if (finished) break;
		//
		//	Eat tokens til we get to the
		//	end of the line (or a premature EOF)
		//
		while ((cLex = mScan->NextLex())->lex != LNewLine && cLex->lex != LEof);
		} // end while
	End();
	ReleaseScanner();
	return true;
}
//
//	Functions to install and release the scanner. Create automatically
//	releases the scanner when it is finished with it. These are for
//	completeness.
//
void CGLAList::InstallScanner(CTextScanner* newScan) {
	mScan = newScan;
}
void CGLAList::ReleaseScanner() {
	mScan = NULL;
}
//
//	Helper functions used internally.
//	GetArgList scans the rest of the current line for numbers which
//	it puts into the argument array.
//
int CGLAList::GetArgList() {
	//
	//	Reads three space separated numbers into the three slots
	//	in thePoint.
	//
	Lexeme* cLex;
	int argNum = 0;
	while ((cLex = mScan->NextLex())->lex == LReal || (cLex->lex == LInteger)) {
		gArguments[argNum++] = (float) cLex->nVal;
		//
		//	Ignore any separators
		//
		cLex = mScan->NextLex();
		if (cLex->lex < LPunctuation) {
			mScan->ReturnSym();
		}
	}
	if (cLex->lex != LNewLine) {
wxLogMessage("CGLAList.Create: Expected end of line!\n");
		return -argNum;
	}
	mScan->ReturnSym();
	return argNum;
}
//
//	BuildLine reads two float triplets in and uses them to construct
//	an OpenGL line.
//
bool CGLAList::BuildLine(int nArg) {
	bool valid = false;
	if (nArg >= 6) {
		glBegin(GL_LINES);
		glVertex3fv(&gArguments[0]);
		glVertex3fv(&gArguments[3]);
		glEnd();
		mBounds->AddPoint3fv(&gArguments[0]);
		mBounds->AddPoint3fv(&gArguments[3]);
		valid = true;
	} else {
wxLogMessage("LINE needs 6 floats to make two end points.\n");
	}
	return valid;
}
//
//	BuildPolyLine reads a number that tells us the number of points
//	in the line and then reads that many triplets into the vertices
//	of an OpenGL polyline.
//
bool CGLAList::BuildPolyLine(int nArg) {
	//
	//	First thing is the number of points in the line.
	//
	if (nArg > 1) {
		int nPoint = (int) gArguments[0];
		if (nArg - 1 != 3 * nPoint) {
      eprintf("Not enough arguments. To make %d points need %d args.\n",
              nPoint,3*nPoint);
			return false;
		}
		glBegin(GL_LINE_STRIP);
		for (int p = 1; p <3*nPoint; ++p) {
			glVertex3fv(&gArguments[p]);
			mBounds->AddPoint3fv(&gArguments[p]);
		}
		glEnd();
		return true;
	} else {
wxLogMessage("Polyline command does not have number of points.\r\n");
	}
	return false;
}
//
//	BuildSphere reads in a radius and centre position and
//	accepts a number of slices if there is one.
//	It creates a GLUSphere and moves it into position.
//
bool CGLAList::BuildSphere(int nArg) {
	if (nArg >= 4) {
		float radius = gArguments[0];
		glPushMatrix();
		 glTranslatef(gArguments[1],gArguments[2],gArguments[3]);
		 GLUquadricObj *glq = gluNewQuadric();
		 if (nArg >= 5) {
			gluSphere(glq, radius, (int) gArguments[4], (int) gArguments[4]);
		 } else {
			gluSphere(glq, radius, 20, 20);
		 }
		glPopMatrix();
		return true;
	} else {
wxLogMessage("Need at least 4 args for sphere: radius and centre.\r\n");
	}
	return false;
}
//
//	BuildBox reads two float triplets in and uses them to construct
//	a wire-frame box.
//
bool CGLAList::BuildBox(int nArg) {
	if (nArg >= 6) {
		//
		//	Bottom cap.
		//
		glBegin(GL_LINE_LOOP);
		glVertex3f(gArguments[0], gArguments[1], gArguments[2]);
		glVertex3f(gArguments[0], gArguments[4], gArguments[2]);
		glVertex3f(gArguments[3], gArguments[4], gArguments[2]);
		glVertex3f(gArguments[3], gArguments[1], gArguments[2]);
		glEnd();
		//
		//	Top cap.
		//
		glBegin(GL_LINE_LOOP);
		glVertex3f(gArguments[0], gArguments[1], gArguments[5]);
		glVertex3f(gArguments[0], gArguments[4], gArguments[5]);
		glVertex3f(gArguments[3], gArguments[4], gArguments[5]);
		glVertex3f(gArguments[3], gArguments[1], gArguments[5]);
		glEnd();
		//
		//	And four pillars.
		//
		glBegin(GL_LINES);
		glVertex3f(gArguments[0], gArguments[1], gArguments[2]);
		glVertex3f(gArguments[0], gArguments[1], gArguments[5]);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(gArguments[0], gArguments[4], gArguments[2]);
		glVertex3f(gArguments[0], gArguments[4], gArguments[5]);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(gArguments[3], gArguments[4], gArguments[2]);
		glVertex3f(gArguments[3], gArguments[4], gArguments[5]);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(gArguments[3], gArguments[1], gArguments[2]);
		glVertex3f(gArguments[3], gArguments[1], gArguments[5]);
		glEnd();
		mBounds->AddPoint3fv(&gArguments[0]);
		mBounds->AddPoint3fv(&gArguments[3]);
		return true;
	}else {
wxLogMessage("Need at least 6 args for box (2 points).\r\n");
	}
	return false;
}


//
//	BuildTriangle reads three float triplets in and uses them to construct
//	an OpenGL triangle.
//
bool CGLAList::BuildTriangle(int nArg) {
	if (nArg >= 9) {
		glBegin(GL_TRIANGLES);
		glVertex3fv(&gArguments[0]);
		glVertex3fv(&gArguments[3]);
		glVertex3fv(&gArguments[6]);
		glEnd();
		mBounds->AddPoint3fv(&gArguments[0]);
		mBounds->AddPoint3fv(&gArguments[3]);
		mBounds->AddPoint3fv(&gArguments[6]);
		return true;
	}else {
wxLogMessage("Need at least 9 args for triangle (3 points).\r\n");
	}
	return false;
}

//
//  BuildCylinder takes either one or two float triplets and constructs
//  a cylinder.
//
bool CGLAList::BuildCylinder(int nArg) {
  if (nArg >= 3) {
    //
    //  This one is not too bad because it builds its cylinder
    //  with the axis along z.
    //
    float bottom = gArguments[0];
    float top = gArguments[1];
    float radius = gArguments[2];
    GLUquadricObj *glq = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0, 0, bottom);
//    if (mFlags & kGFWire) { // Wire Frame
      gluQuadricDrawStyle(glq, GLU_LINE);
//    }
    gluCylinder(glq, radius, radius, top - bottom, 20, 20);
    glPopMatrix();
  } else if (nArg >= 6) {
    //
    //  This is more complex because it has to re-orient the
    //  axis. Actually it re-orients the coordinates and builds
    //  the cylinder along the new z.
    //
	}else {
    wxLogMessage("Need at least 3 args for cylinder.\r\n");
	}
	return false;
}

//
//  BuildCap constructs a flat circular sheet possibly with a hole in it.
//
bool CGLAList::BuildCap(int nArg)
{
  return false;
}

