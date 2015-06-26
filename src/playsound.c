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
	Play a wav file via OSS.
*/

#include "clio.h"

#if defined CLIO_OSS

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

#include <audiofile.h>

#define	DEVICE		"/dev/dsp"
#define	FRAMES		4096

	/* Some versions of audiofile.h don't have this prototype */
int	afSetVirtualRate(AFfilehandle, int track, double rate);


static	int dspopen(const char *device)
{
	int	fd;
	int	requested;
	int	actual;

	if ((fd = open(device,O_WRONLY)) == -1) {
		return -1;
	}

	/*- Format -*/
	requested = actual = AFMT_U8;
	if (ioctl(fd,SNDCTL_DSP_SETFMT,&requested) == -1) {
		(void)close(fd);
		return -1;
	}
	if (requested != actual) {
		(void)close(fd);
		return -1;
	}

	/*- Channels -*/
	requested = actual = 2;
	if (ioctl(fd,SNDCTL_DSP_CHANNELS,&requested) == -1) {
		(void)close(fd);
		return -1;
	}
	if (requested != actual) {
		(void)close(fd);
		return -1;
	}

	/*- Sample rate -*/
	requested = actual = 22050;
	if (ioctl(fd,SNDCTL_DSP_SPEED,&requested) == -1) {
		(void)close(fd);
		return -1;
	}
	if (requested != actual) {
		(void)close(fd);
		return -1;
	}

	return fd;
}


void	playsound(const char *wavfile)
{
	AFfilehandle	afh;	/* Handle for WAV file */
	size_t	nr;
	int	fd = -1;	/* Descriptor for /dev/dsp */
	char	*buffer;	/* Playback buffer */
/* should buffer be unsigned char? */
	float	framesize;	/* */


	afSetErrorHandler(NULL);

	/* Play Sound Asynchronously */
	if (fork() != 0) {
		/* Now in parent... */
		return;
	}

	/*- Open Sound Output Device -*/
	if ((fd = dspopen(DEVICE)) < 0) {
		_exit(EXIT_SUCCESS);
	}

	/*- Open Sound Input File -*/
	afh = afOpenFile(wavfile,"r",NULL);
	if (afh == AF_NULL_FILEHANDLE) {
		(void)close(fd);
		_exit(EXIT_SUCCESS);
	}


	/*- Set Virtual Format -*/
	afSetVirtualSampleFormat(afh,AF_DEFAULT_TRACK,AF_SAMPFMT_UNSIGNED,8);
	afSetVirtualChannels(afh,AF_DEFAULT_TRACK,2);
	afSetVirtualRate(afh,AF_DEFAULT_TRACK,22050.0);


	/*- Create Playback Buffer -*/
	framesize = afGetVirtualFrameSize(afh,AF_DEFAULT_TRACK,1);
	if ((buffer = malloc((size_t)(FRAMES * framesize))) == NULL) {
		afCloseFile(afh);
		(void)close(fd);
		_exit(EXIT_SUCCESS);
	}

/* calloc buffer?*/
memset(buffer,0,(size_t)(FRAMES*framesize));

	/*- Play it! -*/
	nr = afReadFrames(afh,AF_DEFAULT_TRACK,buffer,FRAMES);
	while (nr > (size_t)0)
	{
		write(fd,buffer,(size_t)(nr * framesize));
/*might we fail to write everything? */
		nr = afReadFrames(afh,AF_DEFAULT_TRACK,buffer,FRAMES);
	}
/*flush fd here?*/

	/*- Tidy Up -*/
	free(buffer);
	(void)close(fd);
	afCloseFile(afh);

	_exit(EXIT_SUCCESS);
}

#endif /* CLIO_OSS */
