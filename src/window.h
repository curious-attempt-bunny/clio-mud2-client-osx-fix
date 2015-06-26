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
	Supporting definitions for the various window handling routines.
*/

#ifndef WINDOW_H
#define WINDOW_H

#if defined CLIO_WINDOWS
#include "curses.h"
#else
#include <curses.h>
#endif

typedef	struct	window {
	WINDOW	*win;		/* Curses window structure */

	int	foreground;	/* Current foreground colour */
	int	background;	/* Current background colour */

	int	reversed;	/* Colours are reversed */
	int	bold;		/* Foreground is bold */

	int	logged;		/* Is output to this window logged? */

	int	nr;		/* Number of logical (displayed) rows */
	int	nc;		/* Number of logical (displayed) cols */
	int	sr;		/* Starting physical row */
	int	sc;		/* Starting physical columns */
	int	er;		/* Ending physical row */
	int	ec;		/* Ending physical columns */

	int	fr;		/* First logical line to display */

	FILE	*fp;		/* Input stream - NULL = keyboard */
} WIN;


/*
	Colour Names.

	Note that LT_ means 'light' - ie the colour is brighter.
	On ANSI screens this is usually achieved by enabling the
	'bright' video attribute.

	'YELLOW' on an ANSI screen usually appears as brown, while
	'LT_YELLOW' appears as a true yellow.

	Normal 'WHITE' text is usually gray, and LT_WHITE is a 
	true WHITE.
	
	'LT_BLACK' is gray, but a darker shade of gray than WHITE...
*/

#define BLACK		000
#define RED		001
#define GREEN		002
#define YELLOW		003
#define BLUE		004
#define MAGENTA		005
#define CYAN		006
#define WHITE		007

#define LT_BLACK	010
#define LT_RED		011
#define LT_GREEN	012
#define LT_YELLOW	013
#define LT_BLUE		014
#define LT_MAGENTA	015
#define LT_CYAN		016
#define LT_WHITE	017

WIN	*wopen(const int,const int,const int,const int,const int,const int,const int,const int);
int	wheight(WIN *);
int	wkey(WIN *);
int	wstream(WIN *,const char *);
int	wwidth(WIN *);
void	wat(WIN *,const int,const int);
void	wback(WIN *,const int);
void	wclose(WIN *);
void	wcls(WIN *);
void	wcolour(WIN *,int,int);
void	wdisplay(WIN *, const char *, ...)/*__attribute__((format(printf,2,3)))*/;
void	weol(WIN *);
void	wflush(void);
void	wfore(WIN *,const int);
void	wline(const int,const int,const int);
void	wrestore(WIN *);
void	wreverse(WIN *,const int);
void	wsave(WIN *);

#endif /* WINDOW_H */
