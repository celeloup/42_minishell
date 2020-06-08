/*******************
//HEADER
****/

#include "../includes/minishell.h"

//IFS = Internal Field Separator

/*
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
		if (!(double_quote % 2) && input[i] == SINGLE_QUOTE)
			single_quote++;
		if (!(single_quote % 2) && input[i] == DOUBLE_QUOTE)
			double_quote++;
		if ((!(single_quote % 2) && !(double_quote % 2))// a changer
			&& (is_IFS(input[i]) && input[i + 1] && !is_IFS(input[i + 1])))
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
		if (!(double_quote % 2) && input[len] == SINGLE_QUOTE)
			single_quote++;
		if (!(single_quote % 2) && input[len] == DOUBLE_QUOTE)
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
		list->argv[i] = ft_strdirup(tokens[i]);
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

	char**
deal_quotes(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{

		i++;
	}
	return (tokens);
}
*/

	char*
get_env_var(char *var, char *env[])
{
	size_t	i;
	i = 0;
	while (env[i])
	{
		if (!strncmp(env[i], var + 1, ft_strlen(var + 1)))
			return (ft_strdup(ft_strrchr(env[i], '=') + 1));
		i++;
	}
	return (NULL);//vérifier s'il ne renvoie pas plutôt une chaine vide
}

	int
var_len(char *input, char *env[], int name)
{
	int		len;
	char	*var_name = NULL;
	char	*var = NULL;

	len = 1;
	if (input[len] && input[len] == '?')
		len++;
	else if (input[len])
		while (input[len] && ft_isalnum(input[len]))// a confirmer
			len++;
	if (name)
		return (len);
	var_name = (char *)malloc(sizeof(char) * (len + 1));
	var_name[len] = '\0';
	var = get_env_var(var, env);
	len = ft_strlen(var);
	if (len == -1)
		len = 1;
	free(var_name);
	if (var)
		free(var);
	var_name = NULL;
	var = NULL;
	return (len);
}

	int
single_quote_len(char *input, int clean)
{
	int len;
	int	clean_len;

	clean_len = 0;
	len = 1;
	while (input[len] && input[len] != SINGLE_QUOTE)
		len++;
	if (input[len] && input[len] == SINGLE_QUOTE)
	{
		clean_len -= 2;
		len++;
	}
	if (clean)
		return (clean_len + len);
	return (len);
}

	int
double_quote_len(char *input, char *env[], int clean)
{
	int len;
	int	clean_len;

	clean_len = 0;
	len = 1;
	while (input[len] && input[len] != DOUBLE_QUOTE)
	{
		if (input[len] == BKSLASH && input[len + 1] 
			&& (input[len + 1] == DOLLAR || input[len + 1] == DOUBLE_QUOTE 
			|| input[len + 1] == BKSLASH))
			{
				len += 2;
				clean_len--;
			}
		else if (input[len] == DOLLAR)
		{
			len += var_len(&input[len], env, 1);
			clean_len -= var_len(&input[len], env, 1);
			clean_len += var_len(&input[len], env, 0);
		}
		else
			len++;
	}
	if (input[len] && input[len] == DOUBLE_QUOTE)
	{
		clean_len -= 2;
		len++;
	}
	if (clean)
		return (clean_len + len);
	return (len);
}

	int
quote_len(char *input, char *env[], int clean)
{
	int	len;

	len = 0;
	if (input[0] == DOUBLE_QUOTE)
		len = double_quote_len(input, env, clean);
	if (input[0] == SINGLE_QUOTE)
		len = single_quote_len(input, clean);
	return(len);
}

	int
token_len(char *input, char *env[], int clean)
{
	int i;
	int clean_len;

	i = 0;
	clean_len = 0;
	while (input[i] && !(IFS(input[i]) || RDIR(input[i]) || CMD_SEP(input[i])))
	{
		if (input[i] == BKSLASH && input[i + 1])
		{
			clean_len--;
			if (input[i + 1] == NEWLINE)//multiligne hors sujet ?
				clean_len--;
			i += 2;
		}
		else if (QUOTE(input[i]))
		{
			clean_len -= (quote_len(&input[i], env, 0));
			clean_len += (quote_len(&input[i], env, 1));
			i += quote_len(&input[i], env, 0);
		}
		else if (input[i] == DOLLAR)
		{
			clean_len -= var_len(&input[i], env, 1);
			clean_len += var_len(&input[i], env, 0);
			i += var_len(&input[i], env, 1);
		}
		else
			i++;
	}
	return (i);
}

	int
get_rdir_type(t_rdir *rdir, char *input)
{
	int	i;
	int rdir_type;

	i = 0;
	if (input[0] == '<')
		rdir_type = RDIR_IN;
	else if (input[0] == '>' && input[1] && input[1] == '>')
		rdir_type = APP_RDIR_OUT;
	else
		rdir_type = RDIR_OUT;
	if (rdir_type == RDIR_IN || rdir_type == RDIR_OUT)
		i++;
	else
		i += 2;
	while (input[i] && IFS(input[i]))
		i++;
	if (rdir)
		rdir->type = rdir_type;
	return (i);
}

	int
get_rdir_value(t_rdir *rdir, char *input, char *env[])
{
	int	len;
	int	i;
	int	j;

	i = token_len(input, env, 0);
	len = token_len(input, env, 1);
	// cas d'erreur si len = 0 ?
	if (rdir)
	{
		if (!(rdir->value = (char *)malloc(sizeof(char) * (len + 1))))
				return (0);
		while (i < len)
		{
			rdir->value[i] = input[i];
			i++;
		}
		rdir->value[len] = '\0';
	}
	return (i);
}

	t_rdir*
copy_rdir(t_rdir *src)
{
	t_rdir	*dest;

	dest = NULL;
	if (!src)
		return (NULL);
	dest = init_rdir();
	dest->type = src->type;
	dest->value = ft_strdup(src->value);
	if (src->next)
		dest->next = copy_rdir(src->next);
	return (dest);
}
	
	int
get_cmd_rdir(t_rdir **rdir, char *input, char *env[])
{
	int		i;
	
	if (*rdir)
	{
		while (*rdir)
			rdir = &(*rdir)->next;
	}
	i = 0;
	*rdir = init_rdir();
	i += get_rdir_type(*rdir, input);
	i += get_rdir_value(*rdir, &input[i], env);
	return (i);
}

	int
cmd_len(t_cmd *cmd, char *input, char *env[])
{
	int	i;

	i = 0;
	while (input[i] && !(input[i] == ';' || input[i] == '|'))
	{	
		//ft_printf("\n%d", (int)input[i]);
		if (token_len(&input[i], 0))
		{
			cmd->argc++;
			i += token_len(&input[i], 0);
		}
		else if (RDIR(input[i]))
			i += get_cmd_rdir(&cmd->rdir, &input[i], env);
		else
			i++;
	}
	return (i);
}

	char*
clean_token(char *input, int len, char *env[])
{
	char	*token;
	int		i;

	i = 0;
	token = (char *)malloc(sizeof(char) * (token_len(input, 1) + 1));//secu
	token[token_len(input, 1)] = '\0';
	while(i < len)
	{
		i++;
	}
	/*token = malloc(sizeof(char *) * (len + 1));
	token = ft_strncpy(token, &input[i], len);
	token[len] = '\0';	*/
	return (token);
}

	void
get_cmd_argv(t_cmd *cmd, char *input, int cmd_len, char *env[])
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = 0;
	while (input[i] && i < cmd_len)
	{
		if ((len = token_len(&input[i], env, 0)))
		{
			cmd->argv[j] = clean_token(&input[i], len, env);
			i += len;
			j++;
		}
		else if (RDIR(input[i]))
		{
			i += get_rdir_type(NULL, &input[i]);
			i += token_len(&input[i], env, 0);
		}
		else
			i++;
	}
}

	t_cmd*
parse_input(char *input, char *env[])
{
	t_cmd	*cmd = NULL;
	int		len;
	
	cmd = init_cmd(input);
	if (!cmd)
		return (NULL);//remplacer par un cas d'erreur ? 
	len = cmd_len(cmd, input);
	if (!input || (input && !(cmd->argv = malloc(sizeof(char **) * cmd->argc))))
        return (NULL);//idem //pourquoi size of char ** et pas char * ?
	get_cmd_argv(cmd, input, len, env);
	if (input[len] && input[len] == '|')
		cmd->pipe++;
	if (input[len] && (input[len] == ';' || input[len] == '|'))
		len++;
	if (input[len])//gérer l'erreur 2 ';' a suivre
		cmd->next = parse_input(&input[len], env);
	return (cmd);
}