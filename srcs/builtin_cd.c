/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/28 15:08:57 by user42            #+#    #+#             */
/*   Updated: 2020/10/10 21:57:30 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		ft_cd_no_arg(char **env[])
{
	int		ret;
	char	*path;

	path = NULL;
	path = get_var_value("$HOME", *env);
	ret = 0;
	if (!path)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	else if (path && path[0])
		ret = chdir(path);
	path = free_and_null_str(&path);
	return (ret);
}

int		ft_cd_arg_issue(t_cmd *cmd, char **env[])
{
	if (!cmd->argv[1])
		return (ft_cd_no_arg(env));
	else if (cmd->argc > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

int		ft_cd_error_msg(char *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putchar_fd('\n', 2);
	return (1);
}

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
		if (!(stat(cd_path[i], buf)))
			valid_path_found = ft_strdup(cd_path[i]);
		else if ((tmp = ft_strjoin("./", cd_path[i]))
			&& !(stat(tmp, buf)))
			valid_path_found = ft_strdup(tmp);
		tmp = free_and_null_str(&tmp);
		i++;
	}
	return (valid_path_found);
}

void	get_curpath_with_cd_path(char **curpath, char *env[])
{
	char		*cd_path_value;
	char		**cd_path_tab;
	char		*tmp;
	struct stat	*buf;

	if (!(cd_path_value = get_var_value("$CDPATH", env)))
		return ;
	cd_path_tab = ft_split(cd_path_value, ':');
	cd_path_value = free_and_null_str(&cd_path_value);
	if (cd_path_tab && search_cd_path(cd_path_tab, *curpath, buf))
	{
		tmp = ft_strdup(*curpath);
		*curpath = free_and_null_str(curpath);
		*curpath = search_cd_path(cd_path_tab, tmp, buf);
		tmp = free_and_null_str(&tmp);
	}
	cd_path_tab = free_and_null_tab(&cd_path_tab);
}
  
int		ft_cd(t_cmd *cmd, char **env[])
{
	int		ret;
	char	*curpath;

	curpath = NULL;
	if (!cmd->argv[1] && !(curpath = get_var_value("$HOME", *env)))
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	else if (cmd->argv[1])
		curpath = ft_strdup(cmd->argv[1]);
	if (curpath && curpath[0] && !(curpath[0] == '/' || curpath[0] == '.')
		&& var_is_set(env, "CDPATH"))
		get_curpath_with_cd_path(&curpath, *env);
	
	{
	
	}
	

        7.  If curpath
           does not begin with a <slash> character, set curpath to the
           string formed by the concatenation of the value of PWD, a <slash>
           character if the value of PWD did not end with a <slash>
           character, and curpath.

        8. The curpath value shall then be converted to canonical form as
           follows, considering each component from beginning to end, in
           sequence:

            a. Dot components and any <slash> characters that separate them
               from the next component shall be deleted.

            b. For each dot-dot component, if there is a preceding component
               and it is neither root nor dot-dot, then:

                i.  If the preceding component does not refer (in the
                    context of pathname resolution with symbolic links
                    followed) to a directory, then the cd utility shall
                    display an appropriate error message and no further
                    steps shall be taken.

               ii.  The preceding component, all <slash> characters
                    separating the preceding component from dot-dot, dot-
                    dot, and all <slash> characters separating dot-dot from
                    the following component (if any) shall be deleted.

            c. An implementation may further simplify curpath by removing
               any trailing <slash> characters that are not also leading
               <slash> characters, replacing multiple non-leading
               consecutive <slash> characters with a single <slash>, and
               replacing three or more leading <slash> characters with a
               single <slash>.  If, as a result of this canonicalization,
               the curpath variable is null, no further steps shall be
               taken.

        9. If curpath is longer than {PATH_MAX} bytes (including the
           terminating null) and the directory operand was not longer than
           {PATH_MAX} bytes (including the terminating null), then curpath
           shall be converted from an absolute pathname to an equivalent
           relative pathname if possible. This conversion shall always be
           considered possible if the value of PWD, with a trailing <slash>
           added if it does not already have one, is an initial substring of
           curpath.  Whether or not it is considered possible under other
           circumstances is unspecified. Implementations may also apply this
           conversion if curpath is not longer than {PATH_MAX} bytes or the
           directory operand was longer than {PATH_MAX} bytes.

       10. The cd utility shall then perform actions equivalent to the
           chdir() function called with curpath as the path argument. If
           these actions fail for any reason, the cd utility shall display
           an appropriate error message and the remainder of this step shall
           not be executed. If the −P option is not in effect, the PWD
           environment variable shall be set to the value that curpath had
           on entry to step 9 (i.e., before conversion to a relative
           pathname). If the −P option is in effect, the PWD environment
           variable shall be set to the string that would be output by pwd
           −P.  If there is insufficient permission on the new directory, or
           on any parent of that directory, to determine the current working
           directory, the value of the PWD environment variable is
           unspecified.

       If, during the execution of the above steps, the PWD environment
       variable is set, the OLDPWD environment variable shall also be set to
       the value of the old working directory (that is the current working
       directory immediately prior to the call to cd).


	ret = 0;
	if (!cmd->argv[1] || cmd->argc > 2)
		return (ft_cd_arg_issue(cmd, env));
	if (!(ft_strcmp(cmd->argv[1], "-")))
	{
		cmd->argv[1] = free_and_null_str(&cmd->argv[1]);
		cmd->argv[1] = get_var_value("$OLDPWD", *env);
		ft_printf("%s\n", cmd->argv[1]);// ! Pas du debug ! Comportement normal de shell
	}
	ret = chdir(cmd->argv[1]);
	if (ret && cmd->argv[1])
		return (ft_cd_error_msg(cmd->argv[1]));
	return (ret);
}


int		ft_cd0(t_cmd *cmd, char **env[])
{
	int		ret;

	ret = 0;
	if (!cmd->argv[1] || cmd->argc > 2)
		return (ft_cd_arg_issue(cmd, env));
	if (!(ft_strcmp(cmd->argv[1], "-")))
	{
		cmd->argv[1] = free_and_null_str(&cmd->argv[1]);
		cmd->argv[1] = get_var_value("$OLDPWD", *env);
		ft_printf("%s\n", cmd->argv[1]);// ! Pas du debug ! Comportement normal de shell
	}
	ret = chdir(cmd->argv[1]);
	if (ret && cmd->argv[1])
		return (ft_cd_error_msg(cmd->argv[1]));
	return (ret);
}
