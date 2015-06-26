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
	Command history management functions.

	The history is held in a static buffer, and is accessed via
	a pointer which loops around through the buffer.
*/

#include <string.h>

#include "clio.h"

#define	HISTSIZE	40

static	char	h[HISTSIZE][256];
static	int	hptr;		/* Next free slot in h[] */
static	int	sptr;		/* Last line returned by next/prev */


void	hadd(const char *s)
{
	int	lasthptr;

	lasthptr = (hptr == 0) ? HISTSIZE-1 : hptr-1;

	if (s == NULL || *s == '\0' || strcmp(s,h[lasthptr]) == 0) {
		sptr = hptr;
		return;
	}

	strcpy(h[hptr],s);
	if (++hptr >= HISTSIZE) {
		hptr = 0;
	}
	sptr = hptr;

	/* Clear the current (new) history line */
	strcpy(h[hptr]," ");
}


char	*hprevious(void)
{
	int	i;

	for (i = 0; i <= HISTSIZE; i++) {
		if (--sptr < 0) sptr = HISTSIZE-1;
		if (*h[sptr] != 0) {
			return h[sptr];
		}
	}
	return h[sptr];
}


char	*hnext(void)
{
	int	i;

	for (i = 0; i <= HISTSIZE; i++) {
		if (++sptr >= HISTSIZE) sptr = 0;
		if (*h[sptr] != 0) {
			return h[sptr];
		}
	}
	return h[sptr];
}
