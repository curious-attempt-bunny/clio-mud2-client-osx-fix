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
	void	sound(unsigned int, unsigned int, unsigned int);
*/

#include <stdio.h>
#include <stdlib.h>

#include "clio.h"


extern	int	sfx;


void	sound(unsigned int n1, unsigned int n2, unsigned int n3)
{
	char	path[FILENAME_MAX];
	const	char	*dir;

	if (sfx == 0 || (sfx == 1 && n1 != 13)) {
		return;
	}

	if ((dir = getenv("CLIOSOUNDS")) == NULL) {
		dir = CLIO_SOUNDS;
	}

	if (n3 == 255) {
		if (n2 == 255) {
			sprintf(path,"%s/%02d.wav",dir,n1);
		} else {
			sprintf(path,"%s/%02d%02d.wav",dir,n1,n2);
		}
	} else {
		sprintf(path,"%s/%02d%02d%02d.wav",dir,n1,n2,n3);
	}

#if defined CLIO_PLAYSOUND
	PlaySound(path,0,SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
#endif /* CLIO_PLAYSOUND */

#if defined CLIO_OSS
	playsound(path);
#endif /* CLIO_OSS */
}
