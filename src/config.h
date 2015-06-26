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

#ifndef CONFIG_H
#define CONFIG_H

/*
CLIO_UNIX	OS is Unix or Unix-like. (Ie, POSIX).
CLIO_WINDOWS	OS Windows.

CLIO_OS		A description of the operating system.
CLIO_OPTCHAR	The character used to delimit command line options.
CLIO_INI	The location of the Clio initialisation file.
CLIO_LOG	The location of the Clio log file directory.
CLIO_SOUNDS	The location of the Clio sound effects.
CLIO_OSS	Use OSS to play sounds.
CLIO_PLAYSOUND	Use Windows PlaySound() function to play sounds.
*/


#if !defined CLIO_UNIX
#define CLIO_WINDOWS
#endif


/*********************************************************************/
/* Clio for Windows                                                 */
/*********************************************************************/
#if defined CLIO_WINDOWS

#define CLIO_OS "Windows"
#define CLIO_INI "clio.ini"
#define CLIO_LOG "Logs"
#define	CLIO_MAPS "Maps"
#define	CLIO_SOUNDS "Sounds"
#define CLIO_PLAYSOUND
#define	CLIO_OPTCHAR "/"

#endif /* CLIO_WINDOWS */


/*********************************************************************/
/* Clio for UNIX                                                     */
/*********************************************************************/
#if defined CLIO_UNIX

#if !defined CLIO_OS
#define CLIO_OS "UNIX"
#endif /* !CLIO_OS */

#define	CLIO_MAPS "/usr/local/lib/clio/maps"

#define	CLIO_SOUNDS "/usr/local/lib/clio/sounds"

#define CLIO_INI ".cliorc"
#define CLIO_LOG "cliologs"

#define	CLIO_OPTCHAR "-"

#endif /* CLIO_UNIX */

#endif /* CONFIG_H */
