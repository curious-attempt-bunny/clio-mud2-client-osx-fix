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

#include <stdio.h>
#include <stdlib.h>

#include "clio.h"


#define	CLIOWEB		"http://www.wabe.org.uk/clio/"
#define	CLIOMAIL	"clio@wabe.org.uk"
#define	DOCTYPE		"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">"


static	const	struct	css_definition {
	const	char	*col;
	const	char	*c;
} css[] = {
	{"#000000",	"n"},   /* Black */
	{"#C00000",	"r"},	/* Red (Maroon) */
	{"#00C000",	"g"},	/* Green */
	{"#C0C000",	"y"},	/* Brown (Olive) */
	{"#0000C0",	"b"},	/* Blue (Navy) */
	{"#C000C0",	"m"},	/* Magenta (Purple) */
	{"#00C0C0",	"c"},	/* Cyan (Teal) */
	{"#C0C0C0",	"w"},	/* White (Silver) */

	{"#808080",	"xn"},	/* Bright black */
	{"#FF0000",	"xr"},	/* Bright red */
	{"#00FF00",	"xg"},	/* Bright green */
	{"#FFFF00",	"xy"},	/* Bright brown */
	{"#0000FF",	"xb"},	/* Bright blue */
	{"#FF00FF",	"xm"},	/* Bright magenta */
	{"#00FFFF",	"xc"},	/* Bright cyan */
	{"#FFFFFF",	"xw"}	/* Bright white */
} ;


static	void	create_css(FILE *fp)
{
	int	f,b;

	fprintf(fp,"<style type=\"text/css\">\n");
	fprintf(fp,"<!--\n");

	fprintf(fp," :link {\n");
	fprintf(fp,"   background: black !important;\n");
	fprintf(fp,"   color: yellow;\n");
	fprintf(fp," }\n");
	fprintf(fp," :visited {\n");
	fprintf(fp,"   background: black !important;\n");
	fprintf(fp,"   color: #C0C000;\n");
	fprintf(fp," }\n");
	fprintf(fp," :active {\n");
	fprintf(fp,"   background: black !important;\n");
	fprintf(fp,"   color: yellow;\n");
	fprintf(fp," }\n");
	fprintf(fp," :hover {\n");
	fprintf(fp,"   background: black !important;\n");
	fprintf(fp,"   color: #FFFF80;\n");
	fprintf(fp," }\n");

	fprintf(fp," body {\n");
	fprintf(fp,"   background: black !important;\n");
	fprintf(fp,"   color: white !important;\n");
	fprintf(fp,"   font-family: Garamond, serif;\n");
	fprintf(fp,"   font-style: normal;\n");
	fprintf(fp,"   font-weight: normal;\n");
	fprintf(fp," }\n");

	fprintf(fp," h1 {\n");
	fprintf(fp,"   background: black !important;\n");
	fprintf(fp,"   color: yellow !important;\n");
	fprintf(fp,"   font-family: Verdana, sans-serif;\n");
	fprintf(fp," }\n");

	fprintf(fp," pre {\n");
	fprintf(fp,"   background: black !important;\n");
	fprintf(fp,"   color: white !important;\n");
	fprintf(fp,"   font-family: monospace !important;\n");
	fprintf(fp," }\n");

	for (b = 0; b <= 7; b++) {
		for (f = 0; f <= 15; f++) {
			fprintf(fp," span.%s%s {\n",css[f].c,css[b].c);
			fprintf(fp,"   background: %s !important;\n",css[b].col);
			fprintf(fp,"   color: %s !important;\n",css[f].col);
			fprintf(fp," }\n");
		}
	}

	fprintf(fp,"-->\n");
	fprintf(fp,"</style>\n");
}


void	html_header(FILE *fp, const char *host, const char *port)
{
	fprintf(fp,"%s\n",DOCTYPE);

	fprintf(fp,"<html>\n");
	fprintf(fp,"<head>\n");

	fprintf(fp,"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\">\n");
	fprintf(fp,"<meta name=\"generator\" content=\"Clio MUD2 client %s\">\n",CLIO_VERSION);
	fprintf(fp,"<meta name=\"comment\" content=\"This MUD log file was produced by Clio MUD2 client version %s\">\n",CLIO_VERSION);

	fprintf(fp,"<title>Clio MUD2 Client | %s | %s</title>\n",host,date());
	create_css(fp);
	fprintf(fp,"</head>\n");

	fprintf(fp,"<body>\n");
	fprintf(fp,"<h1>MUD Log File</h1>\n");
	fprintf(fp,"<p>Logging by <a href=\"%s\" title=\"Visit the Clio web site\">Clio MUD2 client</a> from <a href=\"telnet://%s:%s/\" title=\"Telnet to %s\">%s</a> started on %s.</p>\n",CLIOWEB,host,port,host,host,date());
	fprintf(fp,"<hr>\n");
	fprintf(fp,"<pre>\n");
	fprintf(fp,"<span class=\"wn\">");
}


void	html_footer(FILE *fp, const char *host, const char *port)
{
	fprintf(fp,"</span>\n");
	fprintf(fp,"</pre>\n");
	fprintf(fp,"<hr>\n");

	fprintf(fp,"<p>Logging by <a href=\"%s\" title=\"Visit the Clio web site\">Clio MUD2 client</a> from <a href=\"telnet://%s:%s/\" title=\"Telnet to %s\">%s</a> finished on %s.</p>\n",CLIOWEB,host,port,host,host,date());

	fprintf(fp,"<p>");
	fprintf(fp,"Clio is copyright &copy; 2003 <a href=\"mailto:%s\" title=\"Email the author of Clio\">Ian Peattie</a>. All rights reserved.<br>\n",CLIOMAIL);
	if (ismud2()) {
		fprintf(fp,"MUD2 is copyright &copy; <a href=\"http://www.mud.co.uk/muse/\" title=\"Visit the MUSE Ltd web site\">Multi-User Entertainment Ltd</a>.\n");
	}
	fprintf(fp,"</p>");

	fprintf(fp,"</body>\n");
	fprintf(fp,"</html>\n");
}
