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

#include <assert.h>

#include "clio.h"
#include "window.h"


void	wcolour(WIN *w, int f, int b)
{
	int	nr;
	int	bold;
	int	tmp;

	assert(w != NULL);

	if (w->reversed != 0) {
		tmp = b;
		b = f;
		f = tmp;

		if (b >= 8) {
			b -= 8;
			f += 8;
		}
	}

	if (f == w->foreground && b == w->background) {
		return;
	}

	w->foreground = f;
	w->background = b;

	if (w->logged != 0) {
		lcolour(w,f,b);
	}

	bold = 0;
	if (f >= 8) {
		f -= 8;
		bold = 1;
	}

	if (++f >= 8) {
		f = 0;
	}
	nr = f + (b * 8);


	if (nr == 0) {
		wattrset(w->win,0);
	} else {
		(void)wattron(w->win,COLOR_PAIR(nr));
	}

	if (bold != 0) {
		(void)wattron(w->win,A_BOLD);
		w->bold = 1;
	} else {
		(void)wattroff(w->win,A_BOLD);
		w->bold = 0;
	}

	/*pnoutrefresh(w->win,w->fr,0,w->sr,w->sc,w->er,w->ec);*/
}
