/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeier <mmeier@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:39:05 by mariusmeier       #+#    #+#             */
/*   Updated: 2024/08/13 14:09:36 by mmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Returns 0 in case another redirect out '>' or '>>' operator has been found in
  redir array, otherwise returns 1.*/
int	no_other_redout(t_data *data)
{
	int	i;
	int	j;

	i = data->i;
	j = data->j;
	i++;
	while (data->proc[j].redir[i])
	{
		if (ft_strncmp(data->proc[j].redir[i], ">", 1) == 0)
			return (0);
		i++;
	}
	return (1);
}

/*Returns 0 in case another redirect in '<' operator has been found in
  redir array, otherwise returns 1.*/
int	no_other_redin(t_data *data)
{
	int	i;
	int	j;

	i = data->i;
	j = data->j;
	i++;
	while (data->proc[j].redir[i])
	{
		if (ft_strncmp(data->proc[j].redir[i], "<", 1) == 0
			&& !data->proc[j].redir[i][1])
			return (0);
		i++;
	}
	return (1);
}

/*Checks if there is any other heredoc in a process. If yes,
  returns 0.*/
int	no_other_heredoc(t_data *data)
{
	int	l;
	int	j;

	l = data->l;
	j = data->j;
	l++;
	while (data->proc[j].redir[l])
	{
		if (ft_strncmp(data->proc[j].redir[l], "<<", 2) == 0)
			return (0);
		l++;
	}
	return (1);
}