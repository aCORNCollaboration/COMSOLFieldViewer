/*
 *	CTextScanner.cp
 *
 *	A TextScanner parses a C string of text (possibly multiline) into a set of lexemes.
 *	The primitive recognition is controlled by a CharClass array.
 *	This version has been adapted to work with the CArchive mechanism of
 *	the MFC library. A TextScanner is created based on an archive and
 *	calls the Archive's ReadLine routine in the NextChar routine. So we
 *	read the text in a line at a time and then pass it out to the user
 *	a character at a time, taking care of the transition from one line
 *	to another.
 *
 *	BCollett 2/95
 *  Slightly reworked BCollett 2/14 to use FILE I/O.
 */
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include "CTextScanner.h"

//
//	The scanner can potentially work with quite complex types
//	of text. You can control which of the more advanced types
//	of lexeme are supported by altering these defines.
//
//	Uncomment the next line if you want strings.
//
//#define WantStrings 1
//
//	Uncomment the next line to include reals.
//
#define WantReals 1
//
//	Uncomment the next line to include negative numbers.
//
#define WantNegatives 1
//
//	Uncomment the next line to include Hexadecimal numbers.
//
//#define WantHex 1
//
//	Uncomment the next line to include single char primitives
//
#define WantSPrims
//
//	Uncomment the next line to include multi-char primitives
//
//#define WantMPrims
//
//	Constructor is initialised with the text we scan through.
//	We keep a pointer to the text but we do not own it.
//
CTextScanner::CTextScanner(FILE* fp)
{
  mIfp = fp;
	mText = new char[CTLineLength];
	mNextChar = mText;
	mText[0] = 0;
	mLineNumber = 0;
	currLex.lex = LError;
	pushedBack = false;
}
//
//	The destructor has to dispose of the text buffer.
//
CTextScanner::~CTextScanner()
{
	delete[] mText;
}
//
//	The scanner is the externally visible thing that does the real work.
//	Every time the parser wants a symbol it calls this.
//
Lexeme *CTextScanner::NextLex()
{
	char ch;                  // The current character
	CClass cc;                // The class of the current character
	char *tokp = TokenBuff;		// build tokens here
	unsigned char nChar;			// counts number of chars in a string/word
	double fVal = 0.0,				// denominator fractional part of a number
  fScale = 1.0,             // numerator of fraction part of number
  dVal = 0.0,               // Complete value of a number
  exponent = 0.0,           // Exponent of a number
  eSign = 1.0;              // Sign of exponent
	short neg = false,				// tells us to change sign of a number
  isInteger = true;         // tells us if number is an integer
  /*
   *	First see if we can just return the old one else make sure 
   *  PushedBack is FALSE.
   */
	if (pushedBack) {
		pushedBack = false;			// Must remember to clear the flag whatever happens!
		return &currLex;
  }
	pushedBack = false;
  /*
   *	Initialise the global error number to no error and start a gigantic 
   *  loop that practically never gets executed. It is there to catch things 
   *  like comments and escaped end-of-line's that are totally ignored. 
   *  All real sybols exit the loop by direct return.
   */
	currLex.lex = LError;			// Default response for if nothing better happens
	while (true) {
		mBase = 10;						// default numbers to decimal
    /*
     *	First skip white space.
     */
		while ((cc = CharClass[(ch = NextChar())]) == LSpace) {
		}
    /*
     *	Now work your way through the non-space chars. Test for primitives 
     *  because they are treated separately. Anything else goes into the 
     *  big case.
     *	Note, we mask off the top bit that marks word letters before 
     *  checking for prims.
     */
		if ((cc & 0x3F) < LPrimStart) {
			switch (cc & 0x3F) {
          /*
           *	First catch the ending things.
           */
				case LEndFile:currLex.lex = LEof;
          return &currLex;
          
				case LNewLine:currLex.lex = LNewLine;
          return &currLex;
          /*
           *	Then try for a word. Words must begin with a letter and must
           *	contain letters, digits, or underscores.
           *	Words are first built into the longest possible token and 
           *  then looked up in the symbol table.
           */
				case LAlpha:
					*tokp++ = toupper(ch);		// Put first char into word
					nChar = 1;
					while (CharClass[ch = NextChar()] & LAlphaBit) {
						*tokp++ = toupper(ch);
						++nChar;
          }
					*tokp = 0;
					BackUp();						// Return the unused char
					currLex.lex = LWord;
					currLex.sVal = TokenBuff;
					return &currLex;
          /*
           *	Before numbers and .'s need to check up on a '-' 
           *  because it could begin a negative number!
           */
				case LMinus:
#ifdef WantNegatives
					ch = Peek();
					if (ch >= '0' && ch <= '9') {
						ch = NextChar();
						neg = true;
						goto StartNum;
          }
					else if (ch == '.') {
						ch = NextChar();      // that read the '.'
						ch = Peek();          // and that looked ahead still further
						if (ch >= '0' && ch <= '9') {
							neg = true;
							ch = NextChar();    // ch is now the first digit
							goto HavePeriod;		// and we handle the rest of the number
						}
						else {
							ch = '.';
							BackUp();
							currLex.lex = LMinus;
						}
					}
					else
#endif
						currLex.lex = LMinus;
					return &currLex;
          /*
           *	Before I do numbers let me do '.'s. This nasty code 
           *  allows the user to type a number like .35 with no leading
           *  digits.
           */
#ifdef WantReals
				case LPeriod:ch = Peek();		// Rare enough not to NextChar
          if (ch >= '0' && ch <= '9') {
            ch = NextChar();
            goto HavePeriod;
          }
          else {
            currLex.lex = LError;
            currLex.sVal = "Found an unexpected period.";
          }
          break;
#endif
          /*
           *	Hex numbers starting with a '$' can optionally be recognised.
           */
#ifdef WantHex
				case LDollar:
					if (CharClass[ch = NextChar()] != LDollar) {
						currLex.lex = LDollar;
						return &currLex;
					}
					//
					//	Else falls through to number
					//
					mBase = 16;
#endif
          /*
           *	Numbers can only begin with digits but have a fairly complex 
           *  structure once they get started. I convert them into values 
           *  as I go. Note that tests in inner loops are all with CDigit, 
           *  the code WITH the 0x*0 bit set since it has not been stripped.
           */
				case LDigit:
        StartNum:
					dVal = DigitValue(ch);		// Start accumulating the number
					while (IsDigit(ch = NextChar())) {
						dVal *= mBase;
						dVal += DigitValue(ch);
					}
#ifdef WantReals
					if (ch == '.' && Peek() != '.') {		// Do fractional part
          HavePeriod:
						isInteger = false;
						while (IsDigit(ch = NextChar())) {
							fVal *= mBase;
							fScale *= mBase;
							fVal += ch - '0';
						}
						dVal += fVal / fScale;			// Complete fraction is added in
					}
					if (ch == 'e' || ch == 'E') {					// It has an exponent
						isInteger = false;
						if ((ch =  NextChar()) == '-') {		// How about a sign
							eSign = -1;
							ch =  NextChar();
						}
						else if (ch == '+') {
							ch =  NextChar();
						}
						if (CharClass[ch] != CDigit) {				// But no exponent!
							BackUp();
							currLex.lex = LError;
							currLex.sVal = "Missing exponent value";
							return &currLex;
						}
						exponent = (double) ch - '0';						// Build exponent
						while (CharClass[(ch = NextChar())] == CDigit) {
							exponent *= 10.0;
							exponent += (double) ch - '0';
						}
						if (exponent > 300) {						// Check range
							BackUp();
							currLex.lex = LError;
							currLex.sVal = "Exponent out of range";
							return &currLex;
						}
					} // End if exp sign
					BackUp();
					exponent *= eSign;
#endif
					fVal = (isInteger) ? dVal : dVal * pow(mBase,exponent);
					currLex.lex = (isInteger) ? LInteger : LReal;
					currLex.nVal =  (neg) ? -fVal : fVal;		// Finish Number
					return &currLex;
#ifdef WantStrings
          /*
           *	Next process a string. It begins with a quote and goes on 
           *  until a closing quote. Within a string there may be no 
           *  unescaped newlines and standard C escape characters 
           *  (except the octal trick-yet) are supported. Once string 
           *  is built we have to convert to a CString to pass as a value.
           */
				case LQuote:nChar = 0;
					while ((*tokp++ = ReadStrch()) > 0) {
						if (nChar++ >= 255) {
							currLex.lex = LError;
							currLex.nVal = "Too many characters in string.";
							return &currLex;
						}
					}
					*(tokp - 1) = 0;				// Overwrite final " with end-of-string
					currLex.sVal = TokenBuff;
					currLex.lex = LString;
					return &currLex;
#endif
          /*
           *	Catch escaped end-of-line's. Have to ignore them.
           *	Also catch comments to end-of-line the same way.
           *  Note that you cannot BOTH include a comment to end-of-line 
           *  AND escape the end-of-line (can use block comment instead).
           */
				case LEscape:if ((ch =  NextChar()) == '\\') {	// Comment
          while (NextChar() != '\n') {
          }
        }
        else if (ch == '\n') {					// Escaped EOL
          while (NextChar() == '\n') {	// eat any more EOL's
          }
          BackUp();
        }
        else {
          currLex.lex = LError;
          currLex.sVal = "Invalid escape character.";
          return &currLex;
        }
					break;
          /*
           *	Deal similarly with the two kinds of delimited comment. 
           *  (Only ' implemented yet.)
           */
				case LSQuote:while ((ch = NextChar()) != '\'') {
          if (ch == 0) {
            currLex.lex = LError;
            currLex.sVal = "Unexpected end of string";
            return &currLex;
          }
        }
          continue;
          /*
           *	anything else is an error.
           */
				default:
					currLex.lex = LError;
					currLex.sVal = "Unknown character found in input";
					return &currLex;
			}	// End of switch (ch)
		}	// End of if (cc < LPrimStart)
#ifdef WantSPrims
    /*
     *	Single-char primitives are handled by common code that
     *	extracts their value from their code.
     */
		else {
			currLex.lex = cc;
			currLex.sVal = "Unknown character found in input";
			return(&currLex);
		}	// End of primitives
#else
    /*
     *	Single-char primitives are errors.
     */
		else {
			currLex.lex = LError;
			return(&currLex);
		}	// End of primitives
#endif
	}	// End of outer loop
}	// End of NextSym
//
//	The other operation the parser needs is to push a symbol back when
//	it has read more than it needs. The scanner supports 1 symbol of
//	push-back.
//
//inline
void CTextScanner::ReturnSym()
{
	pushedBack = true;
}
/*
 *	CleanLine is called to throw away all characters up to and including the next
 *	end of line character.
 */
void CTextScanner::CleanLine()
{
	char ch;
	while ((ch = NextChar()) != '\n' && ch != 0)
		;
}
//
//	Down here live the helpers.
//	First are the ones for getting characters from the text cell (and for putting
//	them back again!).
//	Note that we always bump the character pointer until we are past the end of
//	the buffer. That way if BackUp is called nextChar will still be fine.
//
char CTextScanner::NextChar()
{
	char ch = *mNextChar;
	if (ch != 0) {			// Still have chars to read
		mNextChar++;
	} else {				// Used up a line.
		//
		//	Get a new line and set up access to it.
		//
		char* text = fgets(mText, CTLineLength - 2, mIfp);
		if (text != NULL) {
			++mLineNumber;
			mNextChar = mText;
			ch = *mNextChar++;
		} else {
			//
			//	Reached end-of-file. Make sure we return it.
			//	Actually build a dummy line with an EOF in it
			//	so that a potential pushback is handled correctly.
			//
			ch = 0;
			mText[0] = 0;
			mText[1] = 0;
			mNextChar = &mText[1];
		}
	}
	return ch;
}
//
//	This one does not move the pointer forward.
//
char CTextScanner::Peek()
{
	char ch = *mNextChar;
	return ch;
}
//
//	But this one pushes it back so that we can re-read a character.
//
//inline
void CTextScanner::BackUp()
{
	--mNextChar;
}
//
//	This one is used to read charactes within strings. It knows how to
//	process escape sequences needed to embedd special chars in strings.
//
char CTextScanner::ReadStrch()
{
  char nch = NextChar();
  
	if (nch == '\n')
    return -2;
	else if (nch == '"') {
    return -1;
  }
	else if (nch == '\\') {
    nch = NextChar();
    switch (nch) {
			case't':return('\t');
			case'n':return('\n');
			case'b':return('\b');
			case'r':return('\r');
			case'f':return('\f');
			case'\\':return('\\');
			case'"':return('"');
			case'\n':return(ReadStrch());
    }
  }
  return(nch);
}
/*
 *	A couple of little helper methods for dealing with numbers.
 *	IsDigit returns true if the letter can be found in a number,
 *	correctly dealing with the different bases possible.
 */
bool CTextScanner::IsDigit(char ch) {
	bool digit = false;
	if (mBase == 10) {
		if ((ch >= '0') && (ch <= '9')) {
			digit = true;
		}
	}
#ifdef WantHex
	if (mBase == 16) {
		if ((ch >= 'A') && (ch <= 'F')) {
			digit = true;
		}
		if ((ch >= 'a') && (ch <= 'f')) {
			digit = true;
		}
	}
#endif
	return digit;
}
/*
 *	DigitValue assumes that a character has passed IsDigit and
 *	returns the appropriate value for the character, taking the
 *	base into account.
 */
int CTextScanner::DigitValue(char ch) {
	int value = -1;
	if ((ch >= '0') && (ch <= '9')) {
		value = ch - '0';
	}
#ifdef WantHex
	if ((ch >= 'A') && (ch <= 'F')) {
		value = ch - 'A' + 10;
	}
	if ((ch >= 'a') && (ch <= 'f')) {
		value = ch - 'a' + 10;
	}
#endif
	return value;
}


