/*
*	CharClass.cpp
*
*	This is a file which defines the characters class array used by Lex to decide
*	what sort of thing the next character is. It is an array of 256 characters
*	where CharClass[ch] is the type of the particular character, ch.
*	BC 6/96 adapted from older DAL versions. This array does not use multi-char primitives
*	but treats all punctuation chars as primitives.
*/
#include "Lexemes.h"
#include "CTextScanner.h"
/*
*	Then comes the actual table. For the lower chars I have put in the actual chars as comments
*	to make it clearer.
*/
CClass CTextScanner::CharClass[256] = {
//  NULL  ,   ^A,	^B,   ^C,   ^D,   ^E,	^F,  BEL,   BS,   HT ,  LF ,   VT,  FF,  CR  ,   ^N,   ^O,
  LEndFile, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr,LSpace,LTerm, LErr, LErr,LTerm, LErr, LErr,

//  ^P,   ^Q,	^R,   ^S,   ^T,   ^U,	^V,   ^W,   ^X,   ^Y,	^Z,  ESC,   FS,   GS,	RS,   US,
  LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr,

//    ,    !,	"  ,  # ,   $   ,    %,	 & ,   '   ,   ( ,    ),    * ,    +,    , ,    -,    .   ,   /   ,
LSpace,LBang,LQuote, LErr,LDollar, LErr, LAnd,LSQuote,LLBra,LRBra,LTimes,LPlus,LComma,LMinus,LPeriod,LDivide,

//  0 ,	  1  ,	2   ,    3 ,    4 ,	  5  ,	6   ,    7 ,    8 ,	  9  ,	:   ,   ;  , <  ,   =   ,  > ,  ? ,
CDigit,CDigit,CDigit,CDigit,CDigit,CDigit,CDigit,CDigit,CDigit,CDigit,LColon,LSemiC,LLT,LEquals, LGT, LQuery,

//  @,	  A ,	B  ,    C ,    D ,	  E ,	F  ,    G ,    H ,	  I ,	J  ,    K ,    L ,	  M ,	N  ,    O ,
 LErr,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,

//  P ,	  Q  ,	R   ,    S ,    T ,	  U  ,	V   ,    W ,    X ,	  Y  ,	Z   ,    [ ,    \  ,	  ]  ,	^ ,    _ ,
CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,LLSBra,LEscape,LRSBra,LExp,CAlpha,

//   ,	  a ,	b  ,    c ,    d ,	  e ,	f  ,    g ,    h ,	  i ,	j  ,    k ,    l ,	  m ,	n  ,    o ,

 LErr,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,

//  p ,	  q  ,	r   ,    s ,    t ,	  u  ,	v   ,    w ,    x ,	  y  ,	z   ,    { ,  | ,	  } ,	~,  DEL 
CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,CAlpha,LLCBra, LOr,LRCBra, LErr, LErr,

  LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr,
  LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr,
  LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr,
  LErr, LErr, LErr, LErr , LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr,
  LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr,
  LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr,
  LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr,
  LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr, LErr};
  
  

