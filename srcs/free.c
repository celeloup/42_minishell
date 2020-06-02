/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 12:22:22 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

    void
free_arg(t_arg *arg)
{
    int i;

    i = 0;
    while (arg->c && i < arg->c)
    {
        if (arg->v[i] != NULL)
            free(arg->v[i]);
        arg->v[i] = NULL;
        i++;
    }
    if (arg->v != NULL)
        free(arg->v);
    arg->v = NULL;
    arg->c = 0; //Pas sûr que ce soit nécessaire.
    arg->max_arg_size = 0; //idem
}