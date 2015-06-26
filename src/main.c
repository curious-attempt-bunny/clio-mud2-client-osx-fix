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
	int	main(int, char **);
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "clio.h"
#include "window.h"


/*
	Pointers to various windows.
*/
WIN	*swin;	/* Status window */
WIN	*mwin;	/* Main window */
WIN	*iwin;	/* Input window */


/*
	Account name and password given on command line, or read
	from CLIOUSER and CLIOPASS in the environment.
*/
char	*cliouser = NULL;
char	*cliopass = NULL;
char	*clioluser = NULL;
char	*cliolpass = NULL;

/*
	Flags set in response to command line args.
*/
int	autoclient = 1;		/* 1 = Automatic entry to client mode */
int	debugging = 0;		/* 1 = Debugging on */
int	showurls = 1;		/* 1 = Highlight URLs in blue */
int	crrepeat = 0;		/* 1 = CR repeats last command */
int	logging = 0;		/* 1 = Log session */
int	htmllog = 0;		/* 1 = Log in HTML */
int	wireplay = 0;		/* 1 = Use wireplay colour scheme */
int	sfx = 2;		/* 0 = none, 1=standard, 2=extended */

static	long	scrollback = 300L;	/* Size of scrollback buffer */

/*
	Used by options();
*/
extern	int	optind;
extern  char	*optarg;


static	void	show_version(void)
{
	printf("Clio MUD2 client for %s.\n",CLIO_OS);
	printf("Copyright (c) 2003 Ian Peattie.\n");
	printf("All rights reserved.\n");
	printf("OSI Certified Open Source Software.\n");
	printf("\n");
	printf("Version %s. ",CLIO_VERSION);
	printf("[build: %s, %s]\n",__DATE__,__TIME__);
}


int	main(int argc, char *argv[])
{
	const char	*failure = "Unable to create %s window\n";

	const char	*host = NULL;	/* Hostname */
	const char	*port = NULL;	/* Port number */
	const char	*folder = NULL;	/* Log folder */
	const char	*map = NULL;	/* Codepage map */

	int	c;			/* Option letter */
	int	screensize;

	csig();	/* Trap signals */

	caption("Clio");

	/* Parse options */
	while ((c = options(argc,argv,"a:A:b:B:c:C:dDeEf:F:hHlLm:M:nNp:P:q:Q:s:S:uUvVwW")) != -1) {
		switch (tolower(c)) {
		case 'a':
			cliouser = optarg;
			break;
		case 'b':
			clioluser = optarg;
			break;
		case 'c':
			cliolpass = optarg;
			break;
		case 'd':
			debugging = 1;
			break;
		case 'e':
			crrepeat = 1;
			break;
		case 'f':
			folder = optarg;
			break;
		case 'h':
			htmllog = 1;
			logging = 1;
			break;
		case 'l':
			logging = 1;
			break;
		case 'm':
			map = optarg;
			break;
		case 'n':
			autoclient = 0;
			break;
		case 'p':
			cliopass = optarg;
			break;
		case 'q':
			sfx = atoi(optarg);
			break;
		case 's':
			scrollback = atol(optarg);
			break;
		case 'u':
			showurls = 0;
			break;
		case 'v':
			show_version();
			exit(EXIT_SUCCESS);
			break;
		case 'w':
			wireplay = 1;
			break;
		case '?':
			usage(CLIO_OPTCHAR);
			exit(EXIT_FAILURE);
			break;
		}
	}


	/*- Get host and port from command line -*/
	switch (argc - optind) {
	case 0:
		host = getenv("CLIOHOST");
		break;
	case 1:
		host = argv[optind];
		break;
	case 2:
		host = argv[optind];
		port = argv[optind+1];
		break;
	default:
		usage(CLIO_OPTCHAR);
		exit(EXIT_FAILURE);
	}

	if (host == NULL) {
		usage(CLIO_OPTCHAR);
		exit(EXIT_FAILURE);
	}

	if (port == NULL) {
		if ((port = getenv("CLIOPORT")) == NULL) {
			port = "23";
		}
	}

	/*- Get account and password from environment -*/
	if (cliouser  == NULL) cliouser  = getenv("CLIOUSER");
	if (cliopass  == NULL) cliopass  = getenv("CLIOPASS");
	if (clioluser == NULL) clioluser = getenv("CLIOLUSER");
	if (cliolpass == NULL) cliolpass = getenv("CLIOLPASS");


	/*- Load code page map -*/
	if (map != NULL) {
		if (load_map(map) == -1) {
			exit(EXIT_FAILURE);
		}
	} else {
		default_map();
	}

	/*- Establish telnet connection -*/
	if (telnet(host,port) == -1) {
		exit(EXIT_FAILURE);
	}
	caption("Clio - %s",host);

	/*- Create status window -*/
	if ((swin = wopen(1,80,0,0,0,79,0,0)) == NULL) {
		printf(failure,"status");
		exit(EXIT_FAILURE);
	}

	screensize = wheight(NULL);

	/*- Create main window -*/
	if ((mwin = wopen(21+scrollback,80,2,0,screensize-3,79,1,1)) == NULL) {
		printf(failure,"main");
		exit(EXIT_FAILURE);
	}
	wcolour(mwin,WHITE,BLACK);

	/*- Create input window -*/
	if (wheight(NULL) == 24) {
		/* Small screen - no lower horizontal line */
		iwin = wopen(1,80,23,0,23,79,0,0);
	} else {
		wline(screensize-2,0,80);
		iwin = wopen(1,80,screensize-1,0,screensize-1,79,0,0);
	}
	if (iwin == NULL) {
		printf(failure,"input");
		exit(EXIT_FAILURE);
	}

	wline(1,0,80);
	sversion(swin);

#if defined CLIO_WINDOWS
	wcolour(mwin,LT_YELLOW,BLACK);
	wdisplay(mwin,"\nUse ");

	wcolour(mwin,LT_WHITE,BLACK);
	wdisplay(mwin,"ALT-ENTER");

	wcolour(mwin,LT_YELLOW,BLACK);
	wdisplay(mwin," to toggle full screen mode.\n");

	wcolour(mwin,WHITE,BLACK);
#endif /* CLIO_WINDOWS */

	wcolour(iwin,BLACK,WHITE);
	wcls(iwin);
	wdisplay(iwin,"%80s","");

	if (folder == NULL) folder = host;
	start_logging(host,port,folder);

	init_stack(WHITE,BLACK);
	wflush();

	(void)yylex();
	caption("Clio");


	wcolour(mwin,BLACK,RED);
	wdisplay(mwin," Connection closed by %s ",host);
	wcolour(iwin,WHITE,BLACK);
	wcls(iwin);
	wflush();

	stop_logging(host,port);
	logging = 0;

	return EXIT_SUCCESS;
}
