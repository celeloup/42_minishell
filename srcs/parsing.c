/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenadier <amenadier@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 15:39:46 by celeloup          #+#    #+#             */
/*   Updated: 2020/09/24 17:07:30 by amenadier        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//IFS = Internal Field Separator

/*
** returns the value of a var (input) in env
** also returns the value of $?
** input[0] == '$'
*/
char	*get_var_value(char *input, char *env[])
{
	size_t	i;
	
	i = 0;
	if (input && !input[1])
		return (ft_strdup("$"));
	// ft_printf("get_var_value input 1 = %s\n", input);
	while (input && env[i])
	{
		// ft_printf("get_var_value env[i] = %s\n", env[i]);
		if (!strncmp(env[i], input + 1, ft_strlen(input + 1))
			&& env[i][ft_strlen(input + 1)] 
			&& env[i][ft_strlen(input + 1)] == '=')
			return (ft_strdup(ft_strrchr(env[i], '=') + 1));
		i++;
	}
	return (ft_strdup(""));//vérifier s'il ne renvoie pas plutôt une chaine vide
}

int		backslash_len(char *input, int quote, int expanded)
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
	
int		single_quote_len(char *input, int expanded)
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
	
int		double_quote_len(char *input, char *env[], int expanded)
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

int		quote_len(char *input, char *env[], int expanded)
{
	if (input[0] == DOUBLE_QUOTE)
		return (double_quote_len(input, env, expanded));
	else
		return (single_quote_len(input, expanded));
}

/*
** gets the len of a var (input) after =
** input[0] == '$' 
*/
int		var_len(char *input, char *env[], int expanded)//dollar inclus
{
	int		len;
	char	*var_name = NULL;
	char	*var_value = NULL;

	len = 1;
	if (input[len] && input[len] == '?' && !expanded)
		len = 2;
	//else if
	while (input[len] && ft_isalnum(input[len]))// a confirmer
			len++;
	if (!expanded)
		return (len);
	if (expanded && len == 1)
		return (1);//juste le dollar sans caractere alphanu derriere
	var_name = (char *)malloc(sizeof(char) * (len + 1));
	ft_strncpy(var_name, input, len);
	var_name[len] = '\0';
	var_value = get_var_value(var_name, env);
	if (!var_value)
		return (0);//variable n'existe pas donc chaine vide
	len = ft_strlen(var_value);
	free(var_name);
	free(var_value);
	var_name = NULL;
	var_value = NULL;
	return (len);
}
	
int		len_after_char(char *input, char *env[], int quote, int expanded)
{
	// ft_printf("len_after_char _ input = %s\n", input);
	if (!input)
		return(0);
	else if (is_quote(input[0]) && !quote)
		return (quote_len(input, env, expanded));
	else if (input[0] == BKSLASH)
		return (backslash_len(input, quote, expanded));
	else if (input[0] == DOLLAR)
		return (var_len(input, env, expanded));
	else 
		return (1);
}
		
int		token_len(char *input, char *env[], int expanded)
{
	int len;
	int exp_len;

	len = 0;
	exp_len = 0;
	// ft_printf("token_len _ input = %s\n", input);
	while (input[len] && !(is_arg_sep(input[len])))
	{
		exp_len -= len_after_char(&input[len], env, NO_QUOTE, NOT_EXP);
		exp_len += len_after_char(&input[len], env, NO_QUOTE, EXP);
		len += len_after_char(&input[len], env, NO_QUOTE, NOT_EXP);
	}
	if (expanded)
		return (len + exp_len);
	return (len);
}

char	*get_var_name(char *input)
{
	int		i;
	int		len;
	char	*name;

	len = var_len(input, NULL, NOT_EXP);//gérer le cas rien apres dollar ? 
	if (len == 2 && input[1] == '?')
		return (ft_strdup("$?"));
	name = NULL;
	name = (char *)malloc(sizeof(char) * (len + 1));
	name[0] = DOLLAR;
	name[len] = '\0';
	i = 1;
	// ft_printf("parse55");
	while (input[i] && ft_isalnum(input[i]))
	{
		name[i] = input[i];
		i++;
	}
	// ft_printf("parse56");
	return (name);
}

char	*get_escaped_char(char *input, int quote)
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

char	*get_single_quote(char *input)
{
	char	*ret;
	int		len;

	len = single_quote_len(input, EXP);
	ret = ft_substr(input, 1, len);
	return (ret);
}

char	*get_double_quote(char *input, char *env[])
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

char	*get_quote(char *input, char *env[])
{
	if (input[0] == SINGLE_QUOTE)
		return (get_single_quote(input));
	else
		return (get_double_quote(input, env));
}
	
char	*expanded_str(char *input, char *env[], int quote)
{
	// ft_printf("parse65");
	if (!input)
		return (NULL);//vérfier ?
	else if (input[0] == BKSLASH)
		return (get_escaped_char(input, quote));
	else if (is_quote(input[0]) && !quote)
		return (get_quote(input, env));
	else if (input[0] == DOLLAR)
		return (get_var_value(get_var_name(input), env));
	else
		return (ft_substr(input, 0, 1));
}
		
char	*get_token(char *input, char *env[])
{
	char	*token = NULL;
	char	*str = NULL;
	int		i;
	int		j;

	i = 0;
	j = 0;
	token = (char *)malloc(sizeof(char) * (token_len(input, env, EXP) + 1));
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

int		get_rdir_type(t_rdir *rdir, char *input)
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
	while (input[i] && is_ifs(input[i]))
		i++;
	if (!input[i])
		return (parsing_error(NULL, UNEXPECTED_TOKEN));
	else if (input[i] && (is_rdir(input[i]) || is_cmd_sep(input[i])))
		return (parsing_error(&input[i], UNEXPECTED_TOKEN));
	if (rdir)
		rdir->type = rdir_type;
	return (i);
}

int		get_rdir_value(t_rdir *rdir, char *input, char *env[])
{
	// cas d'erreur si token_len = 0 ?
	if (!rdir)// a confirmer
		return (0);// a confirmer
	if (rdir)
	{
		rdir->value = get_token(input, env);
	}
	return (token_len(input, env, NOT_EXP));
}
	
int		get_cmd_rdir(t_rdir **rdir, char *input, char *env[])
{
	int		i;

	if (*rdir)
	{
		while (*rdir)
			rdir = &(*rdir)->next;
	}
	i = 0;
	*rdir = init_rdir();
	if (get_rdir_type(NULL, input) > 0)
		i += get_rdir_type(*rdir, input);
	else
		return (-1);
	i += get_rdir_value(*rdir, &input[i], env);
	return (i);
}

void	get_cmd_argv(t_cmd *cmd, char *input, char *env[], int cmd_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!input)
		return ;
	while (input[i] && i < cmd_len)
	{
		if (token_len(&input[i], env, NOT_EXP))
		{
			cmd->argv[j] = get_token(&input[i], env);
			i += token_len(&input[i], env, NOT_EXP);
			j++;
		}
		else if (is_rdir(input[i]))
		{
			i += get_rdir_type(NULL, &input[i]);
			i += token_len(&input[i], env, NOT_EXP);
		}
		else
			i++;
	}
}

int		cmd_len(t_cmd *cmd, char *input, char *env[])
{
	int	len;

	len = 0;
	// ft_printf("parse21\n");
	while (input && input[len] && is_ifs(input[len]))
		len++;
		// ft_printf("cmd_len &input[len] 22 = %s\n", &input[len]);
	if (input && input[len] && (is_cmd_sep(input[len])))
		return (parsing_error(input, UNEXPECTED_TOKEN));
		// ft_printf("parse23\n");
	while (input && input[len] && !(is_cmd_sep(input[len])))
	{	
		// ft_printf("input[len]=%s\n", &input[len]);
		if (token_len(&input[len], env, NOT_EXP))
		{
			// ft_printf("parse2401\n");
			cmd->argc++;
			len += token_len(&input[len], env, NOT_EXP);
		}
		else if (is_rdir(input[len]))
		{
			// ft_printf("parse241\n");
			if (get_rdir_type(NULL, &input[len]) > 0)
				len += get_cmd_rdir(&cmd->rdir, &input[len], env);
			else
				return (-1);
			// ft_printf("parse242\n");
		}
		else
			len++;
	}
	// ft_printf("parse25\n");
	return (len);
}

t_cmd	*parse_input(char *input, char *env[])
{
	t_cmd	*cmd = NULL;
	int		len;
	
	// ft_printf("parse_input env[i] = %s\n", env[0]);//outil debug a supprimer
	// ft_printf("parse1\n");
	if (!(cmd = init_cmd()))
		return (NULL);
		// ft_printf("parse10\n");
	if ((len = cmd_len(cmd, input, env)) == -1)
		return (free_cmd(cmd));
		// ft_printf("parse11\n");
	if (input && input[len] && input[len + 1] && is_cmd_sep(input[len + 1]))
		parsing_error(&input[len], UNEXPECTED_TOKEN);
		// ft_printf("parse13\n");
	cmd->argv = (char **)malloc(sizeof(char *) * (cmd->argc + 1));
	cmd->argv[cmd->argc] = NULL;
	// ft_printf("parse15\n");
	get_cmd_argv(cmd, input, env, len);
	if (input && input[len] && (input[len] == ';' || input[len] == '|'))
	{
		if (input[len] == '|')
			cmd->pipe++;
		len++;
		cmd->next = parse_input(&input[len], env);
		if (!cmd->next)
			return(free_cmd(cmd));
	}
	return (cmd);
}
