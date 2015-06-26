# Clio MUD2 Client.
# Copyright (c) 2003 Ian Peattie.
# All rights reserved.
# 
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicence, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

VERSION=1.8
OS="SCO OpenServer 5"

LEX=	flex
LFLAGS=	-8 -I

CC=	gcc
CFLAGS= -ansi -pedantic -O2 -s -W -Wall \
        -DCLIO_UNIX \
        -DNDEBUG
#        -DCLIO_OSS \   # COMMENTED OUT FROM ORIGINAL

#-Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion \

# LIBS=	-lncurses -laudiofile -lm
# LIBS=	-lncurses -laudiofile -lm -lsocket
LIBS=	-lncurses -lm # CUT DOWN FROM ORIGINAL ABOVE

OBJS=	caption.o chomp.o csig.o date.o despace.o fkey.o history.o home.o html.o ini.o logging.o main.o makedir.o options.o playsound.o sclient.o sdream.o sound.o sscore.o sversion.o sweather.o tdo.o tdont.o telnet.o tinit.o tnaws.o tterm.o twill.o twont.o tx.o txfes.o usage.o wat.o wback.o wclose.o wcls.o wcolour.o wdisplay.o weol.o wflush.o wfore.o wheight.o wkey.o wline.o wopen.o wreverse.o wsave.o wstack.o wstream.o wwidth.o

clio:	$(OBJS)
	$(CC) $(CFLAGS) -o clio $(OBJS) $(LIBS)
# -mcs -d clio # COMMENTED OUT FROM ORIGINAL

lint:
	lint -Xa -p -I/usr/local/include *c | more

lclint:
	lclint -skipposixheaders +posixlib -I/usr/local/include *c | more

clean:
	rm -f clio
	rm -f core a.out mon.out
	rm -f $(OBJS)

spotless:
	rm -f telnet.c
	rm -f cscope.out
	rm -f clio.debug
	rm -f clio-?.?.tar.gz
	@$(MAKE) clean

tar:
	@$(MAKE) spotless
	tar cvf clio-${VERSION}.tar *
	-gzip -9v clio-${VERSION}.tar

dos:
	@$(MAKE) telnet.c
	cp *.c *.h /D/clio/
	cp telnet.c ../contrib/
	xtod ../CHANGES > /D/clio/changes.txt
