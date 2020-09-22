/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 12:17:07 by celeloup          #+#    #+#             */
/*   Updated: 2020/09/22 12:10:10 by celeloup         ###   ########.fr       */
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
** Type 1 = in, type 2 = out, type 3 = both
*/

int		redirections(t_rdir *rd, int type)
{
	int fd;
	int ret;

	while (rd)
	{
		if (rd->type == RDIR_IN && (type == 1 || type == 3))
		{
			fd = open(rd->value, O_RDONLY);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
			if (dup2(fd, STDIN_FILENO) == -1)
				return (-1);
			if (close(fd) == -1)
				return (-1);
			ret = 1;
		}
		else if (rd->type == RDIR_OUT && (type == 2 || type == 3))
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (-1);
			if (close(fd) == -1)
				return (-1);
			ret = 1;
		}
		else if (type == 1 || type == 3)
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (-1);
			if (close(fd) == -1)
				return (-1);
			ret = 1;
		}
		rd = rd->next;
	}
	return (ret);
}

int		get_fd_redir(t_rdir *rd, int type)
{
	int fd;

	fd = -2;
	while (rd)
	{
		if (rd->type == RDIR_IN && (type == 1 || type == 3))
		{
			fd = open(rd->value, O_RDONLY);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
		}
		else if (rd->type == RDIR_OUT && (type == 2 || type == 3))
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
		}
		else if (type == 1 || type == 3)
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
		}
		rd = rd->next;
	}
	return (fd);
}

int		apply_redir(int fd, int flux)
{
	if (fd == -2)
		return (0);
	if (dup2(fd, flux) == -1)
		error_exit("dup", strerror(errno));
	if (close(fd) == -1)
		error_exit("dup", strerror(errno));
	return (0);
}

int		exec_cmds(t_cmd *cmd, char **env[])
{
	int		status;
	int		ret;
	t_cmd	*prev_cmd;
	int		fdin;
	int		fdout;
	int		tmpin;
	int		tmpout;
	int		pipe_length;
	int		fdpipe[2];

	tmpin = dup(STDIN_FILENO);
	tmpout = dup(STDOUT_FILENO);
	status = 0;
	prev_cmd = NULL;
	while (cmd)
	{
		if (builtin(cmd->argv[0]) != NULL && cmd->pipe == 0)
		{
			if (redirections(cmd->rdir, 3) == -1)
				error_exit("redirection", "failed.");
			status = (*builtin(cmd->argv[0]))(cmd, env);
		}
		else
		{
			if (builtin(cmd->argv[0]) == NULL)
				get_cmd_path(&cmd->argv[0], *env);
			//PIPE
			if (cmd->pipe == 1)
			{
				pipe_length = size_pipeline(cmd);
				if ((fdin = get_fd_redir(cmd->rdir, 1)) == -2)
					fdin = dup(tmpin);
				while (pipe_length != 0)
				{
					apply_redir(fdin, 0);
					if (cmd->pipe == 0)
					{
						if ((fdout = get_fd_redir(cmd->rdir, 2)) == -2)
							fdout = dup(tmpout);
					}
					else
					{
						pipe(fdpipe);
						fdout = fdpipe[1];
						fdin = fdpipe[0];
					}
					apply_redir(fdout, 1);
					ret = fork();
					if (ret == 0)
					{
						if (builtin(cmd->argv[0]) != NULL)
						{
							status = (*builtin(cmd->argv[0]))(cmd, env);
							return (-1);
						}
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
					waitpid(ret, &status, 0);
				}
			}
			//NO PIPE
			else
			{
				ret = fork();
				if (ret == 0)
				{
					if (redirections(cmd->rdir, 3) == -1)
						error_exit("redirection", "failed.");
					if (builtin(cmd->argv[0]) != NULL)
					{
						status = (*builtin(cmd->argv[0]))(cmd, env);
						return (-1);
					}
					else
					{
						get_cmd_path(&cmd->argv[0], *env);
						execve(cmd->argv[0], cmd->argv, *env);
						error_exit(cmd->argv[0], "command not found.");
					}
				}
				else
					waitpid(ret, &status, 0);
			}
		}
		cmd = cmd->next;
	}
	apply_redir(tmpin, STDIN_FILENO);
	apply_redir(tmpout, STDOUT_FILENO);
	return (status);
}
