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
	void	fkey(const SOCKET, const int);

	Send the command bound to function key fk via SOCKET tid;

	fk should be in the range 1 to 36;
*/

#include <string.h>

#include "clio.h"

void	fkey(const SOCKET tid, const int fk)
{
	char	key[5];		/* ini file 'key', eg "F1=" */
	char	msg[260];	/* Command to send. */
	char	*ptr;		
	size_t	len;

	if (fk < 1 || fk > 36) {
		return;
	}

	sprintf(key,"F%d=",fk);
	if ((ptr = ini(key)) == NULL) {
		/* No command bound to this key */
		return;
	}

	len = strlen(ptr);
	if (len < 1 || len > 254) {
		return;
	}

	sprintf(msg,"%s\r\n",ptr);
	len += 2;
	tx(tid,msg,len);
}
