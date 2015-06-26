# Clio MUD2 Client.
# Copyright (c) 2002 Ian Peattie.
# All rights reserved.

SHELL=	/bin/sh

TBL=	/usr/local/bin/tbl
EQN=	/usr/local/bin/eqn
NROFF=	/usr/local/bin/nroff
TROFF=	/usr/local/bin/troff

GROHTML=/usr/local/bin/grohtml

all:		clio.man clio.html clio.txt

clio.man:	clio.6
		$(TBL) clio.6 | $(EQN) -Tascii | $(NROFF) -man > clio.man

clio.html:	clio.6
		$(TBL) clio.6 | $(EQN) -Thtml | $(TROFF) -man -mhtml -Thtml | $(GROHTML) > clio.html

clio.txt:	clio.man
		sed -e 's/.//g' < clio.man > clio.txt

clean:
		rm -f clio.man clio.html clio.txt
