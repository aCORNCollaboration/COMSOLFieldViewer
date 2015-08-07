/*
*	GLAList.h
*
*	A CGLAList is a special kind of DisplayList that gets its
*	contents from a text file with the extension .gla.
*	It can parse a file with the syntax
*
*	color <r> <g> <b>
*	line <xs> <ys> <zs> <xe> <ye> <ze>
*	polyline <n> <x1> <y1> <z1> .... <yn> <zn>
*
*	Each line has the form of a keyword followed by a list
*	of real numbers. The meanings of those numbers are
*	assigned by different keywords.
*
*	Where the values in angle brackets are floating point numbers.
*
*	BCollett 9/99
*	This gets text from a TextScanner that it expects to be ready 
*	for use.
*	BCollett 1/26/04 Redesign with argument list and class symbol table.
*/
#ifndef _H_GLAList_H
#define _H_GLAList_H
#pragma once

//#include "GLBAse/OpenGLApp.h"
#include "../Scanner/CSymbolTable.h"
#include "../Scanner/CTextScanner.h"
#include "GeometricObjects.h"

//
//	Enum for the different kinds of OpenGL command that can be
//	found in a .gla file.
//
typedef enum {
	kGLColor = 0,
	kGLLine,
	kGLPoint,
	kGLPolyLine,
	kGLSphere,
	kGLBox,
	kGLTriangle,
	kGLTriStrip,
	kGLTranslate,
  kGLCylinder,
  kGLCap,
  kGLEnd,     // end of geometry. Allows sharing geom file with other.
	kGLError 
} GLCommand;

class CGLAList : public CDisplayList {
protected:
	//
	//	Class variables.
	//	Class owns a symbol table to translate the keywords and
	//	a large array in which to store the arguments.
	//	These should be filled in by calling InitClass(nArg)
	//	before creating any class members.
	//
	static CSymbolTable* gSymTab;
	static GLfloat* gArguments;
	static int gMaxNArgs;
	//
	//	Instance variables.
	//
	//COpenGLApp* mApp;		// Owning app, so we can print!
	CTextScanner* mScan;	//  our scanner (when valid)
	//
public:
	//
	//	Constructor and destructor.
	//	Constructor can optionally take a scanner.
	//
	CGLAList(CTextScanner* newScan = NULL);
	virtual ~CGLAList();
	//
	//	Override the abstract Create function to make this
	//	a real class.
	//
	virtual bool Create();
	//
	//	Have a pair of functions to control our onership of the
	//	scanner.
	//
	void InstallScanner(CTextScanner* newScan);
	void ReleaseScanner();
	//
	//	Class routines to init and destroy the class.
	//
	static bool InitClass(int);
	static void ReleaseClass();
protected:
	//
	//	Helper functions used internally.
	//
	//	GetArgList scans the rest of the current line for numbers which
	//	it puts into the argument array.
	//
	int GetArgList();
	bool BuildPoint(int nArg);
	//
	//	BuildLine reads two float triplets in and uses them to construct
	//	an OpenGL line.
	//
	bool BuildLine(int nArg);
	//
	//	BuildPolyLine reads a number that tells us the number of points
	//	in the line and then reads that many triplets into the vertices
	//	of an OpenGL polyline.
	//
	bool BuildPolyLine(int nArg);
	//
	//	GetSphere reads in a radius and centre position. It creates
	//	a GLUSphere and moves it into position.
	//
	bool BuildSphere(int nArg);
	//
	//	BuildBox reads two float triplets in and uses them to construct
	//	a wire-frame box.
	//
	bool BuildBox(int nArg);
	//
	//	BuildTriangle reads three float triplets in and uses them to construct
	//	a triangular facet.
	//
	bool BuildTriangle(int nArg);
  //
  //  BuildCylinder takes either one or two float triplets and constructs
  //  a cylinder.
  //
  bool BuildCylinder(int nArg);
  //
  //  BuildCap constructs a flat circular sheet possibly with a hole in it.
  //
  bool BuildCap(int nArg);

};

#endif // _H_GLAList_H
