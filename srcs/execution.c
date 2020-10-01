/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 12:17:07 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/01 19:06:51 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>

void	path_join(char *bin, char **path)
{
	char	*new_path;
	size_t	i;
	size_t	j;

	if ((new_path = malloc((ft_strlen(bin) + ft_strlen(*path) + 2)
		* sizeof(char))) == NULL)
		return ;
	i = 0;
	while (i < ft_strlen(bin))
	{
		new_path[i] = bin[i];
		i++;
	}
	new_path[i] = '/';
	i++;
	j = 0;
	while (j < ft_strlen(*path))
	{
		new_path[i] = (*path)[j];
		i++;
		j++;
	}
	new_path[i] = '\0';
	free(*path);
	*path = new_path;
}

void	search_path(char **bin_tab, char **cmd)
{
	int				i;
	DIR				*dir;
	struct dirent	*entry;

	i = 0;
	while (bin_tab[i])
	{
		if ((dir = opendir(bin_tab[i])) != NULL)
		{
			while ((entry = readdir(dir)))
			{
				if (!ft_strcmp(entry->d_name, *cmd))
				{
					closedir(dir);
					path_join(bin_tab[i], cmd);
					free_tab(bin_tab);
					return ;
				}
			}
			closedir(dir);
		}
		i++;
	}
	free_tab(bin_tab);
}

void	get_cmd_path(char **cmd, char *env[])
{
	char			*path;
	char			**bin_tab;

	if (ft_strchr(*cmd, '/') != NULL || !(ft_strncmp(*cmd, "./", 2)))
		return ;
	path = get_var_value("$PATH", env);
	bin_tab = ft_split(path, ':');
	free(path);
	search_path(bin_tab, cmd);
}

int		(*builtin(char *cmd_name))(t_cmd*, char***)
{
	if (ft_strcmp("exit", cmd_name) == 0)
		return (ft_exit);
	else if (ft_strcmp("echo", cmd_name) == 0)
		return (ft_echo);
	else if (ft_strcmp("cd", cmd_name) == 0)
		return (ft_cd);
	else if (ft_strcmp("pwd", cmd_name) == 0)
		return (ft_pwd);
	else if (ft_strcmp("export", cmd_name) == 0)
		return (ft_export);
	else if (ft_strcmp("unset", cmd_name) == 0)
		return (ft_unset);
	else if (ft_strcmp("env", cmd_name) == 0)
		return (ft_env);
	else
		return (NULL);
}

int		size_pipeline(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd->pipe == 1)
	{
		i++;
		cmd = cmd->next;
	}
	i++;
	return (i);
}

void	error_exit(char *actor, char *msg)
{
	//write(2, "minishell: ", 11);
	write(2, actor, ft_strlen(actor));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}

/*
** direction 1 = in, direction 2 = out, direction 3 = both
*/

int		redirect(t_rdir *rd, int direction, int fd, int flux)
{
	if (fd != -1 && flux != -1)
	{
		if (dup2(fd, flux) == -1)
			return (-1);
		if (close(fd) == -1)
			return (-1);
	}
	while (rd)
	{
		if (rd->type == RDIR_IN && (direction == 1 || direction == 3))
		{
			fd = open(rd->value, O_RDONLY);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
			flux = STDIN_FILENO;
		}
		else if (rd->type == RDIR_OUT && (direction == 2 || direction == 3))
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
			flux = STDOUT_FILENO;
		}
		else if (direction == 1 || direction == 3)
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
			flux = STDOUT_FILENO;
		}
		if (dup2(fd, flux) == -1)
			return (-1);
		if (close(fd) == -1)
			return (-1);
		rd = rd->next;
	}
	return (0);
}

int		exec_cmds(t_cmd *cmd, char **env[])
{
	int status;
	int pid;
	int fdpipe[2];
	int pipe_length;
	int tmpin;
	int tmpout;

	tmpin = dup(STDIN_FILENO);
	tmpout = dup(STDOUT_FILENO);
	status = 0;
	while (cmd)
	{
		get_var_in_cmd(cmd, *env);
		if (builtin(cmd->argv[0]) != NULL && cmd->pipe == 0)
		{
			if (redirect(cmd->rdir, 3, -1, -1) == -1)
				error_exit("redirection", "failed.");
			status = (*builtin(cmd->argv[0]))(cmd, env);
		}
		else
		{
			if (builtin(cmd->argv[0]) == NULL)
				get_cmd_path(&cmd->argv[0], *env);
			if (cmd->pipe == 1)
			{
				pipe_length = size_pipeline(cmd);
				fdpipe[0] = dup(tmpin);
				while (pipe_length != 0)
				{
					redirect(cmd->rdir, 1, fdpipe[0], STDIN_FILENO);
					if (cmd->pipe != 0)
					{
						pipe(fdpipe);
						redirect(cmd->rdir, 2, fdpipe[1], STDOUT_FILENO);
					}
					else
						redirect(cmd->rdir, 2, tmpout, STDOUT_FILENO);
					pid = fork();
					if (pid == 0)
					{
						if (builtin(cmd->argv[0]) != NULL)
							return ((*builtin(cmd->argv[0]))(cmd, env));
						else
						{
							get_cmd_path(&cmd->argv[0], *env);
							execve(cmd->argv[0], cmd->argv, *env);
							error_exit(cmd->argv[0], "command not found.");
						}
					}
					if (cmd->pipe == 1)
						cmd = cmd->next;
					pipe_length--;
					waitpid(pid, &status, 0);
				}
			}
			else
			{
				pid = fork();
				if (pid == 0)
				{
					if (redirect(cmd->rdir, 3, -1, -1) == -1)
						error_exit("redirection", "failed.");
					execve(cmd->argv[0], cmd->argv, *env);
					error_exit(cmd->argv[0], "command not found.");
				}
				else
					waitpid(pid, &status, 0);
			}
		}
		cmd = cmd->next;
	}
	redirect(NULL, 0, tmpin, STDIN_FILENO);
	redirect(NULL, 0, tmpout, STDOUT_FILENO);
	return (status);
}
