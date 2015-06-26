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
	Return a keypress.
*/

#include <assert.h>
#include <ctype.h>

#include "clio.h"
#include "window.h"

int	wkey(WIN *w)
{
	int	ch;

	assert(w != NULL);

	if (w->fp != NULL) {
		while ((ch = getc(w->fp)) != EOF) {

			/* Replace tabs with spaces */
			if (ch == '\t') {
				ch = ' ';
			}

			if (!isprint(ch) && !isspace(ch)) {
					continue;
			}

#if defined CLIO_WINDOWS
			if (ch == '\n') {
				Sleep(100);
			}
#endif
			return ch;
		}

		/* EOF */
		(void)fclose(w->fp);
		w->fp = NULL;
		
		/* Fall through to return wgetch() value */
	}

	return wgetch(w->win) ;
}
