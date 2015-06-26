%{
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
	This file defines the state machine for parsing data
	arriving from the MUD2 server.  Note that ALL data is
	parsed by the one state machine, including both telnet
	options and ANSI colour codes, as well as the actual 
	MUD2 client mode codes.

	This file contains some non-standard lex code, and so
	requires GNU flex. Flex 2.5.4 was used during
	development. Earlier versions may work, but have not
	be tried. Standard (AT&T) lex will not work.
*/

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "clio.h"
#include "window.h"
%}

%option noyywrap
%option nounput
%option interactive
%option always-interactive

IAC		(\377)
DONT		(\376)
DO		(\375)
WONT		(\374)
WILL    	(\373)

GO_AHEAD	(\371)

SB		(\372)
SE		(\360)

NAWS		(\037)
TERMINAL_TYPE	(\030)

SEND		(\001)


ESC		(\033)
NR		([0-9])


C00		(\233)
C01		(\234)
C02		(\235)
C03		(\236)
C04		(\237)
C05		(\240)
C06		(\241)
C07		(\242)
C08		(\243)
C09		(\244)
C10		(\245)
C11		(\246)
C12		(\247)
C13		(\250)
C14		(\251)
C15		(\252)
C16		(\253)
C17		(\254)
C18		(\255)
C19		(\256)
C20		(\257)
C21		(\260)

C89		(\364)
C90		(\365)

C94		(\371)
C95		(\372)
C96		(\373)
C97		(\374)
C98		(\375)
C99		(\376)

C255		(\377\377)

%{

extern	WIN	*swin;
extern	WIN	*mwin;
extern	WIN	*iwin;

extern	char	*cliouser;
extern	char	*cliopass;
extern	char	*clioluser;
extern	char	*cliolpass;

extern	int	autoclient;
extern	int	crrepeat;
extern	int	debugging;
extern	int	showurls;
extern	int	wireplay;	/* Are we using the Wireplay/mudii colours? */
extern	int	sfx;

static	int	cursor_state = 1;

static	SOCKET	tid;

static	int	getdata(char *,int);

#define YY_INPUT(buf,result,maxsize)	result = getdata(buf,maxsize)

static	struct	telnet_options {
	int	suppress_goahead;
	int	binary;
	int	echo;
	int	terminal_type;
	int	naws;
} to = { 0, 0, 0, 0, 0 };


enum tcontrol	{
	IAC  = '\377',
	DONT = '\376',
	DO   = '\375',
	WONT = '\374',
	WILL = '\373',

	SB   = '\372',
	SE   = '\360',

	NAWS = '\037',
	TERMINAL_TYPE = '\030',

	SEND = '\001',
	IS   = '\000'
};

enum mtype {
	TELNET,	/* Not in client mode */
	CLIENT,	/* In client mode, but not in the game */
	GAME	/* In client mode, and in the game */
};

static	int	mode = TELNET;		/* Current mode */
static	struct	account_info ai;	/* Account info */
static	struct	frontend_score fes;	/* FES info */

static	int	cliouser_sent = 0;	/* Autologin account send? */
static	int	cliopass_sent = 0;	/* Autologin password send? */
static	int	clioluser_sent = 0;	/* Autologin account send? */
static	int	cliolpass_sent = 0;	/* Autologin password send? */

static	int	ansi_bold = 0;		/* ANSI bold mode active? */

static	int	prompt_allowed = 1;
static	int	sendfes = 1;
%}

%%
	/* Telnet options */

{IAC}{DO}.	{
			switch (*(yytext+2)) {
			case 1: /* Echo - rfc857 */
				twont(tid,1);
				/*if (to.echo == 0) {
					twill(tid,*(yytext+2));
					to.echo = 1;
				}*/
				break;
			case TERMINAL_TYPE: /* rfc1091 */
				if (to.terminal_type == 0) {
					twill(tid,TERMINAL_TYPE);
					to.terminal_type = 1;
				}
				break;
			case NAWS: /* rfc1073 */
				if (to.naws == 0) {
					twill(tid,NAWS);
					tnaws(tid,80,21);
					to.naws = 1;
				}
				break;
			case 32: /* Terminal speed - rfc1079 */
			case 33: /* Toggle flow control - rfc1080 */
			case 35: /* X Display location - rfc1096 */
			case 36: /* Environ - rfc1408 */
			case 37: /* Authentication - rfc1416 */
			case 39: /* New environ - rfc1572 */
				twont(tid,*(yytext+2));
				break;
			default:
				twont(tid,*(yytext+2));
				break;
			}
		}

{IAC}{DONT}.	{
			/*twont(tid,*(yytext+2));*/
		}

{IAC}{WILL}.	{
			switch (*(yytext+2)) {
			case 1:
				break;
			case 3:	/* Suppress GO-AHEAD - rfc858 */
				if (to.suppress_goahead == 0) {
					tdo(tid,*(yytext+2));
					to.suppress_goahead = 1;
				}
				break;
			case 5: /* Status - rfc859 */
				tdont(tid,*(yytext+2));
				break;
			default:
				tdont(tid,*(yytext+2));
				break;
			}
		}

{IAC}{WONT}.	{
			/*tdont(tid,*(yytext+2));*/
		}

{IAC}{SB}{TERMINAL_TYPE}{SEND}{IAC}{SE}	{
			if (to.terminal_type == 1) {
				tterm(tid,"ansi");
			}
		}

{IAC}{GO_AHEAD}	;



	/* ANSI escape sequences */
{ESC}"[J"	|
{ESC}"[2J"	{
			wcls(mwin);

			/* Reset to end of scroll buffer */
			mwin->fr = ((mwin->nr)-1) - (mwin->er - mwin->sr);
			pnoutrefresh(mwin->win,mwin->fr,0,mwin->sr,mwin->sc,mwin->er,mwin->ec);
		}

{ESC}"[f"	|
{ESC}"[H"	{ wat(mwin,mwin->fr,0); }

{ESC}"["{NR}+;{NR}+"H"	{
			int	r,c;
			char	*ptr;

			ptr = strtok(yytext,"\033[;H");
			r = atoi(ptr);
			ptr = strtok(NULL,"\033[;H");
			c = atoi(ptr);
			wat(mwin,mwin->fr+r,c);
		}

{ESC}"[K"	{ weol(mwin); }

{ESC}"[m"	|
{ESC}"[0;0m"	{
			ansi_bold = 0;
			wreverse(mwin,0);
			if (wireplay) {
				wfore(mwin,LT_WHITE);
			} else {
				wfore(mwin,WHITE);
			}
			wback(mwin,BLACK);
		}

{ESC}"["{NR}+"m"		|
{ESC}"["{NR}+;{NR}+"m"		|
{ESC}"["{NR}+;{NR}+;{NR}+"m"	|
{ESC}"[0;37;40\r\nm"		{	/* Bug workaround for mud2.com */
		int	nr;
		char	*ptr;

		ptr = strtok(yytext,"\033[;m");
		while (ptr != NULL) {
			nr = atoi(ptr);
			ptr = strtok(NULL,"\033[;m");

			switch (nr) {
			case 0:
				ansi_bold = 0;
				wreverse(mwin,0);
				break;
			case 1:
				ansi_bold = 1;
				break;
			case 7:
				wreverse(mwin,1);
				break;
			case 30: case 31: case 32: case 33:
			case 34: case 35: case 36: case 37:
				if (ansi_bold) {
					wfore(mwin,nr-22);
				} else {
					wfore(mwin,nr-30);
				}
				break;
			case 40: case 41: case 42: case 43:
			case 44: case 45: case 46: case 47:
				wback(mwin,nr-40);
				break;
			}
		}
	}


	/* Autologin stuff ... */
"login: "	{
			
			char sbuf[100];
			if (clioluser != NULL && !clioluser_sent) {
				sprintf(sbuf,"%s\r\n",clioluser);
				tx(tid,sbuf,strlen(sbuf));
				clioluser_sent = 1;
			}
			wdisplay(mwin,"%s",yytext);
		}
^"Account ID: "	{
			char sbuf[100];
			if (cliouser != NULL && !cliouser_sent) {
				sprintf(sbuf,"%s\r\n",cliouser);
				tx(tid,sbuf,strlen(sbuf));
				cliouser_sent = 1;
			}
			wdisplay(mwin,"%s",yytext);
		}

^[Pp]"assword:"	{
			char sbuf[100];
			wdisplay(mwin,"%s",yytext);

			if (cliolpass != NULL && clioluser_sent && !cliolpass_sent){
				sprintf(sbuf,"%s\r\n",cliolpass);
				tx(tid,sbuf,strlen(sbuf));
				cliolpass_sent = 1;
			}

			if (cliopass != NULL && cliouser_sent && !cliopass_sent){
				sprintf(sbuf,"%s\r\n",cliopass);
				tx(tid,sbuf,strlen(sbuf));
				cliopass_sent = 1;
			}
		}


^"Option: "			|
^"Option (H for help): "	{
		/*
			ESC ^F		Client mode
			ESC - T		Text mode (78x21)
		*/
		if (autoclient && mode == TELNET) {
			tx(tid,"\033\006\033-T",5);
		}
		wdisplay(mwin,"%s",yytext);
		if (mode == GAME) {
			mode = CLIENT;
		}
	}


	/* MUD2 client mode codes */
{ESC}-C	{
		wcls(mwin);

		/* Reset to end of scroll buffer */
		mwin->fr = ((mwin->nr)-1) - (mwin->er - mwin->sr);
		pnoutrefresh(mwin->win,mwin->fr,0,mwin->sr,mwin->sc,mwin->er,mwin->ec);
		mode = CLIENT;
	}
{ESC}-R	{
		wreverse(mwin,1);
		mode = CLIENT;
	}
{ESC}-r	{
		wreverse(mwin,0);
		mode = CLIENT;
	}
{ESC}-K	{	weol(mwin);
		mode = CLIENT;
	}


{C00}{C255}		{
				if (mode == CLIENT || mode == GAME) {
					sclient(swin,&ai);
				}
				mode = CLIENT;

				init_stack(wireplay?LT_WHITE:WHITE,BLACK);
			}


{C01}{C255}({C01}{C04}{C255}&{C255})?({C01}{C05}{C255}>{C255})?([\(\[]*){C01}({C01}|{C02}|{C03}){C255}([^\377]+){C255}([\)\]]*){C255}	{
				if (prompt_allowed) {
					prompt_allowed = 0;
					REJECT;	
				}
				sendfes = 1;
			}
{C01}{C255}		|
{C01}{C04}{C255}	|
{C01}{C05}{C255}	push(BLUE,BLACK);
{C01}{C01}{C255}	|
{C01}{C02}{C255}	|
{C01}{C03}{C255}	{
				push(LT_BLUE,BLACK);
				if (mode != GAME) {
					/*
						This is a little work around
						for the wiz /DIR command
						messing up the status line.
					*/
					mode = GAME;
					wcls(swin);
					sscore(swin,&fes);
					sdream(swin,fes.dreamword);
				}
			}


{C02}{C00}{C255}	push(BLACK,GREEN);
{C02}{C01}{C255}	{
				push(LT_GREEN,BLACK);
				if (mode != GAME) {
					txfes(tid);
					wcls(swin);
					mode = GAME;
					fes.init_score = -1;
					fes.sv = -1;
				}
			}
{C02}{C02}{C255}	push(GREEN,BLACK);


{C03}{C00}{C255}	|
{C03}{C00}{C00}{C255}	|
{C03}{C00}{C01}{C255}	|
{C03}{C00}{C02}{C255}	|
{C03}{C00}{C03}{C255}	push(GREEN,BLACK);
{C03}{C01}{C255}	|
{C03}{C01}{C00}{C255}	|
{C03}{C01}{C01}{C255}	|
{C03}{C01}{C02}{C255}	|
{C03}{C01}{C03}{C255}	push(CYAN,BLACK);
{C03}{C02}{C255}	|
{C03}{C02}{C00}{C255}	|
{C03}{C02}{C01}{C255}	|
{C03}{C02}{C02}{C255}	|
{C03}{C02}{C03}{C255}	|
{C03}{C03}{C255}	|
{C03}{C03}{C00}{C255}	|
{C03}{C03}{C01}{C255}	|
{C03}{C03}{C02}{C255}	|
{C03}{C03}{C03}{C255}	push(LT_CYAN,BLACK);


{C04}{C00}{C255}	|
{C04}{C00}{C00}{C255}	|
{C04}{C00}{C01}{C255}	|
{C04}{C00}{C02}{C255}	|
{C04}{C00}{C03}{C255}	|
{C04}{C00}{C04}{C255}	|
{C04}{C00}{C05}{C255}	|
{C04}{C00}{C06}{C255}	|
{C04}{C00}{C07}{C255}	|
{C04}{C00}{C08}{C255}	push(MAGENTA,BLACK);
{C04}{C01}{C00}{C255}	|
{C04}{C01}{C255}	|
{C04}{C01}{C01}{C255}	|
{C04}{C01}{C02}{C255}	|
{C04}{C01}{C03}{C255}	|
{C04}{C01}{C04}{C255}	|
{C04}{C01}{C05}{C255}	|
{C04}{C01}{C06}{C255}	|
{C04}{C01}{C07}{C255}	|
{C04}{C01}{C08}{C255}	push(LT_MAGENTA,BLACK);


{C05}{C00}{C255}	|
{C05}{C00}{C00}{C255}	|
{C05}{C00}{C01}{C255}	|
{C05}{C00}{C02}{C255}	|
{C05}{C00}{C03}{C255}	|
{C05}{C00}{C04}{C255}	|
{C05}{C00}{C05}{C255}	|
{C05}{C00}{C06}{C255}	|
{C05}{C00}{C07}{C255}	|
{C05}{C00}{C08}{C255}	|
{C05}{C00}{C10}{C255}	push(RED,BLACK);
{C05}{C00}{C09}{C255}	push(LT_YELLOW,BLACK);
{C05}{C01}{C255}	|
{C05}{C01}{C00}{C255}	|
{C05}{C01}{C01}{C255}	|
{C05}{C01}{C02}{C255}	|
{C05}{C01}{C03}{C255}	|
{C05}{C01}{C04}{C255}	|
{C05}{C01}{C05}{C255}	|
{C05}{C01}{C06}{C255}	|
{C05}{C01}{C07}{C255}	|
{C05}{C01}{C08}{C255}	|
{C05}{C01}{C10}{C255}	push(LT_RED,BLACK);
{C05}{C01}{C09}{C255}	push(LT_YELLOW,BLACK);


{C06}{C255}		|
{C06}{C00}{C255}	|
{C06}{C01}{C255}	|
{C06}{C02}{C255}	|
{C06}{C03}{C255}	|
{C06}{C04}{C255}	|
{C06}{C05}{C255}	|
{C06}{C07}{C255}	|
{C06}{C08}{C255}	|
{C06}{C09}{C255}	|
{C06}{C10}{C255}	|
{C06}{C11}{C00}{C255}	|
{C06}{C11}{C01}{C255}	|
{C06}{C12}{C00}{C255}	|
{C06}{C12}{C01}{C255}	{
				txfes(tid);
				sound(6,255,255);
				push(LT_BLUE,BLACK);
			}
{C06}{C06}{C255}	{	/* "Something magical" */
				sound(6,255,255);
				push(LT_BLUE,BLACK);
			}


{C07}{C255}		{
				sound(7,0,0);
				txfes(tid);
				push(RED,BLACK);
			}
{C07}{C00}{C00}{C255}	|
{C07}{C00}{C01}{C255}	|
{C07}{C00}{C02}{C255}	|
{C07}{C01}{C00}{C255}	|
{C07}{C01}{C01}{C255}	|
{C07}{C02}{C00}{C255}	|
{C07}{C02}{C01}{C255}	|
{C07}{C02}{C02}{C255}	|
{C07}{C02}{C03}{C255}	{
				unsigned char n1, n2;

				n1 = *(yytext+1);
				n2 = *(yytext+2);

				sound(7,(unsigned)(n1-155),(unsigned)(n2-155));
				txfes(tid);
				push(RED,BLACK);
			}
{C07}{C03}{C255}	|
{C07}{C04}{C255}	|
{C07}{C05}{C255}	|
{C07}{C06}{C255}	{
				unsigned char n1;

				n1 = *(yytext+1);
				sound(7,(unsigned)(n1-155),255);
				txfes(tid);
				push(RED,BLACK);
			}


{C08}{C01}{C255}	{
				sound(8,1,255);
				txfes(tid);
				push(LT_RED,BLACK);
			}
{C08}{C03}{C255}	{
				sound(8,3,255);
				txfes(tid);
				push(LT_RED,BLACK);
			}
{C08}{C255}		|
{C08}{C00}{C255}	|
{C08}{C02}{C255}	|
{C08}{C04}{C255}	push(RED,BLACK);
{C08}{C05}{C255}	|
{C08}{C06}{C255}	|
{C08}{C07}{C255}	|
{C08}{C09}{C255}	push(WHITE,BLACK);
{C08}{C08}{C255}	{
				txfes(tid);
				push(BLACK,RED);
			}
{C08}{C10}{C255}	|
{C08}{C11}{C255}	|
{C08}{C12}{C255}	{
				txfes(tid);
				push(RED,BLACK);
			}
{C08}{C13}{C255}	push(BLACK,RED);

{C09}{C00}{C255}	push(YELLOW,BLACK);
{C09}{C01}{C255}	|
{C09}{C02}{C255}	|
{C09}{C03}{C255}	|
{C09}{C04}{C255}	|
{C09}{C05}{C255}	|
{C09}{C05}{C00}{C255}	|
{C09}{C05}{C01}{C255}	|
{C09}{C05}{C02}{C255}	|
{C09}{C06}{C255}	|
{C09}{C07}{C255}	|
{C09}{C08}{C255}	|
{C09}{C09}{C255}	|
{C09}{C10}{C255}	push(LT_YELLOW,BLACK);

{C10}{C00}{C255}	|
{C10}{C03}{C255}	push(BLACK,YELLOW);
{C10}{C01}{C255}	|
{C10}{C02}{C255}	|
{C10}{C04}{C255}	push(wireplay?LT_YELLOW:LT_RED,YELLOW);


{C11}{C255}		|
{C11}{C06}{C255}	|
{C11}{C09}{C255}	|
{C11}{C14}{C255}	{
				/*
					06 = FOD
					09 = WHERE
					14 = SUMMON
				*/
				push(LT_RED,BLACK);
			}

{C11}{C00}{C255}	|
{C11}{C01}{C255}	|
{C11}{C02}{C255}	|
{C11}{C03}{C255}	|
{C11}{C04}{C255}	|
{C11}{C05}{C255}	|
{C11}{C07}{C255}	|
{C11}{C08}{C255}	|
{C11}{C10}{C255}	|
{C11}{C11}{C255}	|
{C11}{C12}{C255}	|
{C11}{C13}{C255}	|
{C11}{C15}{C255}	|
{C11}{C16}{C255}	|
{C11}{C17}{C255}	|
{C11}{C18}{C255}	|
{C11}{C19}{C255}	|
{C11}{C20}{C255}	|
{C11}{C21}{C255}	{
				unsigned char n1;

				n1 = *(yytext+1);
				sound(11,(unsigned)(n1-155),255);

				txfes(tid);
				push(LT_RED,BLACK);
			}


{C12}{C255}		|
{C12}{C01}{C255}	|
{C12}{C02}{C255}	|
{C12}{C02}{C01}{C255}	|
{C12}{C02}{C02}{C255}	|
{C12}{C03}{C255}	|
{C12}{C03}{C01}{C255}	|
{C12}{C03}{C02}{C255}	push(WHITE,BLACK);
{C12}{C04}{C255}	|
{C12}{C05}{C255}	push(GREEN,BLACK);
{C12}{C06}{C255}	push(YELLOW,BLACK);
{C12}{C07}{C255}	push(LT_YELLOW,BLACK);
{C12}{C08}{C01}{C255}.+\n	{		/* Front end score */
			char *ptr;
			if ((ptr = strchr(yytext,'\376')) != NULL) {
				fes.scolour = (char)(*(ptr+1) - 155);
			}
			if ((ptr = strpbrk(yytext,"0123456789")) != NULL) {
				if ((ptr = strtok(ptr," ")) != NULL) {
					fes.sta = atoi(ptr);
				}
			}
			if ((ptr = strpbrk(yytext+25,"0123456789")) != NULL) {
				if ((ptr = strtok(ptr," ")) != NULL) {
					fes.msta = atoi(ptr);
				}
			}

			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.str = atoi(ptr);
			}
			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.mstr = atoi(ptr);
			}

			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.dex = atoi(ptr);
			}
			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.mdex = atoi(ptr);
			}

			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.mag = atoi(ptr);
			}
			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.mmag = atoi(ptr);
			}

			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.score = atol(ptr);
				if (fes.init_score == -1) {
					fes.init_score = fes.score;
				}
			}

			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.blind = (*ptr == 'Y') ? 1 : 0;
			}
			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.deaf = (*ptr == 'Y') ? 1 : 0;
			}
			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.crippled= (*ptr == 'Y') ? 1 : 0;
			}
			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.dumb= (*ptr == 'Y') ? 1 : 0;
			}

			if ((ptr = strtok(NULL," ")) != NULL) {
				fes.reset = atoi(ptr);
			}

			if ((ptr = strtok(NULL," ")) != NULL) {
				*fes.weather = *ptr;
			}

			sscore(swin,&fes);
			push(WHITE,BLACK);
	}
{C12}{C08}{C02}{C255}	|
{C12}{C08}{C03}{C255}	|
{C12}{C08}{C04}{C255}	|
{C12}{C09}{C255}	|
{C12}{C10}{C255}	push(WHITE,BLACK);

{C13}{C255}		|
{C13}..{C255}		{
				unsigned char n1;

				if (strlen(yytext) == 5) {
					n1 = *(yytext+1);
					sound(13,(unsigned)(n1-155),255);
				}
				push(wireplay ? LT_BLACK : WHITE, wireplay ? WHITE : BLACK);
			}
^"Out from the end of the cannon flies a projectile, which smashes" {
		if (mode == GAME) {
			sound(13,13,255);
		}
		wdisplay(mwin,"%s",yytext);
	}
^"HAWUMPH! The dragon incinerates you with its fiery breath." {
		if (mode == GAME) {
			sound(13,25,255);
		}
		wdisplay(mwin,"%s",yytext);
	}
^"You hear a near-deafening crash, as if millions of gallons of water" {
		if (mode == GAME) {
			sound(13,26,255);
		}
		wdisplay(mwin,"%s",yytext);
	}


{C14}{C00}{C255}	{ txfes(tid); push(GREEN,BLACK); }
{C14}{C01}{C255}	{
				/*- It is snowing [heavily] -*/
				if (*fes.weather != 'S' && *fes.weather != 'B') {
					txfes(tid);
				}
				if (wireplay) {
					push(LT_WHITE,GREEN);
				} else {
					push(BLACK,WHITE);
				}
			}
{C14}{C02}{C255}	{
				/*- It is raining [heavily] -*/
				if (*fes.weather != 'R' && *fes.weather != 'T') {
					txfes(tid);
				}
				push(GREEN,BLACK);
			}

{C14}{C03}{C00}{C255}	{ txfes(tid); push(GREEN,BLACK); }
{C14}{C03}{C01}{C255}	{
				txfes(tid);
				if (wireplay) {
					push(LT_WHITE,GREEN);
				} else {
					push(BLACK,WHITE);
				}
			}
{C14}{C03}{C02}{C255}	{
				sound(14,3,2);
				txfes(tid);
				push(GREEN,BLACK);
			}
{C14}{C03}{C03}{C255}	{
				txfes(tid);
				if (wireplay) {
					push(LT_WHITE,GREEN);
				} else {
					push(BLACK,WHITE);
				}
			}

{C14}{C04}{C00}{C255}	|
{C14}{C04}{C02}{C255}	{ sweather(swin,"F"); push(GREEN,BLACK); }
{C14}{C04}{C01}{C255}	|
{C14}{C04}{C03}{C255}	{
				sweather(swin,"F");
				if (wireplay) {
					push(LT_WHITE,GREEN);
				} else {
					push(BLACK,WHITE);
				}
			}

{C14}{C05}{C00}{C255}	{ txfes(tid); push(GREEN,BLACK); }
{C14}{C05}{C01}{C255}	{
				txfes(tid);
				if (wireplay) {
					push(LT_WHITE,GREEN);
				} else {
					push(BLACK,WHITE);
				}
			}

{C14}{C06}{C00}{C255}	|
{C14}{C06}{C01}{C255}	|
{C14}{C06}{C02}{C255}	{ txfes(tid); push(GREEN,BLACK); } /* rainbow */



{C15}{C00}{C255}	push(BLACK,CYAN);
{C15}{C00}{C00}{C255}[a-z]{1,14}	{ /* Dreamword */
			sdream(swin,yytext+5);
			push(BLACK,CYAN);
			wdisplay(mwin,"%s",yytext+5);
			sprintf(fes.dreamword,"%s",yytext+5);
		}
{C15}{C00}{C01}{C255}	{
			/*
				The effect of saying a
				dreamword.
			*/
			txfes(tid);
			sdream(swin,"");
			push(BLACK,CYAN);
			memset(fes.dreamword,0,sizeof(fes.dreamword));
		}
{C15}{C01}{C255}	push(BLACK,CYAN);

{C16}.{C255}		|
{C16}{C00}{C00}{C255}	|
{C16}{C00}{C01}{C255}	|
{C16}{C00}{C02}{C255}	|
{C16}{C00}{C03}{C255}	|
{C16}{C00}{C04}{C255}	|
{C16}{C00}{C05}{C255}	|
{C16}{C00}{C06}{C255}	{
				/* House messages */
				if (wireplay) {
					push(LT_WHITE,YELLOW);
				} else {
					push(LT_WHITE,BLUE);
				}
			}

{C18}{C00}{C255}	|
{C18}{C01}{C255}	|
{C18}{C02}{C255}	|
{C18}{C03}{C255}	|
{C18}{C04}{C255}	|
{C18}{C05}{C255}	|
{C18}{C06}{C255}	{
				unsigned char n1;

				n1 = *(yytext+1);
				sound(18,(unsigned)n1-155,255);
				push(WHITE,BLACK);
				txfes(tid);
			}

{C19}{C00}{C255}	|
{C19}{C01}{C255}	|
{C19}{C02}{C255}	|
{C19}{C03}{C255}	|
{C19}{C04}{C255}	|
{C19}{C05}{C255}	push(LT_WHITE,BLUE);

{C89}{C00}{C00}		|
{C89}{C00}{C01}		|
{C89}{C01}		push(WHITE,BLACK);

{C90}{C255}		catch();
{C90}{C01}{C255}	throw();

{C94}...{C255}[^\377]+	push(WHITE,BLACK);	/* Snoop starts */

{C95}{C255}.+\n.+\n.+\n.+\n.+\n {
		/*	Start of client mode
	
			Licence nr<CR>
			Client min level<CR>
			Client max level<CR>
			Account<CR>
			Privs<CR>
		*/
		char	*ptr;

		if ((ptr = strtok(yytext+3,"\r\n")) != NULL) {
			sprintf(ai.licence,"%.25s",ptr);
		}
		if ((ptr = strtok(NULL,"\r\n")) != NULL) {
			ai.minclient = atoi(ptr);
		}
		if ((ptr = strtok(NULL,"\r\n")) != NULL) {
			ai.maxclient = atoi(ptr);
		}
		if ((ptr = strtok(NULL,"\r\n")) != NULL) {
			sprintf(ai.account,"%.25s",ptr);
		}
		if ((ptr = strtok(NULL,"\r\n")) != NULL) {
			ai.privs = atoi(ptr);
		}

		sclient(swin,&ai);
		push(WHITE,BLACK);
		mode = CLIENT;
	}
{C95}{C02}{C255}.+\n.+\n {
		/*	Account priv level change.
	
			Account<CR>
			Privs<CR>
		*/
		char	*ptr;

		if ((ptr = strtok(yytext+4,"\r\n")) != NULL) {
			sprintf(ai.account,"%.25s",ptr);
		}
		if ((ptr = strtok(NULL,"\r\n")) != NULL) {
			ai.privs = atoi(ptr);
		}

		sclient(swin,&ai);
		push(WHITE,BLACK);
		mode = CLIENT;
	}
{C95}{C03}{C255}.+\n{C255}	;	/* Account logout */

{C96}{C255}...	push(WHITE,BLACK); /* Snoop ends*/

{C98}.{C255}	{
			/*
				odd =blue
				even =magenta
			*/
			if (*(yytext+1)/2 * 2 == *(yytext+1)) {
				push(BLACK,BLUE);
			} else {
				push(BLACK,MAGENTA);
			}
			prompt_allowed = 1;
		}

{C99}..{C255}	{
			char f, b;
	
			f = *(yytext+1) - 155;
			b = *(yytext+2) - 155;
			push(f,b);
		}
{C99}{C99}{C255}	{
			push(wireplay?LT_WHITE:WHITE,BLACK);
		}
{C99}.{C255}	{
			char f;
	
			f = *(yytext+1) - 155;
			push(f,BLACK);
		}

{C255}		{
			if (pop() == -1) {
				/* commented out to hide mud2.com bug */
				/*wdisplay(mwin,"%c",255);*/
			}
		}

([\233-\376]{1,4}){C255}	push(wireplay?LT_WHITE:WHITE,BLACK);


[Ff][Tt][Pp]"://"([^., "\0\r\n\t)>\033\377]|((\.|,)[^., "\0\r\n\t)>\033\377]))+	|
[Ww][Aa][Ii][Ss]"://"([^., "\0\r\n\t)>\033\377]|((\.|,)[^., "\0\r\n\t)>\033\377]))+	|
[Tt][Ee][Ll][Nn][Ee][Tt]"://"([^., "\0\r\n\t)>\033\377]|((\.|,)[^., "\0\r\n\t)>\033\377]))+	|
[Gg][Oo][Pp][Hh][Ee][Rr]"://"([^., "\0\r\n\t)>\033\377]|((\.|,)[^., "\0\r\n\t)>\033\377]))+	|
[Hh][Tt][Tt][Pp]([Ss]?)"://"([^., "\0\r\n\t)>\033\377]|((\.|,)[^., "\0\r\n\t)>\033\377]))+ {
		if (showurls) {
			wsave(mwin);
			wcolour(mwin,LT_BLUE,BLACK);
			/*push(LT_BLUE,BLACK);*/
		}
		url_start(mwin,yytext);
		wdisplay(mwin,"%s",yytext);
		url_end(mwin);
		if (showurls) {
			wrestore(mwin);
			/*pop();*/
		}
	}

^"Old Password:"	|
^"New time-out period:"	|
^"1 - Old Man. 2 - Young Girl. 3 - Dwarf. Q to quit /BR." {
	wdisplay(mwin,"%s",yytext);
	if (mode == GAME) {
		sendfes = 0;
	}
}


\007		beep();

\r\0	;

\r?\n	{
		wdisplay(mwin,"\n");
		wflush();
		prompt_allowed = 1;
	}

.	{
		wdisplay(mwin,"%s",yytext);
	}

%%


#define	MSG_OK		"[OK]\n"
#define	MSG_FAILED	"[Failed]\n"



static int	connectto(struct in_addr *addr, unsigned short portnr)
{
	struct	sockaddr_in sa;

	if ( (tid = socket(AF_INET,SOCK_STREAM,0)) == SFAIL) {
		printf("Failed to create socket.\n");
		return -1;
	}

	memset(&sa,0,sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(portnr);
	memcpy(&sa.sin_addr,addr,sizeof(struct in_addr));

	printf("Connecting to %s...",inet_ntoa(sa.sin_addr));
	fflush(stdout);
	if (connect(tid,(struct sockaddr *)&sa,sizeof(sa)) == -1) {
		printf(MSG_FAILED);
		closesocket(tid);
		return -1;
	}
	printf(MSG_OK);
	return 0;
}


int	telnet(const char *hostname, const char *port)
{
	struct	servent *se;	/* Struct for getservbyname() */
	unsigned short	portnr;		/* Port as number (host order) */
	long	lport;
	char	*errpos;

	struct	hostent	*he;
	struct	in_addr	**pptr;
	struct	in_addr	ia;

	assert(hostname != NULL);
	assert(port != NULL);

	tinit();	/* Initialise sockets */

	/*- Resolve port -*/
	lport = strtol(port,&errpos,0);
	if ((*errpos != 0) || (lport < 1) || (lport > SHRT_MAX) ) {
		printf("Resolving port %s...",port);
		fflush(stdout);
		if ((se = getservbyname(port,"tcp")) == NULL) {
			printf(MSG_FAILED);
			return -1;
		}
		printf(MSG_OK);
		portnr = ntohs(se->s_port);
	} else {
		portnr = lport;
	}


	/*- Resolve host -*/
	/*
		inet_aton() would be a better choice to use here,
		instead of inet_addr, but it's not supported by
		Winsock 2.0.
	*/
	if ((ia.s_addr = inet_addr(hostname)) != INADDR_NONE) {
		if (connectto(&ia,portnr) != -1) {
			return 0;
		}
	} else {
		printf("Resolving host %s...",hostname);
		fflush(stdout);
		if ((he = gethostbyname(hostname)) == NULL) {
			printf(MSG_FAILED);
			return -1;
		}
		printf(MSG_OK);

		pptr = (struct in_addr **)he->h_addr_list;
		for (; *pptr != NULL; pptr++) {
			if (connectto(*pptr,portnr) != -1) {
				return 0;
			}
		}
	} 

	return -1;
}


static	const	char	*sound_level[] = {
		"sound effects disabled",
		"standard sound effects enabled",
		"extended sound effects enabled",
};


static	int	getdata(char *buf, int maxsize)
{
	int	status;		/* select() return status */
	struct	timeval tmout;	/* select() timeout structure */
	fd_set	fds;		/* select() fd set */

	int	ch;			/* Input character */
	static	char	ibuf[257];	/* Input buffer */
	static	int	ipos = 0;	/* Current position in ibuf */
	static	int	ilen = 0;	/* Total length of ibuf */
	static	char	tmp[257];


	int	nr;
	char	cmd[30];

	static	time_t	now = 0;
	static	time_t	then = 0;


	if (now  == 0) { now  = time(NULL); }
	if (then == 0) { then = time(NULL); }


	do {	/* While waiting on output... */

		/*- Send an escaped FES every 10 seconds -*/
		if (mode == GAME) {
			now = time(NULL);
			if (difftime(now,then) >= 10) {
				if (sendfes) txfes(tid);
				then = now;
			}
		}

		/*- Process keyboard input -*/
		switch (ch = wkey(iwin)) {

		/* Function keys F1 to F12 */
		case KEY_F(1):  fkey(tid,1);  break;
		case KEY_F(2):  fkey(tid,2);  break;
		case KEY_F(3):  fkey(tid,3);  break;
		case KEY_F(4):  fkey(tid,4);  break;
		case KEY_F(5):  fkey(tid,5);  break;
		case KEY_F(6):  fkey(tid,6);  break;
		case KEY_F(7):  fkey(tid,7);  break;
		case KEY_F(8):  fkey(tid,8);  break;
		case KEY_F(9):  fkey(tid,9);  break;
		case KEY_F(10): fkey(tid,10); break;
		case KEY_F(11): fkey(tid,11); break;
		case KEY_F(12): fkey(tid,12); break;

		/* Function keys F13 to F24 */
		case KEY_F(13): fkey(tid,13); break;
		case KEY_F(14): fkey(tid,14); break;
		case KEY_F(15): fkey(tid,15); break;
		case KEY_F(16): fkey(tid,16); break;
		case KEY_F(17): fkey(tid,17); break;
		case KEY_F(18): fkey(tid,18); break;
		case KEY_F(19): fkey(tid,19); break;
		case KEY_F(20): fkey(tid,20); break;
		case KEY_F(21): fkey(tid,21); break;
		case KEY_F(22): fkey(tid,22); break;
		case KEY_F(23): fkey(tid,23); break;
		case KEY_F(24): fkey(tid,24); break;

		/* Function keys F25 to F36 */
		case KEY_F(25): fkey(tid,25); break;
		case KEY_F(26): fkey(tid,26); break;
		case KEY_F(27): fkey(tid,27); break;
		case KEY_F(28): fkey(tid,28); break;
		case KEY_F(29): fkey(tid,29); break;
		case KEY_F(30): fkey(tid,30); break;
		case KEY_F(31): fkey(tid,31); break;
		case KEY_F(32): fkey(tid,32); break;
		case KEY_F(33): fkey(tid,33); break;
		case KEY_F(34): fkey(tid,34); break;
		case KEY_F(35): fkey(tid,35); break;
		case KEY_F(36): fkey(tid,36); break;

		case 1: /* CTRL-A - Jump to end of scrollback */
			mwin->fr = ((mwin->nr)-1) - (mwin->er - mwin->sr);
			pnoutrefresh(mwin->win,mwin->fr,0,mwin->sr,mwin->sc,mwin->er,mwin->ec);
			
			break;

		case 2: /* CTRL-B - Move left one character */
		case KEY_LEFT:
			if (ipos) --ipos;
			break;

		case 3: /* CTRL-C - Set cursor visibility level */
			cursor_state = (cursor_state == 2) ? 1 : 2;
			curs_set(cursor_state);
			break;

		case 4:	/* CTRL-D - Speak the dreamword */
			if (mode == GAME && *fes.dreamword != 0) {
				sprintf(cmd,"say \"%s\"\r\n",fes.dreamword);
				tx(tid,cmd,strlen(cmd));
			}
			break;

		case 5: /* CTRL-E - Go to end of input -*/
		case KEY_END:
			ipos = ilen;
			break;

		case 6: /* CTRL-F - Move right one character */
		case KEY_RIGHT:
			if (ipos < ilen) ++ipos;
			break;

		case KEY_DC: /* Delete char to right of cursor */
			if (ipos < ilen) {
				--ilen;
				strcpy(tmp,ibuf);
				strcpy(tmp+ipos,ibuf+ipos+1);
				strcpy(ibuf,tmp);
				break;
			}
			/*FALLTHROUGH*/

		case 8:	/* CTRL-H - Delete one char to left of cursor */
		case KEY_BACKSPACE:
			if (ipos) {
				--ipos;
				--ilen;
				strcpy(tmp,ibuf);
				strcpy(tmp+ipos,ibuf+ipos+1);
				strcpy(ibuf,tmp);
			}
			break;

		case 12: /* CTRL-L - Repeat last command */
			strcpy(ibuf,hprevious());
			hadd(ibuf); /* needed ? */
			strcat(ibuf,"\r\n");
			tx(tid,ibuf,strlen(ibuf));

			memset(ibuf,0,256);
			ipos = 0;
			ilen = 0;
			break;

		case 14: /* CTRL-N */
		case KEY_DOWN:
			memset(ibuf,0,256);
			strcpy(ibuf,hnext());
			despace(ibuf);
			ipos = ilen = strlen(ibuf);
			break;

		case 15: /* CTRL-O - Jump to start of input buffer -*/
		case KEY_HOME:
			ipos = 0;
			break;

		case 16: /* CTRL-P - Recall previous command -*/
		case KEY_UP:
			memset(ibuf,0,256);
			strcpy(ibuf,hprevious());
			despace(ibuf);
			ipos = ilen = strlen(ibuf);
			break;

		case 18: /* CTRL-R - Redraw screen -*/
			wrefresh(curscr);
			break;

		case '\r':
		case '\n':
		case KEY_ENTER:
#if defined CLIO_WINDOWS
		case PADENTER:
#endif /* CLIO_WINDOWS */
			/*if (crrepeat && mode == GAME && strlen(ibuf) == 0) {*/
			if (crrepeat && strlen(ibuf) == 0) {
				strcpy(ibuf,hprevious());
			}
			/* Add to history */
			if (iwin->fp == NULL) {
				hadd(ibuf);
			}

			strcat(ibuf,"\r\n");
			tx(tid,ibuf,strlen(ibuf));

			memset(ibuf,0,256);
			ipos = 0;
			ilen = 0;

			/* Jump to end of scroll buffer */
			mwin->fr = ((mwin->nr)-1) - (mwin->er - mwin->sr);
			pnoutrefresh(mwin->win,mwin->fr,0,mwin->sr,mwin->sc,mwin->er,mwin->ec);
			break;

		case 27:
		case KEY_DL:
		case 21: /*- CTRL-U - Clear input buffer -*/
			memset(ibuf,0,256);
			ipos = 0;
			ilen = 0;
			break;

		case KEY_NPAGE:
			if (mwin->fr < (mwin->nr - 1) - (mwin->er - mwin->sr)) {
				++(mwin->fr);
				pnoutrefresh(mwin->win,mwin->fr,0,mwin->sr,mwin->sc,mwin->er,mwin->ec);
			}
			break;

		case KEY_PPAGE:
			if (mwin->fr > 0) {
				--(mwin->fr);
				pnoutrefresh(mwin->win,mwin->fr,0,mwin->sr,mwin->sc,mwin->er,mwin->ec);
			}
			break;


		case 23: /*- CTRL-W - Sound level -*/
			if (++sfx > 2) {
				sfx = 0;
			}
			wdisplay(mwin,"\n+- Clio %s. -+\n",sound_level[sfx]);
			wflush();
			break;

		case 25: /* CTRL-Y - Upload file -*/
			if (ai.privs >= 2) { /* Wizzes only */
				if (wstream(iwin,ibuf) != -1) {
					memset(ibuf,0,256);
					ipos = 0;
					ilen = 0;

					/* Jump to end of scroll buffer */
					mwin->fr = ((mwin->nr)-1) - (mwin->er - mwin->sr);
					pnoutrefresh(mwin->win,mwin->fr,0,mwin->sr,mwin->sc,mwin->er,mwin->ec);
				}
			}
			break;

		default:
			if (ilen < 254 && ((ch >= 0x20 && ch <= 0x7E) || (ch >= 0xA0 && ch <= 0xFF))) {
				strcpy(tmp,ibuf);
				strcpy(tmp+ipos+1,ibuf+ipos);
				strcpy(ibuf,tmp);
				
				ibuf[ipos++] = ch;
				++ilen;
			} else {
/*
				if (ch > 0) wdisplay(mwin,"KEY=%d\n",ch);
*/
			}
		}

		if (ipos <= 79) {
			wat(iwin,0,0);
			wdisplay(iwin,"%-79s ",ibuf);
			wat(iwin,0,ipos);
		} else {
			wat(iwin,0,0);
			wdisplay(iwin,"%-79s ",ibuf+(ipos-79));
		}
		wflush();


		/*- Check for pending screen info -*/
		tmout.tv_sec = 0;
		tmout.tv_usec = 50; /* 50ms */

		FD_ZERO(&fds);
		FD_SET(tid,&fds);
	} while ((status = select(tid+1,&fds,NULL,NULL,&tmout)) == 0);

	if (status == -1 && errno != EINTR) {
		/* EINTER from SIGWINCH.  */
		return YY_NULL;
	}

	if ((nr = recv(tid,buf,maxsize,0)) <= 0) {
		return YY_NULL;
	}

	if (debugging) {
		static	FILE	*dfp;

		if (dfp == NULL) {
			dfp = fopen("clio.debug","w+");
		}
		if (dfp != NULL) {
			fwrite(buf,(size_t)nr,1,dfp);
			fflush(dfp);
		}
	}

	return nr;
}


int	ismud2(void)
{
	return mode == TELNET ? 0 : 1;
}
