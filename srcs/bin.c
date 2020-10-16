/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/16 17:29:03 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/16 17:30:22 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*path_join(char *bin, char *path)
{
	char	*new_path;
	size_t	i;
	size_t	j;

	if ((new_path = malloc((ft_strlen(bin) + ft_strlen(path) + 2)
		* sizeof(char))) == NULL)
		return (NULL);
	i = 0;
	while (i < ft_strlen(bin))
	{
		new_path[i] = bin[i];
		i++;
	}
	new_path[i] = '/';
	i++;
	j = 0;
	while (j < ft_strlen(path))
	{
		new_path[i] = path[j];
		i++;
		j++;
	}
	new_path[i] = '\0';
	return (new_path);
}

char	*search_path(char **bin_tab, char *cmd)
{
	int				i;
	DIR				*dir;
	struct dirent	*entry;
	char			*cmd_name;

	i = 0;
	while (bin_tab && bin_tab[i])
	{
		if ((dir = opendir(bin_tab[i])) != NULL)
		{
			while ((entry = readdir(dir)))
			{
				if (!ft_strcmp(entry->d_name, cmd))
				{
					closedir(dir);
					cmd_name = path_join(bin_tab[i], cmd);
					return (cmd_name);
				}
			}
			closedir(dir);
		}
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, char *env[])
{
	char	*path;
	char	**bin_tab;
	char	*cmd_name;

	if (ft_strchr(cmd, '/') != NULL || !(ft_strncmp(cmd, ".", 1)))
		return (cmd);
	path = get_var_value("$PATH", env);
	if (path == NULL)
		return (cmd);
	bin_tab = ft_split(path, ':');
	free(path);
	cmd_name = search_path(bin_tab, cmd);
	free_tab(bin_tab);
	if (cmd_name != NULL)
		return (cmd_name);
	else
		return (cmd);
}

int		file_exist(char *file_name)
{
	DIR				*dir;
	struct dirent	*entry;

	if ((dir = opendir(".")) != NULL)
	{
		while ((entry = readdir(dir)))
		{
			if (!ft_strcmp(entry->d_name, file_name))
			{
				closedir(dir);
				return (1);
			}
		}
		closedir(dir);
	}
	return (0);
}
