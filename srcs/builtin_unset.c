/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 13:02:20 by user42            #+#    #+#             */
/*   Updated: 2020/10/07 20:49:37 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** remove_var returns 0 if var does is not already in the environment because
** unset does not consider this as an error (and therefore returns 0 too)
*/

int		remove_var(char **env[], char *cmd, char *var, int value_expected)
{
	char	**new_env;
	int		name_len;
	int		i;

	if (!var || (var && !var[0]))
		return (0);
	if ((name_len = var_is_valid(*env, var, NULL, value_expected)) <= 0)
		return (-var_is_valid(*env, var, cmd, value_expected));
	i = 0;
	while ((*env)[i] && (ft_strncmp((*env)[i], var, name_len)
		|| ((*env)[i])[name_len] != '='))
		i++;
	if (!(*env)[i])
		return (EXIT_SUCCESS);
	if ((new_env = (char**)malloc(sizeof(char*) * env_len(*env))) == NULL)
		return (EXIT_FAILURE);
	new_env = env_ncpy(new_env, *env, 0, i);
	new_env = env_ncpy(new_env, &((*env)[i + 1]), i, env_len(*env) - i - 1);
	new_env[env_len(*env) - 1] = NULL;
	*env = free_env(env);
	*env = env_dup(new_env);
	new_env = free_env(&new_env);
	return (EXIT_SUCCESS);
}

/*
** MAN UNSET :
** Unsetting a variable or function that was not previously set shall
**       not be considered an error and does not cause the shell to abort.
** Note that:
**           VARIABLE=
**       is not equivalent to an unset of VARIABLE; in the example, VARIABLE
**       is set to "".
*/

int		ft_unset(t_cmd *cmd, char **env[])
{
	int		i;
	char	**env_cpy;
//	char	*var;
	int		ret;

	i = 1;
//	var = NULL;
	while (cmd->argv[i] && !cmd->argv[i][0])
		i++;
//	if (var)
//		free(var);
	if (!cmd->argv[i])
		return (0);
	env_cpy = env_dup(*env);
//	var = NULL;
	ret = 0;
	while (cmd->argv[i])
	{
//		var = free_and_null(&var);
//		var = get_expanded_token(cmd->argv[i], env_cpy);
		if (remove_var(env, "unset", cmd->argv[i], NO) > 0)
			ret = 1;
		i++;
	}
//	var = free_and_null(&var);
	env_cpy = free_env(&env_cpy);
	return (ret);
}
