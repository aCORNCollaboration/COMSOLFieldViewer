/*
*	CDisplayList.h
*	This class encapsulates the idea of an OpenGL display
*	list and allows a program to manipulate display lists
*	as objects without having to keep track of the small
*	integers that OpenGL uses to track the display lists.
*	An OpenGL display is stored in OpenGL's own memory space
*	and referenced by a display list number, a small integer
*	greater than 0. This makes it difficult to maintain and
*	manipulate multiple display lists so this class encapsulates
*	the idea.
*	Each instance of CDisplayList maintains a reference to the
*	underlying OpenGL object through its mListHandle member.
*	When the CDisplayList is created the instance obtains a
*	new, unique, display list number from OpenGL and then
*	stores that for future reference.
*	This is an abstract class since it does not provide any
*	way to fill in the contents of the display list. You must
*	subclass CDisplayList and implement the InitList member
*	function.
*
*	BCollett 8/99
*/
#include "GeometricObjects.h"

#ifdef UseOpenGL
//
//	Constructor sets up the link to OpenGL.
//	Its argument is a mode flag that determines
//	whether the display list is executed as it
//	is built or merely compiled.
//
CDisplayList::CDisplayList(DisplayListMode theMode) {
/*
	GLuint newList = glGenLists(1);	// ask for 1 new ID
	if (newList == 0) {		// Something really wrong
		mListHandle = -1;	// Mark. Usually means that OpenGL is not there yet
	} else {
		mListHandle = newList;
	}
*/
  mListHandle = -1;	// Mark. Usually means that OpenGL is not there yet
	mMode = theMode;
	mBounds = new Frame3D();
}
//
//	Base destructor disposes of mBounds.
//
CDisplayList::~CDisplayList() {
	delete mBounds;
}
//
//	Validate can be called to make sure that a display list is valid,
//	that is that it has a valid list handle. If not it will try to get
//	and will only return false if it fails.
//
bool CDisplayList::Validate() {
	if (mListHandle <= 0) {
		GLuint newList = glGenLists(1);	// ask for 1 new ID
		if (newList == 0) {		// Something really wrong
//			mListHandle = 10000;
			return false;
		} else {
			mListHandle = newList;
		}
	}
	return true;
}
//
//	Internal helpers for the use of subclasses.
//	Start and stop building list.
//
void CDisplayList::Start() {
  Validate();
	if (mListHandle > 0) {
		glNewList(mListHandle, mMode);
	} else {
		--mListHandle;
	}
}
void CDisplayList::End() {
	if (mListHandle > 0) {
		glEndList();
	} else {
		--mListHandle;
	}
}
#endif
