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

/*
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
*/
/*
// si bug : codes ASCII ' = 39. Code ASCII " = 34
	int
count_cmd(char *input)
{
	int	i;
	int	single_quote;
	int	double_quote;
	int	count;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (i < ft_strlen(input))
	{
		if (!(double_quote % 2) && input[i] == '\'')
			single_quote++;
		if (!(single_quote % 2) && input[i] == '"')
			double_quote++;
		if (!(single_quote % 2) && !(double_quote % 2) && input[i] == ';')
			count++;
		i++;
	}
	return (count);
}
*/

	static int
tokens_count(char *input)
{
	int	single_quote;
	int	double_quote;
	int	count;
	int	i;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	count = 0;
	if (!is_IFS(input[0]) && input[0] != '\0')
		count++;
	while (input[i])
	{
		if (!(double_quote % 2) && input[i] == 39)
			single_quote++;
		if (!(single_quote % 2) && input[i] == 34)
			double_quote++;
		if ((!(single_quote % 2) && !(double_quote % 2))
			&& ((is_IFS(input[i]) && input[i + 1] && !is_IFS(input[i + 1]))
			|| (input[i] != ';' && input[i - 1] && input[i - 1] == ';')))
			count++;
		i++;
	}
	return (count);
}

	int
token_len(char *input)
{
	int	len;
	int	single_quote;
	int	double_quote;

	len = 0;
	single_quote = 0;
	double_quote = 0;
	while (input[len] && input[len] == ';')
		len++;
	while (input[len] && input[0] != ';'
		&& (!is_IFS(input[len]) || (single_quote % 2) || (double_quote % 2)))
	{
		if (!(double_quote % 2) && input[len] == 39)
			single_quote++;
		if (!(single_quote % 2) && input[len] == 34)
			double_quote++;
		if ((!(single_quote % 2) || !(double_quote % 2)) && input[len] == ';')
			break;
		len++;
	}
	return (len);
}

	static void
fill_tokens(char **tokens, char *input)
{
	int i;
	int j;
	int	k;
	int len;

	i = 0;
	j = 0;
	while (input[i])
	{
		if (!is_IFS(input[i]))
		{
			len = token_len(&input[i]);
			tokens[j] = (char*)malloc(sizeof(char) * (len + 1));
			k = 0;
			while (input[i] && k < len)
				tokens[j][k++] = input[i++];
			tokens[j++][len] = '\0';
		}
		else
			i++;
	}
}

	char**			
split_input(char *input)
{
	char	**tokens;
	int		count;

	if (!input)
		return (NULL);
	count = tokens_count(input);
	if (!(tokens = (char **)malloc(sizeof(char*) * (count + 1))))
		return (NULL);
	tokens[count] = NULL;
	fill_tokens(tokens, input);
	return (tokens);
}

	int
cmd_count(char **tokens)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (tokens[0] && ft_strcmp(tokens[0], ";"))
		count++;
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], ";") 
			&& tokens[i + 1] && ft_strcmp(tokens[i + 1], ";"))
			count++;
		i++;
	}
	return (count);
}
	
	t_cmd*
cmd_list(char **tokens)
{
	t_cmd	*list = NULL;
	int		i;

	if (!tokens)
		return (NULL);
	if (!(list = malloc(sizeof(t_cmd))))
		return (NULL);
	list->argc = 0;
	while (tokens[list->argc] && ft_strcmp(tokens[list->argc], ";"))
			list->argc++;
	list->next = NULL;
	if (!(list->argv = (char **)malloc(sizeof(char*) * (list->argc + 1))))
		return (NULL);
	list->argv[list->argc] = NULL;
	i = 0;
	while (i < list->argc)
	{
		list->argv[i] = ft_strdup(tokens[i]);
		i++;
	}
	while (tokens[i] && !ft_strcmp(tokens[i], ";"))
		i++;
	if (tokens[i])
		list->next = cmd_list(&tokens[i]);
	return (list);
}
	
	void
check_tokens(char **tokens)
{
	(void)tokens;// A COMPLETER AVEC LES CAS D'ERREUR
}
	
	t_cmd*
parse_input(char *input)
{
	t_cmd		*list;
	char		**tokens;
	int			i;

	tokens = split_input(input);
	check_tokens(tokens);
	i = 0;
	while (tokens[i] && !ft_strcmp(tokens[i], ";"))
		i++;
	list = NULL; 
	list = cmd_list(&tokens[i]);
	return (list);
}