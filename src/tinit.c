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
	int	tinit(void);

	Initialisation code for the sockets library.  This is
	currently only needed for the Windows winsock DLL.
*/

#include <stdlib.h>

#include "clio.h"

#if defined CLIO_WINDOWS
static	void	endwinsock(void)
{
	WSACleanup();
}
#endif	/* CLIO_WINDOWS */


int	tinit(void)
{
#if defined CLIO_WINDOWS
	static	int	already_called = 0;
	WSADATA	WSAData;

	if (already_called) {
		return 0;
	}
	already_called = 1;

	if (WSAStartup(MAKEWORD(2,0),&WSAData) != 0) {
		return -1;
	}
/*
	WSAData.szDescription tells us the WinSock version

	printf("%s\n",WSAData.szDescription);
*/

	if (atexit(endwinsock) == -1) {
		WSACleanup();
		return -1;
	}
#endif /* CLIO_WINDOWS */

	return 0;
}
