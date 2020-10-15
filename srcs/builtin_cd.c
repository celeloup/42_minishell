/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/28 15:08:57 by user42            #+#    #+#             */
/*   Updated: 2020/10/15 12:49:31 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		go_to_next_component(char **new, char *path, int i, struct stat **buf)
{
	char	*tmp;
	char	*tmp2;

	if (path[0] == '.' && ft_strncmp(path, "..", 2))
		return (go_to_next_component_index(path + 1) + 1);
	else if (!ft_strncmp(path, "..", 2) && stat(*new, *buf))
		return (-i - 1);
	else if (!ft_strncmp(path, "..", 2) && !go_to_parent_directory(new))
		return (go_to_next_component_index(path + 2) + 2);
	else if (!ft_strncmp(path, "..", 2))
		return (0);
	else if (path[0] != '/')
	{
		i = 0;
		while (path[i] && path[i] != '/')
			i++;
		tmp = ft_strjoin(*new, "/");
		tmp2 = ft_substr(path, 0, i);
		*new = free_and_null_str(new);
		*new = ft_strjoin(tmp, tmp2);
		tmp = free_and_null_str(&tmp);
		tmp2 = free_and_null_str(&tmp2);
		return (i);
	}
	return (go_to_next_component_index(path));
}

int		change_relative_to_absolute_path(char **path)
{
	int			i;
	int			next;
	char		*new_path;
	struct stat	*buf;

	if (!ft_strcmp(*path, "/"))
		return (0);
	i = 0;
	new_path = ft_strdup("");
	buf = (struct stat *)malloc(sizeof(struct stat));
	while (i >= 0 && (*path)[i]
		&& (next = go_to_next_component(&new_path, &(*path)[i], i, &buf)))
		i += next;
	free(buf);
	if (i > 0)
	{
		*path = free_and_null_str(path);
		*path = ft_strdup(new_path);
		new_path = free_and_null_str(&new_path);
		return (0);
	}
	new_path = free_and_null_str(&new_path);
	*path = free_and_null_str(path);
	return (-i);
}

int		deal_special_cd_args(char **curpath, t_cmd *cmd, char **env[])
{
	char	*tmp;

	if (cmd->argc > 2)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2) + 1);
	if (!cmd->argv[1] && !(*curpath = get_var_value("$HOME", *env)))
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2) + 1);
	else if (cmd->argv[1] && !(ft_strcmp(cmd->argv[1], "-")))
		ft_printf("%s\n", (*curpath = get_var_value("$OLDPWD", *env)));
	else if (cmd->argv[1] && !(ft_strncmp(cmd->argv[1], "~", 1)))
	{
		tmp = get_var_value("$HOME", *env);
		*curpath = ft_strjoin(tmp, &cmd->argv[1][1]);
		tmp = free_and_null_str(&tmp);
	}
	else if (cmd->argv[1])
		*curpath = ft_strdup(cmd->argv[1]);
	return (0);
}

int		ft_cd_error_msg(char *arg)
{
	if (ft_strcmp("parent", arg))
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	else
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: ", 2);
		ft_putstr_fd("cannot access parent directories: ", 2);
	}
	ft_putstr_fd(strerror(errno), 2);
	ft_putchar_fd('\n', 2);
	return (1);
}

int		ft_cd(t_cmd *cmd, char **env[])
{
	char		*curpath;
	int			print;

	curpath = NULL;
	print = 0;
	if (deal_special_cd_args(&curpath, cmd, env))
		return (1);
	if (curpath && curpath[0] && !(curpath[0] == '/' || curpath[0] == '.')
		&& (get_curpath_with_cd_path(&curpath, *env)))
		print = 1;
	if ((!curpath || !curpath[0] || curpath[0] != '/')
		&& join_pwd_and_curpath(&curpath, *env))
		ft_cd_error_msg("parent");
	else if (curpath && (change_relative_to_absolute_path(&curpath))
		&& ft_cd_error_msg(cmd->argv[1])
		&& !(curpath = free_and_null_str(&curpath)))
		return (1);
	else if ((chdir(curpath)) && ft_cd_error_msg(cmd->argv[1])
		&& !(curpath = free_and_null_str(&curpath)))
		return (1);
	edit_pwd_and_oldpwd(curpath, "cd", env);
	if (print)
		ft_printf("%s\n", curpath);
	curpath = free_and_null_str(&curpath);
	return (0);
}
