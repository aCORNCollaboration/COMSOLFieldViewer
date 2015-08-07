/*
*	Lexemes.h
*
*	Defines the lexemes used by the .gla reader to describe the 
*	contents of an OpenGL file.
*	These definitions are used by the TextScanner.
*
*	BCollett 8/24/99
*/
#ifndef _H_Lexemes_H
#define _H_Lexemes_H
#pragma once
/*
*	Error info and error types.
*/

/*
*	The symbols (or Lexemes) are built from characters and the characters are sorted into
*	classes by the CharClass array. Here are the lexeme codes.
*/
typedef unsigned char CClass;

extern CClass CharClass[];

enum {							// types of different symbol, tied to CharClass.
	LNull = 0,					//  0 unused null sumbol
	LWord,						//  1 an atomic symbol
	LInteger,					//  2 a number with no exponent or fractional part
	LReal,						//  3 a number that is not an int
	LString,					//  4 a quoted string
	LNewLine,					//  5 end of line
	LEof,						//  6 end of file
	LPunctuation = 20,			//  7 start of punctuation.
	LQuote,						//  8 " to begin a string 
	LEscape,					//  9 \ or ¶ used to escape an end-of-line or other odd character 
	LSpace,						// 10
	LDollar,					// 11 $ marks start of hex number
	LPeriod,					// 12 ., special code needed because it is really part of a number 
	LSQuote,					// 13 "'" delimits one kind of comment
	LMinus,						// 14 '-'
	LPrimStart=30,
	LBang,						// 31 '!' as in '!='
	LDivide,					// 32 '/'
	LLT,						// 33 '<'
	LGT,						// 34 '>'
	LColon,						// 35 ':'
	LEquals,					// 36 '='
	LPlus,						// 37 '+' 
	LTimes,						// 38 '*'
	LHash,						// 39 '#'
	LExp,						// 40 '^'
	LLBra,						// 41 '(' 
	LRBra,						// 42 ')' 
	LLSBra,						// 43 '[' 
	LRSBra,						// 44 ']' 
	LLCBra,						// 45 '{' 
	LRCBra,						// 46 '}' 
	LAnd,						// 47 '&'
	LOr,						// 48 '|'
	LComma,						// 49 ','
	LSemiC,						// 50 ';'
	LQuery,						// 51 '?', requests printing of a value
	LError						// 52
};
#define LTerm LNewLine
/*
*	The class codes are constructed in a number of different ways depending on how the
*	code is used by the lexical analyser.  Some codes, those for primitive symbols, consists
*	of the symbol values plus some distinguishing offset. Some are simple unique, arbitrary
*	numbers. Finally, some are based on bit position. In particular, any code in the first
*	group which is >= 128 a character that can be used in a word since the high order bit
*	is the LAlphaBit. Chars that use this are set as eg. CAlpha but respond as LAlpha.
*/
#define LAlphaBit	0x80	// Set for anything that can be in a word 
#define LErr		0		// Some kind of error 
#define	LAlpha		1		// such as a letter 
#define CAlpha		0x81	// What we put in here
#define LWdBeg		2		// $ can begin a word but may not appear in one 
#define LDigit		3		// or a number 
#define CDigit		0x83	// What we put in here
#define LEndFile	29		// the mark of an end of file 
/*
*	Error codes that Lex can return to the parser.
*/
#define LNoErr		0
#define LMissingExponent	-100
#define	LExponentRange		-101
#define LStringSize			-102
#define	LBadBackslash		-103
#define LPlusError			-104
#define LBadChar			-105

#endif // _H_Lexemes_H
