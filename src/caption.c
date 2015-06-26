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
	void	caption(const char *, ...);

	Sets the caption of the console window. This is currently
	a NOP under UNIX, but can be used when Clio is ported to
	X Windows.

	The function takes a printf-like format string.
*/

#include <assert.h>
#include <stdarg.h>

#include "clio.h"

#if defined CLIO_WINDOWS
#include <windows.h>
#endif /* CLIO_WINDOWS */


void	caption(const char *fmt, ...)
{
	va_list	args;
	char	s[1024];

	assert(fmt != NULL);

	va_start(args,fmt);
	(void)vsprintf(s,fmt,args);
	va_end(args);

#if defined CLIO_WINDOWS
	SetConsoleTitle(s);
#endif /* CLIO_WINDOWS */
}
