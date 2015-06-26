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
	void	chomp(char *s);

	Remove any trailing CR or NL characters from a string.


	void	chomps(char *s);

	As per chomp(), but also removes any trailing spaces.
	See also despace();
*/

#include <string.h>

#include "clio.h"


void	chomp(char *s)
{
	long	len;

	if (s == NULL) return;

	if ((len = (long)strlen(s)) < 1L) return;
	--len;
	
	while (len >= 0L && (*(s+len) == '\r' || *(s+len) == '\n')) {
		*(s+len) = '\0';
		--len;
	}
}

void	chomps(char *s)
{
	long	len;

	if (s == NULL) return;

	if ((len = (long)strlen(s)) < 1L) return;
	--len;
	
	while (len >= 0L && (*(s+len) == '\r' || *(s+len) == '\n' || *(s+len) == ' ')) {
		*(s+len) = '\0';
		--len;
	}
}
