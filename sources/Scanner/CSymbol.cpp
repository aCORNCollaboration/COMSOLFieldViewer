/*
 *	CSymbol.cp
 *
 *	Defines a symbol table entry. This is an object which can be stored in a
 *  double linked list and which has a name (up to 16 characters long),
 *  and a value. All entries are the same size so that we can make
 *	this a fixed block class for easy free storage management. 
 *  All storage management is taken care of by the global allocator 
 *  that we access in our new/delete..
 *
 *	BCollett 1/95
 */
#include <string.h>
//
//	Uncomment the following to use the fancy block allocated version
//	of this class. If you do this then you must include the block
//	allocator in the project as well.
//
//#define UseBlockAlloc
//
#include "CSymbol.h"
//
//	Next few lines are only kept if we are using the block allocator.
//
#ifdef UseBlockAlloc
#include "CFixedBlockAllocator.h"
/*
 *	The new method tries very hard to produce a new block.
 *	If there are blocks on the free list then it will reuse those first.
 *	If not then it tries to return the next block from the newStore.
 *	If even that does not work then it tries to allocate a new free store.
 *	If that fails then there is nothing we can do so we let the exception
 *	mechanism handle that case.
 */
void *CSymbol::operator new(size_t blockSize)
{
  //	IASSERT(blockSize == sizeof(CSymbol),BlockSizeMismatch);
	return (CSymbol *) GAllocator.alloc(blockSize);;
}
/*
 *	Delete just links the returned block onto the front of the free list.
 */
void CSymbol::operator delete(void *freeBlock)
{
	GAllocator.free(freeBlock,sizeof(CSymbol));
}
#endif
//
//	Constructor handles task of assigning name and symbol.
//
CSymbol::CSymbol(const char *theName, short newSym)
{
	mNextSym = mPrevSym = NULL;
	mNChar = strlen(theName);
	strncpy(mName, theName, 15);
	mSym = newSym;
}
//
//	We define equality operators for two symbols and for a symbol and
//	a string.
//
bool CSymbol::operator==(CSymbol& theSym) {
	int res = strncmp(mName, theSym.mName, 15);
	return (res == 0);
}
bool CSymbol::operator==(char* theStr) {
	int res = strncmp(mName, theStr, 15);
	return (res == 0);
}


