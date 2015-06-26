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
	void	sdream(WIN *w, const char *s);

	Display the dreamword 's' on on the status window 'w'.

	if (s == NULL) or (s == "") the dreamword is cleared from
	the screen. This is used to remove the dreamword when it
	has been spoken.

	The dreamword is displayed right-justified in the 14
	characters starting at character 56.
*/

#include <assert.h>
#include <string.h>

#include "clio.h"
#include "window.h"

#define	MAX_DREAMWORD	14


void	sdream(WIN *w, const char *s)
{
	size_t	len;

	assert(w != NULL);

	wcolour(w,BLACK,BLACK);
	wat(w,0,55);
	wdisplay(w,"                ");

	if (s == NULL) {
		return;
	}
	len = strlen(s);

	if (len < 1 && len > MAX_DREAMWORD) {
		return ;
	}

	wat(w,0,56+(int)(14 - len));
	wcolour(w,BLACK,CYAN);
	wdisplay(w,"%s",s);
}
