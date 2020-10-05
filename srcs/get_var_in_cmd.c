/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_in_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 12:17:07 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/05 17:51:20 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		words_in_arg(char *arg)
{
	int		i;
	int		wc;

	i = 0;
	wc = 0;
	if (!arg)
		return (0);
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
		ft_printf("\nnew_argv is : >%s<", new_argv);//debug
		argc += words_in_arg(new_argv);
		ft_printf("\nnew argc is = %d", argc);//debug
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
	i = 0;
	expanded = NULL;
	while (argv[i])
	{
		ft_printf("\nargv[i] is : %s", argv[i]);//debug
		if (expanded)
			free(expanded);
		if ((expanded = get_expanded_token(argv[i], env)))
			output = ft_strjoinfree(output, expanded);
		ft_printf("\noutput is : %s", output);//debug
		if (argv[i + 1])
			output = ft_strjoinfree(output, " ");
		i++;
	}
	if (expanded)
		free(expanded);
	return (output);
}

int		arg_list_len(char *arg_list, int go_to_next)
{
	int		len;

	len = 0;
	while (arg_list[len] && !is_ifs(arg_list[len]))
		len++;
	if (!len)
		return (1);
	if (!go_to_next)
		return (len);
	while (arg_list[len] && is_ifs(arg_list[len]))
		len++;	
	return (len);
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

void	get_new_cmd_argv(t_cmd *new, t_cmd *cmd, char *env[])
{
	char	*arg_list;
	int		i;
	int		j;
	int		argc;

	i = 0;
	j = 0;
	argc = 0;
	arg_list = NULL;
	arg_list = get_arg_list(cmd->argv, env);
	ft_printf("\n arg list is : %s", arg_list);
	while (arg_list[i])
	{
		if (new->argv[j])
			free(new->argv[j]);
		ft_printf("\n &arg list is : >%s<", &arg_list[i]);
		new->argv[j] = get_new_argv(&arg_list[i]);
		i += arg_list_len(&arg_list[i], YES);
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

t_cmd	*get_var_in_cmd(t_cmd *cmd, char *env[])
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
	get_new_cmd_argv(new, cmd, env);
	new->next = copy_cmd(cmd->next);
	new->pipe = (cmd->pipe);
	new->rdir = copy_rdir(cmd->rdir);
	ft_putstr_fd("\nARGS get_var_in_cmd fin NEW", 1);
	print_args(new->argc, new->argv);//degug
	ft_printf("\n\n");//debug
	return (new);
}
