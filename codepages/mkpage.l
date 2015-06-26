%{
#include <stdlib.h>
#include <limits.h>

int	c1, c2;
unsigned char	map[256];
%}

CODE	([A-Fa-f0-9]{2})
COMMENT	("#".*\n)
SPACE	([ \t]*)

%%
{COMMENT}	|
{SPACE}		|
{SPACE}\n	;	/* Skip comments and empty lines */

{CODE}:{CODE}	{
			c1 = strtol(yytext,NULL,16);
			c2 = strtol(yytext+3,NULL,16);
			if (map[c1] != 0) {
				printf("Duplicate mapping for %x.\n",c1);
			}
			map[c1] = c2;
		}

%%

int	yywrap(void)
{
	int	i;

	for (i = 0; i <= 255; i++) {
		if (map[i] == 0) {
			printf("%c",i);
		} else {
			printf("%c",map[i]);
		}
	}
	fflush(stdout);
	return 1;
}
