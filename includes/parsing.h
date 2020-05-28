#ifndef _PARSING_H_
# define _PARSING_H_
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <math.h>
# include <sys/types.h>

typedef struct	s_arg{
	int			c;
	char		**v;
}				t_arg;

#define SPACE 32
#define TAB	9
#define NEWLINE 10

#endif