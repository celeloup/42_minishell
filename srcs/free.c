/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/03 16:54:07 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

    void
free_arg(t_arg *arg)
{
    int i;

    i = 0;
    while (arg->count && i < arg->count)
    {
        if (arg->value[i] != NULL)
            free(arg->value[i]);
        arg->value[i] = NULL;
        i++;
    }
    if (arg->value != NULL)
        free(arg->value);
    arg->value = NULL;
    arg->count = 0; //Pas sûr que ce soit nécessaire.
    arg->max_arg_size = 0; //idem
}