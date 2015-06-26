/*
	Copyright (c) 2003 Ian Peattie.
	All rights reserved.

	Display clio.debug dumps in a more readable format.
*/

#include <stdio.h>

int	main(void)
{
	int	ch;
	FILE	*fp;

	if ((fp = fopen("clio.debug","r")) != NULL) {
		while ((ch = getc(fp)) != EOF) {
			if (ch > 127) {
				if (ch == 255) {
					printf("[%d]",ch);
				} else {
					printf("[%d]",ch-155);
				}
			} else {
				printf("%c",(char)ch);
			}
		}

		fflush(stdout);
		fclose(fp);
	} else {
		printf("Can't open clio.debug.\n");
	}

	return 0;
}
