/*
*	CSymbol.h
*
*	Defines a symbol table entry. This is an object which can be stored in a double
*	linked list and which has a name (up to 30 characters long) and a value. 
*	All entries are the same size so that we can make this a fixed block class for 
*	easy free storage management. All storage management is taken care of by the
*	global allocator that we access in our new/delete..
*
*	BCollett 9/96 adapted from version in NewDal. This one uses fixed length character
*	arrays for its name storage and has simplified value storage.
*/
#ifndef _H_CSymbol_H
#define _H_CSymbol_H
#pragma once
#include <stddef.h>


class CSymbol {
public:
	CSymbol *mNextSym;	// Forward link in list of symbols
	CSymbol *mPrevSym;	// Backward link
	char mName[16];		// the external rep of the symbol
	short mNChar;		// number of chars in name
	short mSym;			// symbol number
//
//	Methods start with our special new and delete operators used to
//	manage our free store.
//
public:
//
//	If we are building the fancy block allocated version then we need
//	to override the standard new and delete operators.
//
#ifdef UseBlockAlloc
	static void* operator new (size_t);
	static void operator delete (void *);
#endif
//
//	Constructor handles task of assigning name and SType (which EVERY symbol
//	must have).
//
	CSymbol(const char *newName, short newSym);
//
//	We define equality operators for two symbols and for a symbol and
//	a string.
//
	bool operator==(CSymbol &);
	bool operator==(char *);
};

#endif // _H_CSymbol_H
