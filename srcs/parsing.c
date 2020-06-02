/*******************
//HEADER
****/

#include "../includes/minishell.h"

//IFS = Internal Field Separator

	int
is_IFS(char c)
{
	if (c == SPACE || c == TAB || c == NEWLINE)
		return (1);
	else
		return (0);
}

	void
arg_count(char *input, t_arg *arg)
{
	size_t	i;
	int		size;

	i = 0;
	arg->c = 0;//normalement pas besoin car déjà dans free_arg
	arg->max_arg_size = 0;//idem
	while (input[i] && i < ft_strlen(input))
	{
		if (is_IFS(input[i]) == 0)
		{
			arg->c++;
			size = 0;
			while (input[i + size] && is_IFS(input[i + size]) == 0)
				size++;
			i = i + size;
			if (size > arg->max_arg_size)
				arg->max_arg_size = size;
		}
		while (input[i] && is_IFS(input[i]))
			i++;
	}
}
	
	char *
get_arg()
{
	
}
	
	void
arg_values(char *input, t_arg *arg)
{
	int	i;
	int	j;
	int	count;

	if (!(arg->v = malloc(sizeof(char**) * arg->c)))
		ft_printf("\nFailed allocating memory for arg_v");// sans doute a changer par une fonction globale de sortie propre
	count = 0;
	i = 0;
	while (input[i])
	{
		j = 0;
		while (input[i] && is_IFS(input[i]))
			i++;
		if (input[i] && is_IFS(input[i]) == 0)
		{
			if (!(arg->v[count] = malloc(sizeof(char*) * arg->max_arg_size)))
				ft_printf("\nFailed allocating memory for arg_v[%d]", count);//idem : changer
			while (input[i] && is_IFS(input[i]) == 0)
			{
				arg->v[count][j] = input[i];
				i++;
				j++;
			}
			while (j < arg->max_arg_size)
			{
				arg->v[count][j] = '\0';
				j++;
			}
			count++;
		}
		i++;
	}
}

	void
parse(char *input, t_arg *arg)
{
	free_arg(arg);
	arg_count(input, arg);
	arg_values(input, arg);
}