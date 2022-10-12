/*
 *  GeometricObject.cpp
 *  Simulator1_13_09
 *
 *	A Geometric object posesses both mathematical geometry and an optional
 *	visible representation. GoemetricObjects use reference counting to reduce
 *	copying and provide a debugging mode that tags each object in a
 *	debugger visible way.
 *	The presence of the visible representation is governed by the preprocessor
 *	symbol UseOpenGL. If this is not declared then there is practically no
 *	overhead and you have a lightweight system of representing 3D geometries.
 *	In addition, if the preprocessor symbol DebugGeom is defined then the
 *	objects are tagged. OpenGL and tagging are independent.
 *
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <math.h>
#include "GeometricObjects.h"


#ifdef DebugGeom
const char* gGeomIDs[] = {
	"NULL",
	"COLR",
	"PONT",
	"VECT",
	"VERT",
	"ASET",
	"LINE",
	"PLIN",
	"BOX ",
	"GBOX",
	"ELPS",
	"TUBE",
	"GRUP",
	"GER1",
	"GER0",
	"GER1",
	"GER2",
	"GER3",
	"GER4",
	"GER5",
	"GER6",
	"GER7",
	"GER8",
	"GER9"
};
#endif

#ifdef UseOpenGL
const char* GLErrors[6] = {"inv enum", "inv value", "inv opn",
	"stack over", "stack under", "memory"};
#endif

/*
 *	Start with GeometryObject.
 */
//
//	Construct/Destruct
//
GeometryObject::GeometryObject(int code)
#ifdef UseOpenGL
: mList() 
#endif
{
#ifdef DebugGeom
	mID[0] = gGeomIDs[code][0];
	mID[1] = gGeomIDs[code][1];
	mID[2] = gGeomIDs[code][2];
	mID[3] = gGeomIDs[code][3];
#endif
	mRefCount = 1;
	mGeomCode = code;
	
#ifdef UseOpenGL
	//
	//	Then the material parameters.
	//
	mColor.Set(1.0f, 1.0f, 1.0f);
	mMaterial[0] = 1.0f;		// Ambient
	mMaterial[1] = 1.0f;		// Diffuse
	mMaterial[2] = 0.0f;		// Specular
	mMaterial[3] = 0.3f;		// Shininess
	mMaterial[4] = 1.0f;		// Transparency, 1 = opaque
	mFlags = 0;
#endif
}

GeometryObject::~GeometryObject() {
	mID[0] = -1;
}

int GeometryObject::Delete() {
	int ref = (mRefCount -= 1);
	if (mRefCount <= 0) {
		delete this;	// Invokes destructor chain automagically
	}
	return ref;
}

GeometryObject* GeometryObject::GetPtr() {
	++mRefCount;
	return this;
}

#ifdef UseOpenGL
//
//	Routines to manipulate the appearance of the object.
//
void GeometryObject::SetColor(RGBColour& c) {
	mColor = c;
	Update();
}
void GeometryObject::SetColor(GLfloat r, GLfloat g, GLfloat b) {
	mColor.Set(r, g, b);
	Update();
}
void GeometryObject::SetMaterials(GLfloat amb, GLfloat diff, GLfloat spec, 
																	GLfloat shine, GLfloat trans) {
	mMaterial[0] = amb;
	mMaterial[1] = diff;
	mMaterial[2] = spec;
	mMaterial[3] = shine;
	mMaterial[4] = trans;
	Update();
}
void GeometryObject::SetMaterials(const GLfloat* mat, int nVal) {
	if (nVal > 4) {
		nVal = 4;
	}
	for (int i = 0; i < nVal; ++i) {
		mMaterial[i] = mat[i];
	}
	Update();
}
long GeometryObject::AddFlags(long f) {
	mFlags |= f;
	Update();
	return mFlags;
}
/*
 *	RunMaterial is a helper for the Update routines. It outputs
 *	the material properties in OpenGL form.
 */
#define mRed _m._c.red
#define mGreen _m._c.green
#define mBlue _m._c.blue
#define mComponents _m._comps
void GeometryObject::RunMaterial() {
	GLfloat a[4];
	a[0] = mColor.mRed * mMaterial[0];
	a[1] = mColor.mGreen * mMaterial[0];
	a[2] = mColor.mBlue * mMaterial[0];
	a[3] = mMaterial[3];
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a);
	a[0] = mColor.mRed * mMaterial[1];
	a[1] = mColor.mGreen * mMaterial[1];
	a[2] = mColor.mBlue * mMaterial[1];
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, a);
	a[0] = mColor.mRed * mMaterial[2];
	a[1] = mColor.mGreen * mMaterial[2];
	a[2] = mColor.mBlue * mMaterial[2];
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, a);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mMaterial[4]);
}
#endif



