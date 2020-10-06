/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 13:00:13 by user42            #+#    #+#             */
/*   Updated: 2020/10/06 20:14:29 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		var_is_set(char **env[], char *var)
{
	char	*tmp;
	char	*to_check;
	int		ret;

	tmp = ft_strjoin("$", var);
	to_check = get_var_name(tmp);
	if (tmp)
		free(tmp);
	tmp = get_var_value(to_check, *env, NO, NO);
	if (to_check)
		free(to_check);
	to_check = NULL;
	if (!tmp)
		return (0);
	else if (!ft_strcmp(tmp, ""))
		ret = 0;
	else
		ret = 1;
	free(tmp);
	tmp = NULL;
	return (ret);
}

int		edit_var(char **env[], char *cmd, char *var)
{
	int	name_len;
	int	i;

	if (var_is_valid(*env, var, NULL, YES) > 0)
		name_len = var_is_valid(*env, var, NULL, YES);
	else
		return (-var_is_valid(*env, var, cmd, YES));
	i = 0;
	while ((*env)[i])
	{
		if (!strncmp((*env)[i], var, name_len))
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(var);
			return (0);
		}
		i++;
	}
	return (1);
}

/*
** Adds a variable to the environment tab.
** Var must be of format "VAR=value". VAR is lower or upper case or digit or ?
*/

int		add_var(char **env[], char *cmd, char *var)
{
	int		i;
	char	**new_env;

	if (var_is_valid(*env, var, NULL, YES) <= 0)
		return (-(var_is_valid(*env, var, cmd, YES)));
	if (var_is_set(env, var))
		return (edit_var(env, cmd, var));
	i = 0;
	while ((*env)[i])
		i++;
	if ((new_env = (char**)malloc(sizeof(char*) * (i + 2))) == NULL)
		return (EXIT_FAILURE);
	new_env = env_ncpy(new_env, *env, 0, i);
	new_env[i] = ft_strdup(var);
	new_env[i + 1] = NULL;
	if (*env)
		*env = free_env(env);
	*env = env_dup(new_env);
	new_env = free_env(&new_env);
	return (EXIT_SUCCESS);
}

int		ft_export(t_cmd *cmd, char **env[])
{
	int		i;
//	char	**env_cpy;
//	char	*var;
	int		ret;

	i = 1;
//	var = NULL;
//	while (cmd->argv[i] && !(var = get_expanded_token(cmd->argv[i], *env)))
//		i++;
//	if (var)
//		free(var);
	if (!cmd->argv[i])
		return (print_env(*env, EXP));
//	env_cpy = env_dup(*env);
//	var = NULL;
	ret = 0;
	while (cmd->argv[i])
	{
//		if (var)
//			free(var);
//		var = get_expanded_token(cmd->argv[i], env_cpy);
		if (add_var(env, cmd->argv[0], cmd->argv[i]) > 0)
			ret = 1;
		i++;
	}
//	if (var)
//		free(var);
//	var = NULL;
//	env_cpy = free_env(&env_cpy);
	return (ret);
}
