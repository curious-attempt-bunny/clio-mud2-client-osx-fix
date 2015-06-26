/*
Clio MUD2 Client.
Copyright (c) 2003 Ian Peattie.
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicence, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	HTML Entity names, indexed by ISO-8859-1 code.
*/

#ifndef HTML_H
#define HTML_H

static	char	*entity[] = {
	"",	/* NUL */
	"",	/* SOH */
	"",	/* STX */
	"",	/* ETX */
	"",	/* EOT */
	"",	/* ENQ */
	"",	/* ACK */
	"",	/* BEL */
	"",	/* BS */
	"\t",	/* HT */
	"",	/* LF */
	"",	/* VT */
	"",	/* FF */
	"",	/* CR */
	"",	/* SO */
	"",	/* SI */
	"",	/* DLE */
	"",	/* DC1 */
	"",	/* DC2 */
	"",	/* DC3 */
	"",	/* DC4 */
	"",	/* NAK */
	"",	/* SYN */
	"",	/* ETB */
	"",	/* CAN */
	"",	/* EM */
	"",	/* SUB */
	"",	/* ESC */
	"",	/* FS */
	"",	/* GS */
	"",	/* RS */
	"",	/* US */
	" ",	/* SP */
	"!",	
	"&quot;",
	"#",
	"$",
	"%",
	"&amp;",
	"'",
	"(",
	")",
	"*",
	"+",
	",",
	"-",
	".",
	"/",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	":",
	";",
	"&lt;",
	"=",
	"&gt;",
	"?",
	"@",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"[",
	"\\",
	"]",
	"^",
	"_",
	"`",
	"a",
	"b",
	"c",
	"d",
	"e",
	"f",
	"g",
	"h",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",
	"p",
	"q",
	"r",
	"s",
	"t",
	"u",
	"v",
	"w",
	"x",
	"y",
	"z",
	"{",
	"|",
	"}",
	"~",
	"",	/* DEL */
	"",	/* XXX */
	"",	/* XXX */
	"",	/* BPH */
	"",	/* NBH */
	"",	/* XXX */
	"",	/* NEL */
	"",	/* SSA */
	"",	/* ESA */
	"",	/* HTS */
	"",	/* HTJ */
	"",	/* VTS */
	"",	/* PLD */
	"",	/* PLU */
	"",	/* RI */
	"",	/* SS2 */
	"",	/* SS3 */
	"",	/* DCS */
	"",	/* PU1 */
	"",	/* PU2 */
	"",	/* STS */
	"",	/* CCH */
	"",	/* MW */
	"",	/* SPA */
	"",	/* EPA */
	"",	/* SOS */
	"",	/* XXX */
	"",	/* SCI */
	"",	/* CSI */
	"",	/* ST */
	"",	/* OSC */
	"",	/* PM */
	"",	/* APC */
	"&nbsp;",	/* NBSP */
	"&iexcl;",	/* Inverted excliamation point */
	"&cent;",	/* Cent sign */
	"&pound;",	/* Pound sign */
	"&curren;",	/* General currency sign */
	"&yen;",	/* Yen sign */
	"&brvbar;",	/* Broken vertical bar */
	"&sect;",	/* Section sign */
	"&uml;",	/* Umlaut */
	"&copy;",	/* Copyright sign */
	"&ordf;",	/* Feninine ordinal */
	"&laquo;",	/* Left angle quotes */
	"&not;",	/* Not sign */
	"&shy;",	/* Soft hyphen */
	"&reg;",	/* Registered trademark */
	"&macr;",	/* Macron accent */
	"&deg;",	/* Degree sign */
	"&plusmn;",	/* Plus or minus */
	"&sup2;",	/* Superscript 2 */
	"&sup3;",	/* Superscript 3 */
	"&acute;",	/* Acute accent */
	"&micro;",	/* Micro sign */
	"&para;",	/* Plichrow sign (Paragraph sign) */
	"&middot;",	/* Middle dot */
	"&cedil;",	/* Cedilla */
	"&sup1;",	/* Superscript 1 */
	"&ordm;",	/* Masculine ordinal */
	"&raquo;",	/* Right angle quote */
	"&frac14;",	/* Fraction one-fourth */
	"&frac12;",	/* Fraction one-half */
	"&frac34;",	/* Fraction three-fourths */
	"&iquest;",	/* Inverted question mark */
	"&Agrave;",	/* Capital A, grave */
	"&Aacute;",	/* Capital A, acute */
	"&Acirc;",	/* Capital A, circumflex */
	"&Atilde;",	/* Capital A, tilde */
	"&Auml;",	/* Capital A, umlaut */
	"&Aring;",	/* Capital A, ring */
	"&AElig;",	/* Capital AE ligature */
	"&Ccedil;",	/* Capital C, cedilla */
	"&Egrave;",	/* Capital E, grave */
	"&Eacute;",	/* Capital E, acute */
	"&Ecirc;",	/* Capital E, curcimflex */
	"&Euml;",	/* Capital E, umlaut */
	"&Igrave;",	/* Capital I, grave */
	"&Iacute;",	/* Capital I, acute */
	"&Icirc;",	/* Capital I, circumflex */
	"&Iuml;",	/* Capital I, umlaut */
	"&ETH;",	/* Capital Eth */
	"&Ntilde;",	/* Capital N, tilde */
	"&Ograve;",	/* Capital O, grave */
	"&Oacute;",	/* Capital O, acute */
	"&Ocirc;",	/* Capital O, circumflex */
	"&Otilde;",	/* Capital O, tilde */
	"&Ouml;",	/* Capital O, umlut */
	"&times;",	/* Multiply sign */
	"&Oslash;",	/* Capital O, slash */
	"&Ugrave;",	/* Capital U, grave */
	"&Uacute;",	/* Capital U, acute */
	"&Ucirc;",	/* Capital U, circumflex */
	"&Uuml;",	/* Capital U, umlaut */
	"&Yacute;",	/* Capital Y, acute */
	"&THORN;",	/* Capital Thorn */
	"&szlig;",	/* Small sz ligature */
	"&agrave;",	/* Small A, grave */
	"&aacute;",	/* Small A, acute */
	"&acirc;",	/* Small A, circumflex */
	"&atilde;",	/* Small A, tilde */
	"&auml;",	/* Small A, umlaut */
	"&aring;",	/* Small A, ring */
	"&aelig;",	/* Small AE ligature */
	"&ccedil;",	/* Small C, cedilla */
	"&egrave;",	/* Small E, grave */
	"&eacute;",	/* Small E, acute */
	"&ecirc;",	/* Small E, curcimflex */
	"&euml;",	/* Small E, umlaut */
	"&igrave;",	/* Small I, grave */
	"&iacute;",	/* Small I, acute */
	"&icirc;",	/* Small I, circumflex */
	"&iuml;",	/* Small I, umlaut */
	"&eth;",	/* Small Eth */
	"&ntilde;",	/* Small N, tilde */
	"&ograve;",	/* Small O, grave */
	"&oacute;",	/* Small O, acute */
	"&ocirc;",	/* Small O, circumflex */
	"&otilde;",	/* Small O, tilde */
	"&ouml;",	/* Small O, umlut */
	"&divide;",	/* Division sign */
	"&oslash;",	/* Small O, slash */
	"&ugrave;",	/* Small U, grave */
	"&uacute;",	/* Small U, acute */
	"&ucirc;",	/* Small U, circumflex */
	"&uuml;",	/* Small U, umlaut */
	"&yacute;",	/* Small Y, acute */
	"&thorn;",	/* Small Thorn */
	"&yuml;"	/* Small Y, umlaut */
};

#endif /* HTML_H */
