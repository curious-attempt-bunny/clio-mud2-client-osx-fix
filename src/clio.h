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

#ifndef CLIO_H
#define CLIO_H

#include "config.h"
#include "window.h"

/*
	The current Clio version number.

	Keep this shorter than 40 characters, or sversion()
	will not display it.
*/
#define	CLIO_VERSION	"1.8"


/*- Socket Stuff, Stuff for Sockets -*/

/* UNIX Sockets */
#if defined CLIO_UNIX

#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/select.h>
#include <arpa/inet.h>

typedef int SOCKET;
#define	SFAIL		-1
#define	closesocket	close

/* Solaris doesn't define INADDR_NONE */
#if !defined INADDR_NONE
#define INADDR_NONE -1
#endif

#endif /* CLIO_UNIX */

/* Winsock */
#if defined CLIO_WINDOWS
#include <winsock.h>
#define SFAIL		INVALID_SOCKET
#endif /* CLIO_WINDOWS */


struct  account_info {
	char	licence[26];
	int	minclient;
	int	maxclient;
	char	account[26];
	int	privs;
};

struct	frontend_score {
	int	sta;		/* Effective stamina */
	int	scolour;	/* Colour code for effective stamina */
	int	msta;		/* Maximum stamina */
	int	mag; 		/* Effective magic */
	int	mmag; 		/* Maximum magic */
	int	dex;		/* Effective dexterity */
	int	mdex;		/* Maximum dexterity */
	int	str;		/* Effective strength */
	int	mstr;		/* Maximum strength */
	long	score;		/* Score */
	long	init_score;	/* Initial score */
	int	deaf;		/* 1 = Deaf */
	int	dumb;		/* 1 = Dumb */
	int	blind;		/* 1 = Blind  */
	int	crippled;	/* 1 = Crippled */
	int	reset;		/* Minutes until reset */
	char	weather[2];	/* Weather */
	char	dreamword[15];	/* Last dreamword */
	int	sv;		/* Nr of mobs still alive */
};


/*- Function prototypes -*/
char	*hnext(void);
char	*home(void);
char	*hprevious(void);
char	*ini(const char *);
const char *date(void);
int	ismud2(void);
int	load_map(const char *);
int	options(int, char *const*, const char *);
int	pop(void);
int	telnet(const char *, const char *);
int	tinit(void);
int	yylex(void);
void	caption(const char *, ...)/*__attribute__((format(printf,1,2)))*/;
void	catch(void);
void	chomp(char *);
void	chomps(char *);
void	csig(void);
void	default_map(void);
void	despace(char *);
void	fkey(const SOCKET, const int);
void	hadd(const char *);
void	html_footer(FILE *, const char *, const char *);
void	html_header(FILE *, const char *, const char *);
void	init_stack(const int, const int);
void	lcolour(WIN *, const int, const int);
void	ldisplay(WIN *, const char *);
void	makedir(const char *);
void	push(const int, const int);
void	sclient(WIN *, const struct account_info *);
void	sdream(WIN *, const char *);
void	sound(unsigned int, unsigned int, unsigned int);
void	sscore(WIN *, const struct frontend_score *);
void	start_logging(const char *, const char *, const char *);
void	stop_logging(const char *, const char *);
void	sversion(WIN *);
void	sweather(WIN *, const char *);
void	tdo(const SOCKET tid, const unsigned char);
void	tdont(const SOCKET tid, const unsigned char);
void	throw(void);
void	tnaws(const SOCKET tid, const unsigned char, const unsigned char); 
void	tterm(const SOCKET tid, const char *);
void	twill(const SOCKET tid, const unsigned char);
void	twont(const SOCKET tid, const unsigned char);
void	tx(const SOCKET, const char *, const size_t);
void	txfes(const SOCKET);
void	url_end(WIN *);
void	url_start(WIN *,const char *);
void	usage(const char *);

#if defined CLIO_OSS
void	playsound(const char *);
#endif

#endif /* CLIO_H */
