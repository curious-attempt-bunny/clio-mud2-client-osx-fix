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
#include <stdlib.h>

#include "clio.h"
#include "window.h"

static	short	fg[] = {
	COLOR_WHITE, COLOR_BLACK, COLOR_RED, COLOR_GREEN,
	COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN
} ;

static	short	bg[] = {
	COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
	COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE
} ;


static	void	wend(void)
{
	if (isendwin() == FALSE) {
		(void)endwin();
	}
}


static	int	winit(void)
{
	int	f, b;
	short	pair;

	if (initscr() == NULL) {
		return -1;
	}

	if (atexit(wend) == -1) {
		wend();
		return -1;
	}

	if (LINES < 24 || COLS < 80) {
		return -1;
	}

	(void)cbreak();
	(void)noecho();
	(void)nl();
	(void)erase();

	if (typeahead(-1) == ERR) {
		return -1;
	}

	if (has_colors() == TRUE && start_color() != ERR) {
		if (COLORS >= 8 && COLOR_PAIRS >= 64) {
			for (f = 0 ; f <= 7 ; f++) {
				for (b = 0 ; b <= 7 ; b++) {

					if (b == 0 && f == 0) {
						continue ;
					}

					pair = (short)(f + (b * 8)) ;
					init_pair(pair,fg[f],bg[b]);
				}
			}
		}
	}

	return 0;
}


WIN	*wopen(const int nr, const int nc, const int sr, const int sc, const int er, const int ec, const int doscroll,const int logged)
{
	static	int initialised = 0;
	WIN	*w;

	/*- Initialise curses -*/
	if (initialised == 0) {
		if (winit() == -1) {
			return NULL;
		}
		initialised = 1;
	}

	if ((w = malloc(sizeof(WIN))) == NULL) {
		return NULL;
	}

	if ((w->win = newpad(nr,nc)) != NULL) {

		w->fp = NULL;

		w->nr = nr;
		w->nc = nc;
		w->sr = sr;
		w->sc = sc;
		w->er = er;
		w->ec = ec;

		w->fr = (w->nr - 1) - (w->er - w->sr);

		if (doscroll != 0) {
			(void)scrollok(w->win,TRUE);
		} else {
			(void)scrollok(w->win,FALSE);
		}
		(void)leaveok(w->win,FALSE);
		(void)keypad(w->win,TRUE);
		(void)nodelay(w->win,TRUE);
		(void)intrflush(w->win,FALSE);

		w->logged = logged;
		w->reversed = 0;
		w->bold = 0;

		w->background = w->foreground = -1;

		wcls(w);
		wcolour(w,WHITE,BLACK);

		wat(w,w->fr,0);

		return w;
	} else {
		free(w);
		return NULL;
	}
}
