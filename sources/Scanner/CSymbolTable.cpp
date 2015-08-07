/*
*	CSymbolTable.cp
*
*	This defines the symbol table class. The interface is a generic
*	and should hide the implementation from the world.
*	This version is implemented as a standard double-linked bucket-chained hash table.
*
*	BCollett 1/95
*/
#include <string.h>
#include "CSymbolTable.h"

//
//	Constructor has to initialise an empty hash table. Destructor 
//	has to clean out contents of a symbol table.
CSymbolTable::CSymbolTable()
{
	for (long i = 0; i < kHashSize; ++i) {
		mHashTable[i] = NULL;
	}
	mNSymbols = 0;
}

CSymbolTable::~CSymbolTable()
{
	for (long i = 0; i < kHashSize; ++i) {
		CSymbol *cSym = mHashTable[i];
		while (cSym != NULL) {
			CSymbol *oSym = cSym;
			cSym = oSym->mNextSym;
			delete oSym;
		}
	}
}
//
//	InsertName first checks to see if the name is already found
//	in the table. 
//	If the name is new then a new symbol is created and inserted
//	into the table. It returns the new symbol.
//	If the symbol is already in the table then InsertName checks
//	to see that the sym's match and if so returns the old symbol.
//	If this test fails then NULL is returned. 
//
CSymbol *CSymbolTable::InsertName(const char *name, short sym)
{
	CSymbol *newSym = NULL;
	if ((newSym = LookUpWord(name)) == NULL) {
		newSym = InsertSymbol(new CSymbol(name, sym));
	} else {
		if (newSym->mSym != sym) {
			newSym = NULL;
		}
	}
	return newSym;
}
//
//	InsertSymbol takes an existing symbol and puts it into the table. 
//	After this the table owns the symbol.
//	Note that this does not check for an existing symbol. It trusts
//	InsertName to keep the symbols unique.
//
CSymbol *CSymbolTable::InsertSymbol(CSymbol *theSym)
{
	long hashVal = Hash(theSym->mName);
	CSymbol *link = mHashTable[hashVal];
	mHashTable[hashVal] = theSym;
	if (link != NULL) {
		theSym->mNextSym = link;
		link->mPrevSym = theSym;
	}
	++mNSymbols;
	return theSym;
}
//
//	LookupName tries to find an entry in the table for a given name. It returns
//	the symbol if it finds it else it returns nil.
//
CSymbol *CSymbolTable::LookUpWord(const char *theName)
{
	long hashVal = Hash(theName);
	CSymbol *cSym = mHashTable[hashVal];
	while ((cSym != NULL) && strcmp(cSym->mName, theName) ) {
		cSym = cSym->mNextSym;
	}
	return cSym;
}
//
//	Delete a symbol from the table and dispose of it.
//
void CSymbolTable::DeleteSymbol(CSymbol *oldSym)
{
	if (oldSym->mPrevSym == NULL) {		// First in chain is different from all rest
		long hashVal = Hash(oldSym->mName);
//		IASSERT(hashTable[hashVal] == oldSym, DeleteBadSymbol);
		mHashTable[hashVal] = oldSym->mNextSym;
	}
	else {
		(oldSym->mPrevSym)->mNextSym = oldSym->mNextSym;
	}
	delete oldSym;
	--mNSymbols;
}
//
//	We also have a private method that implements the hash function.
//
long CSymbolTable::Hash(const char *name)
{
	short i,hashval;
	const char *tokp = name;
	short length = strlen(name);
	hashval = 3 * length;
	for (i = 0; i < length; ++i) {
		hashval = (hashval >> 1) ^ *tokp++;
	}
	return (hashval % kHashSize);
}
