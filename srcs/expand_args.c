/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 12:17:07 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/08 18:50:12 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
int		words_in_expanded_token(char *arg, char *env[])
{
	int		i;
	int		wc;
	
	i = 0;
	wc = 0;
	if (!arg || (arg && !arg[0]))
		return (1);
	while (arg && arg[i])
	{
		wc++;
		i += token_len(&arg[i], env, NOT_EXP);
	}
	return (wc);
}

int		get_new_argc(t_cmd *cmd, char *env[])
{
	int		argc;
	int		i;
	int		j;
	char	*tmp;

	argc = 0;
	i = 0;
	tmp = NULL;
	while (cmd->argv[i])
	{
		j = 0;
		while (cmd->argv[i][j])
		{
			tmp = free_and_null(&tmp);
			tmp = get_expanded_token(&cmd->argv[i][j], env);
			argc += words_in_expanded_token(tmp, env);
			j += token_len(&cmd->argv[i][j], env, NOT_EXP);
		}
		i++;
	}
	tmp = free_and_null(&tmp);
	return (argc);
}

char	*get_new_argv(char *arg_list)
{
	int		len;
	char	*new_argv;

	len = arg_list_len(arg_list, NO);
	ft_printf("\n arg list len is : %d", len);
	new_argv = NULL;
	if (!(new_argv = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	new_argv = ft_strncpy(new_argv, arg_list, len);
	new_argv[len] = '\0';
	return (new_argv);
}

void	pass_out_args(char **argv, char *tmp, )
{
	
}

void	get_new_args(t_cmd *new, t_cmd *cmd, char *env[])
{
	int		i;
	int		j;
	int		k;
	char	*tmp;

	i = 0;
	k = 0;
	tmp = NULL;
	while (cmd->argv[i])
	{
		j = 0;
		while (cmd->argv[i][j])
		{
			tmp = free_and_null(&tmp);
			tmp = get_expanded_token(&cmd->argv[i][j], env);
			pass_out_args(new->argv, tmp, &k);
			while (tmp)
			argc += words_in_expanded_token(tmp, env);
			new->argv[k] = get_expanded_token(&cmd->argv[i][j], env);
			k++;
			j += token_len(&cmd->argv[i][j], env, NOT_EXP);
		}
		i++;
	}
	tmp = free_and_null(&tmp);
	return (argc);
}
*/

char	**copy_argv(char **argv, int argc)
{
	char	**copy;
	int		i;
	
	copy = (char **)malloc(sizeof(char *) * (argc + 1));
	i = 0;
	while (i < argc)
	{
		copy[i] = ft_strdup(argv[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

t_rdir	*copy_rdir(t_rdir *rdir)
{
	t_rdir	*copy;

	if (!rdir)
		return (NULL);
	copy = init_rdir();
	copy->next = copy_rdir(rdir->next);
	copy->type = rdir->type;
	copy->value = ft_strdup(rdir->value);
	return (copy);
}

t_cmd	*copy_cmd(t_cmd *cmd)
{
	t_cmd	*copy;

	if (!cmd)
		return (NULL);
	copy = init_cmd();
	copy->argc = cmd->argc;
	copy->argv = copy_argv(cmd->argv, cmd->argc);
	copy->pipe = cmd->pipe;
	copy->rdir = copy_rdir(cmd->rdir);
	copy->next = copy_cmd(cmd->next);
	return (copy);
}

/*
t_cmd	*expand_args_in_cmd(t_cmd *cmd, char *env[])
{
	t_cmd	*new;
	int		i;
	int		len;

	if (!cmd)
		return (NULL);
	new = init_cmd();
	i = 0;
	len = 0;
	ft_putstr_fd("\nARGS get_var_in_cmd debut CMD", 1);//debug
	print_args(cmd->argc, cmd->argv);//degug
	new->argc = get_new_argc(cmd, env);
	ft_printf("\nNEW ARGC IS : %d", new->argc);//debug
	new->argv = init_argv(new->argc);
	get_new_args(new, cmd, env);
	new->next = copy_cmd(cmd->next);
	new->pipe = (cmd->pipe);
	new->rdir = copy_rdir(cmd->rdir);
	ft_putstr_fd("\nARGS get_var_in_cmd fin NEW", 1);
	print_args(new->argc, new->argv);//degug
	ft_printf("\n\n");//debug
	return (new);
}


int		words_in_var_value(char *arg, char *before, char *env[])
{
	int		wc;
	char	*var_name;
	char	*var_value;
	char	*tmp;

	var_name = NULL;
	var_value = NULL;
	tmp = NULL;
	var_name = get_var_name(arg);
	var_value = get_var_value(var_name, env, DOUBLE_QUOTE, NO);
	tmp = ft_strjoin(before, var_value);
	var_value = free_and_null(&var_value);
	var_name = free_and_null(&var_name);
	tmp = free_and_null(&tmp);
	return (wc);
}
*/

int		arg_with_expanded_var_len(char *arg, char *env[])
{
	int	len;
	int	null_found;
	int	i;

	len = 0;
	i = 0;
	null_found = NO;
	while (arg[i])
	{
		ft_printf("\nARGEXPVARLEN &arg[%d]=>%s<", i, &arg[i]);
		if (arg[i] == DOLLAR && arg[ft_strlen(arg) - 1] == DOLLAR
			&&  i != (int)(ft_strlen(arg) - 1))
			len += var_len_not_exp(&arg[i]) + 2;
		else if (arg[i] == DOLLAR)
		{
			if (var_len_exp(&arg[i], env) == -1)
				null_found = YES;
			else
				len += var_len_exp(&arg[i], env);
		}
		else
			len += len_after_char(&arg[i], env, NO, NOT_EXP);
		ft_printf("\nARGEXPVARLEN len=>%d<", len);
		i += len_after_char(&arg[i], env, NO, NOT_EXP);
	}
	if (null_found && !len)
		return (-1);
	return (len);
}

char	*arg_with_expanded_var(char *arg, char *env[])
{
	char	*expanded;
	char	*tmp;
	int		len;
	int		i;

	tmp = NULL;
	ft_printf("\nARGEXPVAR arg = >%s<", arg);
	if ((len = arg_with_expanded_var_len(arg, env)) < 0)
		return (NULL);
	ft_printf("\nARGEXPVAR len = %d", len);
	expanded = (char*)malloc(sizeof(char) * (len + 1));
	expanded[len] = '\0';
	i = 0;
	len = 0;
	while (expanded[len] && arg[i])
	{
		if (arg[i] == DOLLAR && arg[ft_strlen(arg) - 1] == DOLLAR
			&& i != (int)(ft_strlen(arg) - 1))
		{
			ft_strcpy(&expanded[len], "\"");
			ft_strncpy(&expanded[len + 1], &arg[i], var_len_not_exp(&arg[i]));
			len += var_len_not_exp(&arg[i]) + 2;
			ft_strcpy(&expanded[len - 1], "\"");
		}
		else if (arg[i] == DOLLAR)
		{
			tmp = expanded_str(&arg[i], env, DOUBLE_QUOTE);
			ft_strcpy(&expanded[len], tmp);
			len += ft_strlen(tmp);
		}
		else
		{
			ft_strncpy(&expanded[len], &arg[i], len_after_char(&arg[i], env, NO, NO));
			len += len_after_char(&arg[i], env, NO, NO);
		}
		i += len_after_char(&arg[i], env, NO, NOT_EXP);
		tmp = free_and_null_str(&tmp);
	}
	tmp = free_and_null_str(&tmp);
	return (expanded);
}

int		words_in_arg(char *arg, char *env[])
{
	int		i;
	int		wc;
	char	*tmp;

	tmp = NULL;
	i = 0;
	wc = 0;
	if (!(tmp = arg_with_expanded_var(arg, env)))
		return (0);
	if (!tmp[0] || tmp[ft_strlen(arg) - 1] == DOLLAR)
		return (1);
	while (tmp[i] && is_ifs(tmp[i]))
		i++;
	while (tmp[i])
	{
		wc++;
		while (tmp[i] && !is_ifs(tmp[i]))
			i += len_after_char(&tmp[i], env, NO, NOT_EXP);
		while (tmp[i] && is_ifs(tmp[i]))
			i++;
	}
	tmp = free_and_null_str(&tmp);
	if (wc == 0)
		wc = 1;
	return (wc);
}

int		get_new_argc(char **argv, char *env[])
{
	int		argc;
	int		i;

	argc = 0;
	i = 0;
	while (argv[i])
	{
		argc += words_in_arg(argv[i], env);
		i++;
	}
	
	return (argc);
}

char	*expanded_str(char *input, char *env[], int quote)
{
	char	*var_name;
	char	*var_value;

	var_name = NULL;
	var_value = NULL;
	if (!input)
		return (NULL);//vérfier ?
	else if (input[0] == BACKSLASH)
		return (get_escaped_char(input, quote));
	else if (is_quote(input[0]) && !quote)
		return (get_quote(input, env));
	else if (input[0] == DOLLAR && quote == DOUBLE_QUOTE)
	{
		var_name = get_var_name(input);
		var_value = get_var_value(var_name, env);
		var_name = free_and_null_str(&var_name);
		return (var_value);
	}
	else
		return (ft_substr(input, 0, len_after_char(input, env, quote, NOT_EXP)));
}

char	**split_arg(char **new, char *semi_exp_arg, char *env[], int j)
{
	int		i;

	i = 0;
	while (semi_exp_arg[i] && is_ifs(semi_exp_arg[i]))
		i++;
	if (!semi_exp_arg[i])
		new[j] = ft_strdup("");
	while (semi_exp_arg[i])
	{
		ft_printf("\nSPLIT_ARG J = %d", j);
		ft_printf("\nSPLIT_ARG &semi_exp_arg = >%s<", &semi_exp_arg[i]);
		if (!(new[j] = get_expanded_token(&semi_exp_arg[i], env)))
			break;
		ft_printf("\nSPLIT_ARG new[%d] = >%s<", j, new[j]);
		j++;
		i += token_len(&semi_exp_arg[i], env, NOT_EXP);
		while (semi_exp_arg[i] && is_ifs(semi_exp_arg[i]))
			i++;
	}
	return (new);
}

char	**get_new_argv(char **old, int new_argc, char *env[])
{
	char	**new;
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tmp = NULL;
	new = init_argv(new_argc);
	while (old[i] && j < new_argc)
	{
		while (old[i] && !(tmp = arg_with_expanded_var(old[i], env)))
			i++;
		if (!old[i])
			break;
		new = split_arg(new, tmp, env, j);
		tmp = free_and_null_str(&tmp);
		j += words_in_arg(old[i], env);
		i++;
	}
	return (new);
}

void	expand_args_in_cmd(t_cmd *cmd, char *env[])
{
	int		i;
	t_cmd	*new;
	
	i = 0;
	new = init_cmd();
	ft_printf("\n\nEXPAND_ARGS GETTING ARGC\n");
	new->argc = get_new_argc(cmd->argv, env);
	ft_printf("\n\nEXPAND_ARGS ARGC = %d\n", new->argc);
	ft_printf("\n\nEXPAND_ARGS GETTING ARGV\n");
	new->argv = get_new_argv(cmd->argv, new->argc, env);
//	new->pipe = cmd->pipe;
//	new->rdir = copy_rdir(cmd->rdir);//a voir avec Celia si on modifie à ce moment-là
//	new->next = copy_cmd(cmd->next);
	cmd->argv = free_and_null_tab(&cmd->argv);
	cmd->argv = copy_argv(new->argv, new->argc);
	cmd->argc = new->argc;
	new = free_and_null_cmd(&new);
	print_args(cmd->argc, cmd->argv);//debug
}
