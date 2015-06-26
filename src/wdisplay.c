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
	void	wdisplay(WIN *, const char *, ...);
*/

#include <assert.h>
#include <stdarg.h>

#include "clio.h"
#include "window.h"

static	int o_codepage[256];
static	int i_codepage[256];

int	load_map(const char *mapname)
{
	int	i;
	FILE	*fp;
	char	path[512];

	printf("Loading %s code page map...",mapname);
	fflush(stdout);

	sprintf(path,"%s/%s",CLIO_MAPS,mapname);
	if ( (fp = fopen(path,"rb")) == NULL) {
		printf("[Failed]\n");
		return -1;
	}

	for (i = 0; i <= 255; i++) {
		if ( (o_codepage[i] = fgetc(fp)) == EOF) {
			fclose(fp);
			printf("[Failed]\n");
			return -1;
		}
	}

	fclose(fp);

	printf("[OK]\n");

	return 0;
}


void	default_map(void)
{
	int	i;

	printf("Loading ISO-8859-1 code page map...");
	for (i = 0; i <= 255; i++) {
		o_codepage[i] = i;
		i_codepage[i] = i;
	}
	printf("[OK]\n");
}


void	wdisplay(WIN *w, const char *fmt,...)
{
	unsigned int	i;
	unsigned char	ch;

	va_list	args;
	char	s[3000];	/* Arbitrarily big number */

	assert(w != NULL);

	va_start(args,fmt);
	(void)vsprintf(s,fmt,args);
	va_end(args);

	for (i = 0; i < strlen(s); i++) {
		ch = (unsigned)s[i];
		(void)wprintw(w->win,"%c",o_codepage[ch]);
	}
	/*(void)wprintw(w->win,"%s",s);*/

	if (w->logged != 0) {
		ldisplay(w,s);
	}

	(void)pnoutrefresh(w->win,w->fr,0,w->sr,w->sc,w->er,w->ec);
}


/*
	Not all curses support vw_printw(). PDCurses doesn't

void	wdisplay(WIN *w, const char *fmt,...)
{
	va_list	args;

	assert(w != NULL);

	va_start(args,fmt);
	vw_printw(w->win,fmt,args);
	va_end(args);

	pnoutrefresh(w->win,w->fr,0,w->sr,w->sc,w->er,w->ec);
}
*/
