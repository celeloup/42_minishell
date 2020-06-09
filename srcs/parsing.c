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
backslash_len(char *input, int quote, int expanded)
{
	if (input[0] != BKSLASH)
		return (0);
	if (!input[1])
		return (1);
	else if (input[1] == NEWLINE && expanded)
		return (0);
	else if (!expanded || (quote == DOUBLE_QUOTE && !(input[1] == DOLLAR 
		|| input[1] == DOUBLE_QUOTE || input[1] == BKSLASH)))
		return (2);
	else
		return (1);
}
	
	int
single_quote_len(char *input, int expanded)
{
	int len;
	int	exp_len;

	exp_len = 0;
	len = 1;
	while (input[len] && input[len] != SINGLE_QUOTE)
		len++;
	if (input[len] && input[len] == SINGLE_QUOTE)
	{
		exp_len -= 2;
		len++;
	}
	if (expanded)
		return (exp_len + len);
	return (len);
}
	
	int
double_quote_len(char *input, char *env[], int expanded)
{
	int len;
	int	exp_len;

	exp_len = 0;
	len = 1;
	while (input[len] && input[len] != DOUBLE_QUOTE)
	{
		exp_len -= len_after_char(&input[len], env, DOUBLE_QUOTE, NOT_EXP);
		exp_len += len_after_char(&input[len], env, DOUBLE_QUOTE, EXP);
		len += len_after_char(&input[len], env, DOUBLE_QUOTE, NOT_EXP);
	}
	if (input[len] && input[len] == DOUBLE_QUOTE)
	{
		exp_len -= 2;
		len++;
	}
	if (expanded)
		return (exp_len + len);
	return (len);
}

	int
quote_len(char *input, char *env[], int expanded)
{
	if (input[0] == DOUBLE_QUOTE)
		return (double_quote_len(input, env, expanded));
	else
		return (single_quote_len(input, expanded));
}
	
	int
var_len(char *input, char *env[], int expanded)//dollar inclus
{
	int		len;
	char	*var_name = NULL;
	char	*var_value = NULL;

	len = 1;
	if (input[len] && input[len] == '?')
		len++;
	else if (input[len])
		while (input[len] && ft_isalnum(input[len]))// a confirmer
			len++;
	if (!expanded)
		return (len);
	var_name = (char *)malloc(sizeof(char) * (len + 1));
	var_name[len] = '\0';
	var_value = get_env_var(var_name, env);
	len = ft_strlen(var_value);
	if (len == -1)
		len = 1;
	free(var_name);
	if (var_value)
		free(var_value);
	var_name = NULL;
	var_value = NULL;
	return (len);
}
	
	int
len_after_char(char *input, char *env[], int quote, int expanded)
{
	if (!input)
		return(0);
	else if (QUOTE(input[0]) && !quote)
		return (quote_len(input, env, expanded));
	else if (input[0] == BKSLASH)
		return (backslash_len(input, quote, expanded));
	else if (input[0] == DOLLAR)
		return (var_len(input, env, expanded));
	else 
		return (1);
}
		
	int
token_len(char *input, char *env[], int expanded)
{
	int len;
	int exp_len;

	len = 0;
	exp_len = 0;
	while (input[len] && !(ARG_SEP(input[len])))
	{
		exp_len -= len_after_char(&input[len], env, NO_QUOTE, NOT_EXP);
		exp_len += len_after_char(&input[len], env, NO_QUOTE, EXP);
		len += len_after_char(&input[len], env, NO_QUOTE, NOT_EXP);
	}
	if (expanded)
		return (len + exp_len);
	return (len);
}

	char*
get_var_name(char *input)
{
	int		i;
	int		len;
	char	*name;

	len = var_len(input, NULL, NOT_EXP);//gérer le cas rien apres dollar ? 
	name = NULL;
	name = (char *)malloc(sizeof(char) * (len + 1));
	name[0] = DOLLAR;
	name[len] = '\0';
	i = 1;
	while (input[i] && ft_isalnum(input[i]))
	{
		name[i] = input[i];
		i++;
	}
	return (name);
}

	char*
get_escaped_char(char *input, int quote)
{
	char	*ret = NULL;
	int		len;
	
	if (!(len = backslash_len(input, quote, EXP)))
		return (NULL);
	if (input[1] && input[1] == NEWLINE)
		return (NULL);
	ret = (char*)malloc(sizeof(char) * (len + 1));
	ret[len] = '\0';
	if (!input[1])
		ret[0] = BKSLASH;
	else if (quote == DOUBLE_QUOTE && !(input[1] == DOLLAR
		|| input[1] == DOUBLE_QUOTE || input[1] == BKSLASH))
	{
		ret[0] = BKSLASH;
		ret[1] = input[1];
	}
	else
		ret[0] = input[1];
	return (ret);
}

	char*
get_single_quote(char *input)
{
	char	*ret;
	int		len;

	len = single_quote_len(input, EXP);
	ret = ft_substr(input, 1, len);
	return (ret);
}

	char*
get_double_quote(char *input, char *env[])
{
	char	*ret;
	char	*str;
	int		len;
	int		i;
	int		j;

	len = double_quote_len(input, env, EXP);
	ret = (char *)malloc(sizeof(char) * (len + 1));
	ret[len] = '\0';
	i = 1;
	j = 0;
	while(input[i] && i < double_quote_len(input, env, NOT_EXP) && j < len)
	{
		if ((str = expanded_str(&input[i], env, DOUBLE_QUOTE)))
		{
			ft_strcpy(&ret[j], str);
			free(str);
		}
		str = NULL;
		j += len_after_char(&input[i], env, DOUBLE_QUOTE, EXP);
		i += len_after_char(&input[i], env, DOUBLE_QUOTE, NOT_EXP);
	}
	return (ret);
}

	char*
get_quote(char *input, char *env[])
{
	if (input[0] == SINGLE_QUOTE)
		return (get_single_quote(input));
	else
		return (get_double_quote(input, env));
}
	
	char*
expanded_str(char *input, char *env[], int quote)
{
	if (!input)
		return (NULL);//vérfier ?
	else if (input[0] == BKSLASH)
		return (get_escaped_char(input, quote));
	else if (QUOTE(input[0]) && !quote)
		return (get_quote(input, env));
	else if (input[0] == DOLLAR)
		return (get_env_var(get_var_name(input), env));
	else
		return (ft_substr(input, 0, 1));
}
		
	char*
get_token(char *input, char *env[])
{
	char	*token = NULL;
	char	*str = NULL;
	int		i;
	int		j;

	i = 0;
	j = 0;
	token = (char *)malloc(sizeof(char) * (token_len(input, env, EXP) + 1));
	//secu ?
	token[token_len(input, env, EXP)] = '\0';
	while(i < token_len(input, env, NOT_EXP) && j < token_len(input, env, EXP))
	{
		str = expanded_str(&input[i], env, NO_QUOTE);
		if (str)
		{
			ft_strcpy(&token[j], str);
			free(str);
		}
		str = NULL;
		j += len_after_char(&input[i], env, NO_QUOTE, EXP);
		i += len_after_char(&input[i], env, NO_QUOTE, NOT_EXP);
	}
	return (token);
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
	// cas d'erreur si token_len = 0 ?
	if (rdir)
		rdir->value = get_token(input, env);
	return (token_len(input, env, NOT_EXP));
}

/*	t_rdir*
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
}*/
	
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

	void
get_cmd_argv(t_cmd *cmd, char *input, char *env[], int cmd_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i] && i < cmd_len)
	{
		if (token_len(&input[i], env, NOT_EXP))
		{
			if (j == cmd->argc)
				ft_printf("\nWRONG ARGC OR PARSING ERROR");
			cmd->argv[j] = get_token(&input[i], env);
			i += token_len(&input[i], env, NOT_EXP);
			j++;
		}
		else if (RDIR(input[i]))
		{
			i += get_rdir_type(NULL, &input[i]);
			i += token_len(&input[i], env, NOT_EXP);
		}
		else
			i++;
	}
}

	int
cmd_len(t_cmd *cmd, char *input, char *env[])
{
	int	i;

	i = 0;
	while (input[i] && !(input[i] == ';' || input[i] == '|'))
	{	
		//ft_printf("\n%d", (int)input[i]);
		if (token_len(&input[i], env, NOT_EXP))
		{
			cmd->argc++;
			i += token_len(&input[i], env, NOT_EXP);
		}
		else if (RDIR(input[i]))
			i += get_cmd_rdir(&cmd->rdir, &input[i], env);
		else
			i++;
	}
	return (i);
}

	t_cmd*
parse_input(char *input, char *env[])
{
	t_cmd	*cmd = NULL;
	int		len;
	
	if (!input)
		return (NULL);
	cmd = init_cmd(input);
	if (!cmd)
		return (NULL);//remplacer par un cas d'erreur ? 
	len = cmd_len(cmd, input, env);
	cmd->argv = (char**)malloc(sizeof(char *) * cmd->argc);
	get_cmd_argv(cmd, input, env, len);
	if (input[len] && input[len] == '|')
		cmd->pipe++;
	if (input[len] && (input[len] == ';' || input[len] == '|'))
		len++;
	if (input[len])//gérer l'erreur 2 ';' a suivre
		cmd->next = parse_input(&input[len], env);
	return (cmd);
}