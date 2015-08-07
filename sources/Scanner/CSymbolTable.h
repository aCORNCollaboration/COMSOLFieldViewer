/*
*	CSymbolTable.h
*
*	This defines a symbol table class. The interface is a generic
*	and should hide the implementation from the world.
*	This version is implemented as a standard bucket-chained hash table.
*
*	BCollett 9/96 see CSymbolTable.cp
*/
#ifndef _H_CSymbolTable_H
#define _H_CSymbolTable_H
#pragma once

#include "CSymbol.h"

const int kHashSize = 256;

class CSymbolTable {
protected:
	//
	//	We have private storage for the hash table.
	//
	CSymbol *mHashTable[kHashSize];
	//
	//	keep track of total number of symbols in table
	//
	int mNSymbols;
public:
	//
	//	Constructor has to initialise an empty hash table. Destructor has to clean out
	//	contents of a symbol table.
	CSymbolTable();
	~CSymbolTable();
	//
	//	We have to be able to insert an existing symbol, create
	//	a new entry for a name, look up a name, or remove a symbol from the
	//	table.
	//
	CSymbol *InsertName(const char *, short);
	CSymbol *InsertSymbol(CSymbol *);
	CSymbol *LookUpWord(const char *);
	void DeleteSymbol(CSymbol *);
	int GetNumberSymbols() { return mNSymbols; };
protected:
	//
	//	We also have a private method that implements the hash function.
	//
	long Hash(const char *);
};

#endif // _H_CSymbolTable_H

