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
	void	sweather(WIN *w, const char *s);

	Displays the current weather condition on the status window.

	WIN *w points to the status window, const char *s points to
	a single character describing the current weather.

	s can be one of:

		F - It is sunny (F=Fair)
		C - It is cloudy
		R - It is raining
		S - It is snowing
		O - It is overcast (A storm brewing)
		T - Heavy rain (T=Thunder)
		B - Heavy snow (B=Blizard)

	If s is any other code, or NULL, the weather description is
	cleared.
*/

#include <assert.h>
#include <string.h>
#include <time.h>

#include "clio.h"
#include "window.h"


void	sweather(WIN *w,const char *s)
{
	char	*ptr;
	int	i;

	const char *wcodes = "FCRSOTB";

	static 	struct weather {
		const char	*desc;
		int	fg;
		int	bg;
	} weather[] = {
		{"Sunny", LT_YELLOW, BLACK},	/* F - It is sunny */
		{"Cloud", WHITE, BLACK},	/* C - It is cloudy */
		{"Rain",  GREEN, BLACK},	/* R - It is raining */
		{"Snow",  BLACK, WHITE},	/* S - It is snowing */
		{"Cloud", LT_BLACK, BLACK},	/* O - It is overcast */
		{"Storm", GREEN, BLACK},	/* T - Heavy rain */
		{"Storm", BLACK, WHITE}		/* B - Heavy snow */
	};

	extern	int wireplay;
/*
	static	char lastweather = 'X';
	static	time_t lastchange = 0;
	static	FILE *fp;
*/

	assert(w != NULL);

	if (wireplay) {
		weather[3].fg = LT_WHITE;
		weather[3].bg = GREEN;

		weather[6].fg = LT_WHITE;
		weather[6].bg = GREEN;
	}

	wat(w,0,70);
	wcolour(w,BLACK,BLACK);
	wdisplay(w,"       ");

	if (s != NULL && (ptr = strchr(wcodes,*s)) != NULL) {
		wat(w,0,71);
		wcolour(w,WHITE,BLACK); /* ? */
		i = ptr - wcodes ;
		wcolour(w,weather[i].fg,weather[i].bg);
		wdisplay(w,weather[i].desc);
	}

/*
	if (lastweather == 'X') {
		lastweather = *s;
		fp = fopen("weather.txt","a+");
		if (fp != NULL) {
			fprintf(fp,"\n");
			fflush(fp);
		}
	}

	if (lastweather != *s) {
		if (fp != NULL && lastchange != 0) {
			fprintf(fp,"%c -> %c %8.2f\n",lastweather,*s,difftime(time(0),lastchange));
			fflush(fp);
		}
		lastchange = time(0);
	}
	lastweather = *s;
*/
}
