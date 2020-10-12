/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/28 15:08:57 by user42            #+#    #+#             */
/*   Updated: 2020/10/12 21:51:00 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*search_cd_path(char **cd_path, char *curpath, struct stat *buf)
{
	char		*valid_path_found;
	int			i;
	char		*tmp;

	tmp = NULL;
	valid_path_found = NULL;
	i = 0;
	while (cd_path[i] && !valid_path_found)
	{
		if (cd_path[i][ft_strlen(cd_path[i]) - 1] != '/')
			tmp = ft_strjoin(cd_path[i], "/");
		else
			tmp = ft_strdup(cd_path[i]);
		cd_path[i] = free_and_null_str(&cd_path[i]);
		cd_path[i] = ft_strjoin(tmp, curpath);
		tmp = free_and_null_str(&tmp);
		if (cd_path[i][1] && !(stat(cd_path[i], buf)))
			valid_path_found = ft_strdup(cd_path[i]);
		else if ((tmp = ft_strjoin("./", cd_path[i]))
			&& !(stat(tmp, buf)))
			valid_path_found = ft_strdup(tmp);
		tmp = free_and_null_str(&tmp);
		i++;
	}
	return (valid_path_found);
}

int		get_curpath_with_cd_path(char **curpath, char *env[])
{
	char		*cd_path_value;
	char		**cd_path_tab;
	char		*tmp;
	struct stat	*buf;

	if (!(cd_path_value = get_var_value("$CDPATH", env)))
		return (0);
	buf = (struct stat *)malloc(sizeof(struct stat));
	cd_path_tab = ft_split(cd_path_value, ':');
	cd_path_value = free_and_null_str(&cd_path_value);
	if (cd_path_tab && (tmp = search_cd_path(cd_path_tab, *curpath, buf)))
	{
		*curpath = free_and_null_str(curpath);
		*curpath = ft_strdup(tmp);
		tmp = free_and_null_str(&tmp);
		free(buf);
		cd_path_tab = free_and_null_tab(&cd_path_tab);
		return (1);
	}
	free(buf);
	cd_path_tab = free_and_null_tab(&cd_path_tab);
	return (0);
}

int		join_pwd_and_curpath(char **curpath, char *env[])
{
	char	*pwd;
	char	*tmp;
	int		ret;

	ret = 0;
	if (!(tmp = getcwd(NULL, 0)))
	{
		ret = 1;
		tmp = get_var_value("$PWD", env);
	}
	if (tmp[ft_strlen(tmp) - 1] != '/')
		pwd = ft_strjoin(tmp, "/");
	else
		pwd = ft_strdup(tmp);
	tmp = free_and_null_str(&tmp);
	if (!(tmp = ft_strdup(*curpath)))
		tmp = ft_strdup("");
	*curpath = free_and_null_str(curpath);
	*curpath = ft_strjoin(pwd, tmp);
	tmp = free_and_null_str(&tmp);
	pwd = free_and_null_str(&pwd);
	return (ret);
}

int		go_to_parent_directory(char **path)
{
	int		ret;
	int		i;
	char	*tmp;

	ret = 0;
	i = ft_strlen(*path);
	while (i > 0 && (*path)[i - 1] != '/')
		i--;
	if (!(tmp = ft_substr(*path, 0, i - 1)))
	{
		tmp = ft_strdup("/");
		ret = 1;
	}
	*path = free_and_null_str(path);
	*path = ft_strdup(tmp);
	tmp = free_and_null_str(&tmp);
	return (ret);
}

int		go_to_next_component_index(char *path)
{
	int	i;

	i = 0;
	while (path && path[i] && path[i] == '/')
		i++;
	return (i);
}
