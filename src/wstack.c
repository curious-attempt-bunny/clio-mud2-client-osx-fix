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
	Routines to maintain the colour stacks.

	void	push(const int, const int) ;
	void	pop(void) ;
	void	throw(void) ;
	void	catch(void) ;

	PUSH:	Place a new colour on top of the current colour
		stack, and make it the current colour.

	POP:	Pop a colour from the current colour stack, and 
		make it the current colour.

	CATCH:	Save the current colour stack, and start a new stack.
		No change of colour is made.

	THROW:	Restore the context stack saved by a previous throw().
		The current colour is set to that colour on the top
		of the new stack, but the colour is not pop()'d .


	These routines were originally part of m2client.
*/

#include "clio.h"
#include "window.h"

#define	MAX_CONTEXT	256
#define	MAX_STACKS	256

extern	WIN *mwin;

static	struct	context_stack {
	int	f[MAX_CONTEXT] ;
	int	b[MAX_CONTEXT] ;
	int	idx ;
} context[MAX_STACKS] ;

static	int	stack = 0 ;


void	push(const int f, const int b)
{
	int	idx ;

	if (context[stack].idx < MAX_CONTEXT - 1) {
		idx = ++context[stack].idx ;
		context[stack].f[idx] = f ;
		context[stack].b[idx] = b ;
		wcolour(mwin,f,b) ;
	}
}


int	pop(void)
{
	int	idx ;
	int	popped ;

	if (context[stack].idx > 0) {

		idx = context[stack].idx ;
		popped = context[stack].f[idx] ;

		--context[stack].idx ;
		idx = context[stack].idx ;
		if (context[stack].f[idx] >= 0) {
			wcolour(mwin,context[stack].f[idx], context[stack].b[idx]) ;
		}

		return popped ;
	}

	return -1;
}


void	catch(void)
{
	int	idx ;

	if (stack < MAX_STACKS - 1) {
		++stack ;
		context[stack].idx = 0 ;

		idx = context[stack-1].idx ;
		context[stack].f[0] = context[stack-1].f[idx] ;
		context[stack].b[0] = context[stack-1].b[idx] ;
	}
}


void	throw(void)
{
	int	idx ;

	if (stack > 0) {
		context[stack].idx = 0 ;
		--stack ;
		idx = context[stack].idx ;
		if (context[stack].f[idx] >= 0) {
			wcolour(mwin,context[stack].f[idx], context[stack].b[idx]) ;
		}
	}
}


void	init_stack(const int f, const int b)
{
	stack = 0 ;
	context[stack].idx = 0 ;
	context[stack].f[0] = f;
	context[stack].b[0] = b;
}
