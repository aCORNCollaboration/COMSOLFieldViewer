/*
 *	CTextScanner.h
 *
 *	A TextScanner is an object that translates a text file into a stream of
 *	lexemes. This scanner makes use of a CharacterClass table to help it
 *	identify primitve symbols in the grammar.
 *	The scanner is a simple finite state machine with one element of pushback.
 *
 *	BCollett 9/96 see CTextScanner.cp Based on CSTextScanner.
 *	BCollett 9/99
 *	This version has been adapted to work with the CArchive mechanism of
 *	the MFC library. A TextScanner is created based on an archive and
 *	calls the Archive's ReadLine routine in the NextChar routine. So we
 *	read the text in a line at a time and then pass it out to the user
 *	a character at a time, taking care of the transition from one line
 *	to another.
 *
 *  BCollett 2/20/14 Remove CArchive dependence. Return to plain old
 *  stream input.
 */
#ifndef _H_CTextScanner_H
#define _H_CTextScanner_H
#include <stdio.h>
#include "Lexemes.h"
#include "CSymbolTable.h"
//
//	First we have the representation of a symbol. This is the thing that
//	we pass back to the interpreter.
//
struct Lexeme {
	short lex;				// The symbol code
	union {
		double rValue;          // value if we saw a number
		const char *strValue;		// value if a string
	} val;
};
#define sVal val.strValue
#define nVal val.rValue

//
//
//	This is a parameter that sets the number characters in the line
//	buffer.
//
#define CTLineLength 32768

class CTextScanner {
protected:
	//
	//	We keep a pointer to our archive and the text of one line
	//	from the file.
	//	In addition we keep track of which line in the text we are on.
	//	Note that we identify lines by counting '\r's.
	//
	FILE* mIfp;
	char *mText;
	char  *mNextChar;
	unsigned long mLineNumber;
	//
	//	We build the symbols in our internal Lexeme and actually pass pointers to
	//	this one real symbol around. This makes pushback very easy to support since
	//	all we then need is a flag.
	//
	Lexeme currLex;
	char TokenBuff[256];		// temporary storage for names and strings
	short pushedBack;
	//
	//	Base is used in the parsing of digits. It is the number of
	//	unique different digits in the number base.
	//
	int mBase;
	//
	//	We also have some storage to help with the translation process. We keep
	//	both the character class table (that classifies characters accoring to
	//	their possible uses) and pointers to the symbol tables here.
	//
	static CClass CharClass[256];
  //
  //  And lastly the line buffer itself.
  //
public:
	//
	//	Constructor is initialised with the archive that we scan.
	//
	CTextScanner(FILE* fp);
	~CTextScanner();
	//
	//	The scanner is the externally visible thing that does the real work.
	//	Every time the parser wants a symbol it calls this.
	//
	Lexeme *NextLex();
	//
	//	The next operation the parser needs is to push a symbol back when
	//	it has read more than it needs. The scanner supports 1 symbol of
	//	push-back.
	//
	void ReturnSym();
	//
	//	We may also need to tell the parser what line we are on.
	//
	unsigned long LineNumber() { return mLineNumber; };
  //
  //  and what the line was.
  //
  const char* GetLine() { return mText; };
	//
	//	This can be called after an error to clean up the rest of a line.
	//
	void CleanLine();
	//
	//	When a syntax error occurs this routine is called to show the point at which
	//	the error was detected. It builds a cell containing the offending line and a
	//	line with a marker at the pont at which the error was detected.
	/*
   CCell *ShowCurrentPosition(StyleRec &);
   */
protected:
	//
	//	Then we have internal operations that the scanner uses such as the
	//	routine to peel off one character from the STextCell and the special
	//	one used to read characters in strings.
	//
	char NextChar();
	char Peek();
	void BackUp();
	char ReadStrch();
	void ResetVars();
	bool IsDigit(char ch);
	int DigitValue(char ch);
};

#endif // _H_CTextScanner_H
