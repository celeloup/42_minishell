/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/17 13:11:55 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		edit_shell_level(char **new_env[], char *env[], int shlvl)
{
	char	*shlvl_str;
	char	*tmp;
	int		shlvl_int;

	if (!env || !shlvl)
	{
		(*new_env)[0] = ft_strdup("SHLVL=1");
		return (1);
	}
	else
	{
		tmp = get_var_value("$SHLVL", env);
		shlvl_int = atoi(tmp) + 1;
		tmp = free_and_null_str(&tmp);
		tmp = ft_itoa(shlvl_int);
		shlvl_str = ft_strjoin("SHLVL=", tmp);
		edit_var(new_env, NULL, shlvl_str);
		shlvl_str = free_and_null_str(&shlvl_str);
		tmp = free_and_null_str(&tmp);
	}
	return (0);
}

/*
** "?=" is set at the last line of env to store the last exit status value
*/

char	**init_env(char *env[])
{
	int		i;
	char	**new_env;
	int		shlvl;

	i = 0;
	shlvl = 0;
	while (env && env[i])
		if (!ft_strncmp("SHLVL=", env[i++], 6))
			shlvl = 1;
	if (!(new_env = (char**)malloc((i + 3 - shlvl) * sizeof(char*))))
		return (NULL);
	i = 0;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], "_=", 2))
			new_env[i] = ft_strdup("_=/bin/bash");
		else
			new_env[i] = ft_strdup(env[i]);
		i++;
	}
	i += edit_shell_level(&new_env, env, shlvl);
	new_env[i] = ft_strdup("?=0");
	new_env[i + 1] = NULL;
	return (new_env);
}

t_rdir	*init_rdir(void)
{
	t_rdir	*rdir;

	rdir = NULL;
	rdir = (t_rdir *)malloc(sizeof(t_rdir));
	if (!rdir)
		return (NULL);
	rdir->type = 0;
	rdir->value = NULL;
	rdir->next = NULL;
	return (rdir);
}

char	**init_argv(int argc)
{
	char	**argv;
	int		i;

	if ((argv = (char**)malloc(sizeof(char*) * (argc + 1))) == NULL)
		return (NULL);
	i = 0;
	while (i < argc)
	{
		argv[i] = NULL;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = NULL;
	cmd = (t_cmd*)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->argc = 0;
	cmd->rdir = NULL;
	cmd->pipe = 0;
	cmd->next = NULL;
	cmd->pid = -1;
	cmd->status = 1;
	return (cmd);
}
