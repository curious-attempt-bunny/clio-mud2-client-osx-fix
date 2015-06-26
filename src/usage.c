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
#include <stdio.h>

#include "clio.h"


void	usage(const char *s)
{
	assert(s != NULL);

	printf("Usage: clio [%sehlnuvw] [%sa account] [%sb user] [%sc upassword] [%sf folder] \\\n",s,s,s,s,s);
	printf("            [%sm map] [%sp apassword] [%sq level] [%ss lines] host [port]\n",s,s,s,s);
	printf("\n");
	printf("            %sa account      Use 'account' for autologin.\n",s);
	printf("            %sb user         Use 'user' for autologin.\n",s);
	printf("            %sc upassword    Use 'upassword' for user autologin.\n",s);
	printf("            %se              Make ENTER repeat the last command.\n",s);
	printf("            %sf folder       Use 'folder' for log files instead of 'host'.\n",s);
	printf("            %sh              Log session in HTML.\n",s);
	printf("            %sl              Log session in plain text.\n",s);
	printf("            %sm map          Map output using code page 'map'.\n",s);
	printf("            %sn              Do not enter client mode.\n",s);
	printf("            %sp apassword    Use 'apassword' for account autologin.\n",s);
	printf("            %sq level        Set sound effects level.\n",s);
	printf("            %ss lines        Set scroll-back buffer size.\n",s);
	printf("            %su              Do not highlight URLs.\n",s);
	printf("            %sv              Show Clio version number, then exit.\n",s);
	printf("            %sw              Enable alternative colour scheme.\n",s);
}
