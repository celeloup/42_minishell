/*******************
//HEADER
****/

#include "parsing.h"
#include "../cub3d/Bonuses/libft/libft.h"

//IFS = Internal Field Separator

	int
is_IFS(char c)
{
	if (c == SPACE || c == TAB || c == NEWLINE)
		return (1);
	else
		return (0);
}

	int
arg_count(char *input)
{
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	while (i < ft_strlen(input))
	{
		if (is_IFS(input[i]) == 0)
		{
			ret++;
			while (is_IFS(input[i]) == 0)
				i++;
		}
		while (is_IFS(input[i]))
			i++;
	}
	return (ret);
}
	
	t_arg
parse(char *input)
{
	t_arg	arg;
	int		i;
	
	arg.c = 0;
	arg.c = arg_count(input);
	i = 0;

	return (arg);
}
	
	int
main(int argc, char**argv)
{
	t_arg	arg;

	(void)argc;
	arg = parse(argv[1]);
	
	return(0);
}
