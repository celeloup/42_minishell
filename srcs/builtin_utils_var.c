/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env-var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 13:12:12 by user42            #+#    #+#             */
/*   Updated: 2020/09/25 13:45:02 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* INUTILE SUR LINUX ?? 
	int
check_cmd_name(char *name)
{
    int     i;
    char    *low_name;

    low_name = ft_strdup(name);
    i = 0;
    while (low_name[i])
    {
        low_name[i] = ft_tolower(low_name[i]);
        i++;
    }
    i = 0;
    if (!ft_strcmp(low_name, "echo"))
        i = ECHO;
    else if (!ft_strcmp(low_name, "env"))
        i = ENV;
    else if (!ft_strcmp(low_name, "pwd"))
        i = PWD;
    else if (!ft_strcmp(low_name, "cd"))
        i = CD;
    if (low_name)
        free(low_name);
    low_name = NULL;
    return (i);
}
*/

int		is_not_name(char *input)
{
	int i;

	if (!input || (input[0] && !(ft_isalpha(input[0]) || input[0] == '_')))
		return (-1);
	i = 1;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	if (input[i])
		return (i);
	else
		return (0);
}

/*
** returns 0 if input is not a name/identifier (cf man bash for definition)
** else its size
*/

int		is_name(char *input)
{
	int i;

	if (!input || (input[0] && !(ft_isalpha(input[0]) || input[0] == '_')))
		return (0);
	i = 1;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	if (input[i])
		return (0);
	else
		return (i);
}

/*
** If the var format is valid (second else if), var_is_valid returns '=' index,
** else 0 or -errno;
** The last else if returns 0 for case export 'VAR' with no '='
*/

int		var_is_valid(char *var, char *cmd, int value_expected)
{
	if (is_not_name(var) < 0 || (is_not_name(var) && !value_expected))
		return (print_env_error(var, cmd, INVALID_NAME));
	else if (is_not_name(var) && value_expected && var[is_not_name(var)] != '=')
		return (print_env_error(var, cmd, INVALID_NAME));
	else if (is_not_name(var) && value_expected && var[is_not_name(var)] == '=')
		return (is_not_name(var));
	else if (is_name(var) && value_expected)
		return (0);
	else
		return (is_name(var));
}