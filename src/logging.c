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
#include <string.h>
#include <time.h>

#include "clio.h"
#include "entity.h"

extern	int	logging;
extern	int	htmllog;

static	FILE	*logfp;

static	char	*months[] = {
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
		};


/*
	Return a logfile name. Names are based on the current date
	and time, in the format: YYYYMMDD-HHMMSS.txt
*/
static	const char	*logfile(void)
{
	time_t	now;
	const	char	*ext;
	static char	name[30];
	struct tm *t;

	if ((now = time(NULL)) == (time_t)-1) {
		return "unknown";
	}

	if ((t = localtime(&now)) == NULL) {
		return "unknown";
	}

	if (htmllog != 0) {
		ext = "html";
	} else {
		ext = "txt";
	}

	sprintf(name,"%02d-%02d%02d%02d.%s",t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,ext);

	return name;
}

static	char	*year(void)
{
	static	char	year[5];
	time_t	now;
	struct tm *t;

	if ((now = time(NULL)) == (time_t)-1) {
		return "unknown";
	}

	if ((t = localtime(&now)) == NULL) {
		return "unknown";
	}

	sprintf(year,"%04d",t->tm_year+1900);

	return  year;
}


static	char	*month(void)
{
	time_t	now;
	struct tm *t;

	if ((now = time(NULL)) == (time_t)-1) {
		return "unknown";
	}

	if ((t = localtime(&now)) == NULL) {
		return "unknown";
	}

	return months[t->tm_mon];
}


void	start_logging(const char *host, const char *port, const char *folder)
{
	char	*ptr;
	char	path[FILENAME_MAX];

	if (logging == 0) return;

	if ((ptr = home()) == NULL) {
		return;
	}
	if (strlen(ptr) > 200) {
		return;
	}

	/*- Create main log directory -*/
	sprintf(path,"%s/%s",ptr,CLIO_LOG);
	makedir(path);

	/*- Create subdirectory for host -*/
	strcat(path,"/");
	strcat(path,folder);
	makedir(path);

	/*- Create subdirectory for year -*/
	strcat(path,"/");
	strcat(path,year());
	makedir(path);

	/*- Create subdirectory for month -*/
	strcat(path,"/");
	strcat(path,month());
	makedir(path);


	/*- Create log file -*/
	strcat(path,"/");
	strcat(path,logfile());
	if ((logfp = fopen(path,"w")) == NULL) {
		logging = 0;
		return;
	}

	/*- Log file header -*/
	if (htmllog != 0) {
		html_header(logfp,host,port);
	} else {
		fprintf(logfp,"Logging by Clio MUD2 client from %s starting on %s.\n",host,date());
	}
}


void	stop_logging(const char *host, const char *port)
{
	if (logfp != NULL) {
		if (htmllog != 0) {
			html_footer(logfp,host,port);
		} else {
			fprintf(logfp,"\nLogging by Clio MUD2 client from %s finished on %s.\n",host,date());
		}
		(void)fclose(logfp);
	}
}


void	ldisplay(WIN *w, const char *s)
{
	int	i;
	unsigned char ch;

	assert(w != NULL);

	if (logging == 0 || w->logged == 0 || logfp == NULL) {
		return;
	}

	if (htmllog != 0) {
		for (i = 0 ; i < (int)strlen(s) ; i++) {
			ch = s[i];
			switch (ch) {
			case '\b':
				fseek(logfp,-1,SEEK_CUR);
				break;
			case '\n':
			case '\r':
				fseek(logfp,0,SEEK_END);
				fprintf(logfp,"%c",ch);
				break;
			default:
				fprintf(logfp,"%s",entity[(int)ch]);
				break;
			}
		}
	} else {
		for (i = 0 ; i < (int)strlen(s) ; i++) {
			switch (s[i]) {
			case '\b':
				fseek(logfp,-1,SEEK_CUR);
				break;
			case '\n':
			case '\r':
				fseek(logfp,0,SEEK_END);
				fprintf(logfp,"%c",s[i]);
				break;
			default:
				fprintf(logfp,"%c",s[i]);
				break;
			}
		}
	}
}

static	const	char	*htmlcolour[] = {
	"n",		/* Black */
	"r",		/* Red */
	"g",		/* Green */
	"y",		/* Yellow */
	"b",		/* Blue */
	"m",		/* Magenta */
	"c",		/* Cyan */
	"w",		/* White */

	"xn",		/* Lt Black */
	"xr",		/* Lt Red */
	"xg",		/* Lt Green */
	"xy",		/* Lt Yellow */
	"xb",		/* Lt Blue */
	"xm",		/* Lt Magenta */
	"xc",		/* Lt Cyan */
	"xw"		/* Lt White */
};

void	lcolour(WIN *w, const int f, const int b)
{
	assert(w != NULL);

	if (htmllog == 0 || logfp == NULL || w->logged == 0) {
		return;
	}

	fprintf(logfp,"</span><span class=\"%s%s\">",htmlcolour[f],htmlcolour[b]);
}


void	url_start(WIN *w,const char *s)
{
	if (htmllog == 0 || logfp == NULL || w->logged == 0) {
		return;
	}

	fprintf(logfp,"<a href=\"%s\">",s);
}


void	url_end(WIN *w)
{
	if (htmllog == 0 || logfp == NULL || w->logged == 0) {
		return;
	}

	fprintf(logfp,"</a>");
}
