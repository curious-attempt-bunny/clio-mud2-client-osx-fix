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
#include <stdlib.h>

#include "clio.h"
#include "window.h"


/*
	colourcode();

	Returns the colour used to display magic, dexterity, or
	strength, based on the difference between the current and
	maximum values.

	The colouring used for stamina is provided by MUD2 as part
	of the FES output.

	Note that the current effective dexterity or strength can
	be greater than the 'maximum'(!)
*/
static	int	colourcode(const int eff, const int max)
{
	int	ratio;

	if (eff <= 0 || max <= 0) {
		return LT_GREEN;
	}

	ratio = (eff*100)/max;

	if (ratio >= 100) return LT_GREEN;	/* Bright green */
	if (ratio >=  76) return GREEN;		/* Green */
	if (ratio >=  36) return LT_YELLOW;	/* Yellow */
	if (ratio >=  16) return YELLOW;	/* Brown */
	if (ratio >=   6) return RED;		/* Red */
	return LT_RED;				/* Bright red */
}


void	sscore(WIN *w, const struct frontend_score *fes)
{
	int	score_colour;

	assert(w != NULL);
	assert(fes != NULL);

	/*- STATS -*/
	wcolour(w,CYAN,BLACK);
	wat(w,0,0);
	if (fes->mag > 0) {
		wdisplay(w,"Sta:   /    Mag:    Dex:    Str:   ");

		wcolour(w,fes->scolour,BLACK);
		wat(w,0, 4); wdisplay(w,"%3d",fes->sta);
		wcolour(w,LT_GREEN,BLACK);
		wat(w,0, 8); wdisplay(w,"%d",fes->msta);

		wcolour(w,colourcode(fes->mag,fes->mmag), BLACK);
		wat(w,0,16); wdisplay(w,"%d",fes->mag);

		wcolour(w,colourcode(fes->dex,fes->mdex), BLACK);
		wat(w,0,24); wdisplay(w,"%d",fes->dex);

		wcolour(w,colourcode(fes->str,fes->mstr), BLACK);
		wat(w,0,32); wdisplay(w,"%d",fes->str);
	} else {
		wdisplay(w,"Sta:   /    Dex:   /    Str:   /   ");

		wcolour(w,fes->scolour,BLACK);
		wat(w,0, 4); wdisplay(w,"%3d",fes->sta);

		wcolour(w,LT_GREEN,BLACK);
		wat(w,0, 8); wdisplay(w,"%d",fes->msta);

		wcolour(w,colourcode(fes->dex,fes->mdex), BLACK);
		wat(w,0,16); wdisplay(w,"%3d",fes->dex);

		wcolour(w,LT_GREEN,BLACK);
		wat(w,0,20); wdisplay(w,"%d",fes->mdex);

		wcolour(w,colourcode(fes->str,fes->mstr), BLACK);
		wat(w,0,28); wdisplay(w,"%3d",fes->str);

		wcolour(w,LT_GREEN,BLACK);
		wat(w,0,32); wdisplay(w,"%d",fes->mstr);
	}


	/*- SCORE -*/
	/*
		Score is displayed as:

		Pts:999		Score is less that 1000.
		999,999		Score is between 1000 and 999999.
	        9999999		Score is between 1000000 and 9999999.
		Pts:99m		Score is above 9999999. (10m to 99m)


		The score is initially displayed in brown (YELLOW),
		and changes to green if points have been gained 
		this reset, or to red if they have been lost.
	*/
	score_colour = YELLOW;
	if (fes->score > fes->init_score) score_colour = GREEN;
	if (fes->score < fes->init_score) score_colour = RED;
	
	wat(w,0,36);
	if (fes->score <= 999L) {
		wcolour(w,CYAN,BLACK);
		wdisplay(w,"Pts:");
		wcolour(w,score_colour,BLACK);
		wdisplay(w,"%3ld",fes->score);
	}

	if (fes->score >= 1000L && fes->score <= 999999L) {
		wcolour(w,score_colour,BLACK);
		wdisplay(w,"%3ld",fes->score/1000L);
		wcolour(w,CYAN,BLACK);
		wdisplay(w,",");
		wcolour(w,score_colour,BLACK);
		wdisplay(w,"%03ld",fes->score%1000L);
	}

	if (fes->score >= 1000000L && fes->score <= 9999999L) {
		wcolour(w,score_colour,BLACK);
		wdisplay(w,"%7ld",fes->score);
	}

	if (fes->score >= 10000000L) {
		wcolour(w,CYAN,BLACK);
		wdisplay(w,"Pts:");
		wcolour(w,score_colour,BLACK);
		wdisplay(w,"%2ld",fes->score/1000000L);
		wcolour(w,CYAN,BLACK);
		wdisplay(w,"m");
	}


	/*- HEALTH -*/
	wat(w,0,44);
	wcolour(w,BLACK,BLACK);
	wdisplay(w,"           ");

	wat(w,0,44);
	wcolour(w,RED,BLACK);
	switch(fes->deaf+fes->dumb+fes->blind+fes->crippled) {
	case 1:
        	wdisplay(w,fes->deaf != 0     ? "Deaf " : "");
        	wdisplay(w,fes->dumb != 0     ? "Dumb " : "");
        	wdisplay(w,fes->blind != 0    ? "Blind" : "");
        	wdisplay(w,fes->crippled != 0 ? "Crippled" : "");
		break;
	case 2:
		if (fes->crippled == 0) {
        		wdisplay(w,fes->deaf != 0     ? "Deaf " : "");
        		wdisplay(w,fes->dumb != 0     ? "Dumb " : "");
        		wdisplay(w,fes->blind != 0    ? "Blind" : "");
			break;
		}
		/*@fallthrough@*/
	default:
        	wdisplay(w,fes->deaf != 0     ? "Df " : "");
        	wdisplay(w,fes->dumb != 0     ? "Dm " : "");
        	wdisplay(w,fes->blind != 0    ? "Bl " : "");
        	wdisplay(w,fes->crippled != 0 ? "Cr " : "");
		break;
	}


	/*- WEATHER -*/
	sweather(w,fes->weather);


	/*- RESET -*/
	wat(w,0,77);
	wcolour(w,LT_BLUE,BLACK);
	wdisplay(w,"%3d",fes->reset);
}
