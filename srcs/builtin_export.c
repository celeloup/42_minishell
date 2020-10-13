/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 13:00:13 by user42            #+#    #+#             */
/*   Updated: 2020/10/13 22:08:07 by user42           ###   ########.fr       */
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
//	ft_printf("\ntocheck =%s", to_check);
	tmp = free_and_null_str(&tmp);
	tmp = get_var_value(to_check, *env);
	to_check = free_and_null_str(&to_check);
	if (!tmp)
		ret = 0;
	else
		ret = 1;
	tmp = free_and_null_str(&tmp);
	return (ret);
}

int		edit_var(char **env[], char *cmd, char *var)
{
	int	name_len;
	int	i;

	if (var_is_valid(var, NULL, YES) > 0)
		name_len = var_is_valid(var, NULL, YES);
	else
		return (-var_is_valid(var, cmd, YES));
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

	if (var && (!ft_strcmp(var, "_") || !ft_strncmp(var, "_=", 2)))
		return (0);
	if (var_is_valid(var, NULL, YES) <= 0 && is_not_name(var))
		return (-(var_is_valid(var, cmd, YES)));
//	ft_printf("\nadd_var inside");//debug
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
		*env = free_and_null_tab(env);
	*env = env_dup(new_env);
	new_env = free_and_null_tab(&new_env);
	return (EXIT_SUCCESS);
}

int		ft_export(t_cmd *cmd, char **env[])
{
	int		i;
	int		ret;

	i = 1;
	if (!cmd->argv[1])
		sort_env(*env);
	ret = 0;
	while (cmd->argv[i])
	{
		if (add_var(env, cmd->argv[0], cmd->argv[i]) != 0)
			ret = 1;
		i++;
	}
//	ft_printf("\nexport ret =%d", ret);//debug
	return (ret);
}
