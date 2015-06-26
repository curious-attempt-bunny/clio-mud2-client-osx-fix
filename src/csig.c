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
	Currently we just ignore signals, but we should
	should probably do something more useful with 
	some of them, such as cleaning up the screen 
	and exiting gracefully.
*/

#include <signal.h>

#include "clio.h"

void	csig(void)
{
	/*
		ABRT, FPE, ILL, INT, SEGV, and TERM are all 
		defined by ANSI. QUIT is POSIX.
	*/
	(void)signal(SIGINT, SIG_IGN);	/* Keyboard interrupt */
	(void)signal(SIGTERM,SIG_IGN);	/* Terminate */
	/*signal(SIGABRT,SIG_IGN);*/	/* Abort */

	/*signal(SIGFPE, SIG_IGN);*/	/* Floating point */
	/*signal(SIGILL, SIG_IGN);*/	/* Illegal instruction */
	/*signal(SIGSEGV,SIG_IGN);*/	/* Segmentation violation */

	/* UNIX-specific signals */
#if defined CLIO_UNIX
	/*signal(SIGQUIT,SIG_IGN);*/
	/*signal(SIGWINCH,SIG_IGN);*/
	(void)signal(SIGCHLD,SIG_IGN);
#endif /* CLIO_UNIX */
}
