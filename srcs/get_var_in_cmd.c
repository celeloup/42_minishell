/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_in_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 12:17:07 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/01 20:59:30 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		words_in_arg(char *arg)
{
	int		i;
	int		wc;

	i = 0;
	wc = 0;
	while (arg[i])
	{
		while (arg[i] && is_ifs(arg[i]))
			i++;
		if (arg[i])
			wc++;
		while (arg[i] && !is_ifs(arg[i]))
			i++;
	}
	return (wc);
}

int		get_new_argc(t_cmd *cmd, char *env[])
{
	int		argc;
	int		i;
	char	*new_argv;

	argc = 0;
	i = 0;
	new_argv = NULL;
	while (cmd->argv[i])
	{
		if (new_argv)
			free(new_argv);
		new_argv = get_expanded_token(cmd->argv[i], env);
		argc += words_in_arg(new_argv);
		i++;
	}
	if (new_argv)
		free(new_argv);
	new_argv = NULL;	
	return (argc);
}

char	*get_arg_list(char **argv, char *env[])
{
	char	*output;
	char	*expanded;
	int		i;

	output = NULL;
	output = ft_strdup("");
	expanded = NULL;
	i = 0;
	while (argv[i])
	{
		if (expanded)
			free(expanded);
		expanded = get_expanded_token(argv[i], env);
		output = ft_strjoinfree(output, expanded);
		if (argv[i + 1])
			output = ft_strjoinfree(output, " ");
		i++;
	}
	if (expanded)
		free(expanded);
	return (output);
}

int		get_new_argv(char *new_argv, char *arg_list)
{
	int		len;
	
	len = 0;
	while (arg_list[len] && !is_ifs(arg_list[len]))
		len++;
	if (new_argv)
		free(new_argv);
	new_argv = (char*)malloc(sizeof(char) * (len + 1));
	new_argv = ft_strncpy(new_argv, arg_list, len);
	new_argv[len] = '\0';
	while (arg_list[len] && is_ifs(arg_list[len]))
		len++;
	return (len);
}

void	get_new_cmd_argv(t_cmd *new, t_cmd *cmd, char *env[])
{
	char	*arg_list;
	int		i;
	int		j;

	i = 0;
	j = 0;
	arg_list = NULL;
	arg_list = get_arg_list(cmd->argv, env);
	while (arg_list[i])
	{
		i += get_new_argv(new->argv[j], &arg_list[i]);
		if (new->argv[j])
			j++;
	}
	if (arg_list)
		free(arg_list);
	arg_list = NULL;
}

char	**argv_dup(char	**argv, int argc)
{
	char	**cpy;
	int		i;
	
	cpy = (char **)malloc(sizeof(char *) * (argc + 1));
	i = 0;
	while (i < argc)
	{
		cpy[i] = ft_strdup(argv[i]);
		i++;
	}
	cpy[i] = NULL;
	return (cpy);
}

t_rdir	*copy_rdir(t_rdir *rdir)
{
	t_rdir	*cpy;

	if (!rdir)
		return (NULL);
	cpy = init_rdir();
	cpy->next = copy_rdir(rdir->next);
	cpy->type = rdir->type;
	cpy->value = ft_strdup(rdir->value);
	return (cpy);
}

t_cmd	*copy_cmd(t_cmd *cmd)
{
	t_cmd	*cpy;

	if (!cmd)
		return (NULL);
	cpy = init_cmd();
	cpy->argc = cmd->argc;
	cpy->argv = argv_dup(cmd->argv, cmd->argc);
	cpy->pipe = cmd->pipe;
	cpy->rdir = copy_rdir(cmd->rdir);
	cpy->next = copy_cmd(cmd->next);
	return (cpy);
}

void	get_var_in_cmd(t_cmd *cmd, char *env[])
{
	t_cmd	*new;
	int		i;
	int		len;

	new = init_cmd();
	i = 0;
	len = 0;
	new->argc = get_new_argc(cmd, env);
	new->argv = init_argv(new->argc);
	get_new_cmd_argv(new, cmd, env);
	new->next = copy_cmd(cmd->next);
	new->pipe = (cmd->pipe);
	new->rdir = copy_rdir(cmd->rdir);
	free_cmd(cmd);
	cmd = copy_cmd(new);
	free_cmd(new);
}
