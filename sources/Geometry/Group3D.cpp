/*
 *  Group3D.cpp
 *  Simulator1_13_09
 *
 *	We represent a complete set of objects as a Group3D. Any
 *	GeometryObject can be put into a group, including another
 *	Group. This allows a complex, tree stuctured, representation
 *	of heirarchical objects.
 *
 *  Created by Brian Collett on 7/8/10.
 *  Copyright 2010 Brian Collett. All rights reserved.
 *
 */
#include <math.h>
#include "GeometricObjects.h"
//
//	Class vars.
//
int Group3D::gNInitialSlots = 20;
int Group3D::gNAdditionalSlots = 10;
//
//	Constructor/Destructor
//
Group3D::Group3D() :  
GeometryObject( kGeomGroup) {
	mElems = new GeometryObject*[gNInitialSlots];
	mNumSlots = gNInitialSlots;
	mNumElem = 0;
	Update();
}

Group3D::~Group3D() {
	//
	//	Recursively dispose of contents.
	//
	for (int i = 0; i < mNumElem; ++i) {
		mElems[i]->Delete();
	}
	delete[] mElems;
}

//
//	Add an object to a group.
//
Group3D& Group3D::Add(GeometryObject* o) {
	//
	//	Make sure there is room else add more slots. That will
	//	mean doing a blind copy of the old elems.
	//
	if (mNumElem >= mNumSlots - 1) {
		GeometryObject** newElems = 
		new GeometryObject*[mNumSlots + gNAdditionalSlots];
		for (int i = 0; i < mNumElem; ++i) {
			newElems[i] = mElems[i];
		}
		delete[] mElems;
		mElems = newElems;
		mNumSlots += gNAdditionalSlots;
	}
	//
	//	Put object into next free slot.
	//
	mElems[mNumElem++] = o->GetPtr();
	Update();
	return *this;
}
//
//	WriteToFile is simply a run through the array with
//	each element printing itself.
//
bool Group3D::WriteToFile(FILE* ofp) {
	int nErr = 0;
	for (int i = 0; i < mNumElem; ++i) {
		if (!mElems[i]->WriteToFile(ofp)) ++nErr;
	}
	return nErr == 0;
}

#ifdef UseOpenGL
//
//	Recursively work through the list.
//
void Group3D::Update() {
	if (mNumElem > 0) {
		mList.Start();
		RunMaterial();
    
		for (int i = 0; i < mNumElem; ++i) {
			mElems[i]->Draw();
		}
		
		mList.End();
	}
}
#endif
//
//  AddToBounds sends in a bounding box and expects us to
//  alter it to include ourselves. 
//  Override to add ourselves to the box.
//
Frame3D* Group3D::AddToBounds(Frame3D* bBox)
{
	if (mNumElem > 0) {
		for (int i = 0; i < mNumElem; ++i) {
			mElems[i]->AddToBounds(bBox);
		}
	}
  return bBox;
}



