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
	void	sclient(WIN *w);
*/

#include <assert.h>
#include <string.h>

#include "clio.h"
#include "window.h"

static	const char *plevel[] = {	/* Privilege level */
	"guest",			/* 0 = guest */
	"",				/* 1 = mortal */
	"wiz",				/* 2 = wiz */
	"arch-wiz",			/* 3 = arch-wiz */
	"master"			/* 4 = master */
};


void	sclient(WIN *w, const struct account_info *ai)
{
	char	ver[80];
	int	col;

	assert(w != NULL);
	assert(ai != NULL);

	wcls(w);
	wat(w,0,0);
	wcolour(w,CYAN,BLACK);
	wdisplay(w,"Client mode, level %d ",ai->maxclient);

	switch (ai->privs) {
	case 0:	/* Guest */
	case 2:	/* Wiz */
	case 3:	/* Arch */
	case 4:	/* Master */
		wdisplay(w,"(%s privileges)",plevel[ai->privs]);
		break;
	}

	/*CONSTCOND*/
	if (strlen(CLIO_VERSION) < 40) {
		sprintf(ver,"Version %s",CLIO_VERSION);
		col = wwidth(w)-strlen(ver);
		wat(w,0,col);
		wdisplay(w,ver);
	}
}
