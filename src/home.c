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
	char	*home(void);

	Return the user's home directory.

	For Unix, this is read from HOME, or failing that, from
	the password file.

	For Windows, the current directory (".") is always
	returned.
*/

#include <stdio.h>
#include <stdlib.h>

#include "clio.h"


#if defined CLIO_WINDOWS
char	*home(void)
{
	return ".";
}
#endif /* CLIO_WINDOWS */


#if defined CLIO_UNIX
#include <pwd.h>

char	*home(void)
{
	char	*ptr;
	struct	passwd *pw;

	if ((ptr = getenv("HOME")) != NULL) {
		return ptr;
	}

	if ((ptr = getlogin()) != NULL) {
		if ((pw = getpwnam(ptr)) != NULL) {
			return pw->pw_dir;
		}
	}

	return NULL;
}
#endif /* CLIO_UNIX */
