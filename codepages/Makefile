mkpage:	mkpage.l
	lex  mkpage.l
	mv lex.yy.c mkpage.c
	cc -O -o mkpage mkpage.c -ll
	rm mkpage.c

# Code Pages.

ascii:	ascii.src
	./mkpage < ascii.src > ascii

cp437:	cp437.src
	./mkpage < cp437.src > cp437

cp850:	cp850.src
	./mkpage < cp850.src > cp850

pages:
	make ascii cp437 cp850

clean:
	rm -f mkpage mkpage.c
	rm -f ascii cp437 cp850
