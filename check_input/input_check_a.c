/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check_a.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeier <mmeier@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 15:45:02 by mmeier            #+#    #+#             */
/*   Updated: 2024/09/16 14:05:04 by mmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Checks if only spaces are present in input and returns in
  this case 1.*/
static int	check_spaces(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

/*Checks for errors related to red_out operant
  with different constellations of operants, 
  including red_out_app operant cases.*/
static int	red_out_check(char *input, int *i)
{
	(*i)++;
	if (!input[*i] || input[*i] == '|')
		return (write_sytx_error("newline", 0));
	if (input[*i] == '>')
		return (check_red_out_app(input, *i));
	if (input[*i] == '<')
		return (write_sytx_error(NULL, '<'));
	while (input[*i] && input[*i] == 32)
		(*i)++;
	if (!input[*i])
		return (write_sytx_error("newline", 0));
	if (input[*i] == '|')
		return (write_sytx_error(NULL, '|'));
	if (input[*i] == '>')
		return (write_sytx_error(NULL, '>'));
	return (0);
}

/*Checks for errors related to red_in operant
  with different constellations of operants, 
  including heredoc cases.*/
static int	red_in_check(char *input, int *i)
{
	(*i)++;
	if (!input[*i] || input[*i] == '|')
		return (write_sytx_error("newline", 0));
	if (input[*i] == '<')
		return (check_heredoc_err(input, *i));
	if (input[*i] == '>')
		return (write_sytx_error("newline", 0));
	while (input[*i] && input[*i] == 32)
		(*i)++;
	if (!input[*i])
		return (write_sytx_error("newline", 0));
	if (input[*i] == '|')
		return (write_sytx_error(NULL, '|'));
	if (input[*i] == '<')
		return (write_sytx_error(NULL, '<'));
	return (0);
}

/*Checks several syntax errors related to all redirect operants.*/
int	check_redirects(char *input)
{
	int	i;

	i = 0;
	if (!input && !input[i])
		return (0);
	while (input[i])
	{
		if ((input[i] == '>' && !between_quotes(input, i)))
		{
			if (red_out_check(input, &i))
				return (1);
		}
		else if (input[i] == '<' && !between_quotes(input, i))
		{
			if (red_in_check(input, &i))
				return (1);
		}
		i++;
	}
	return (0);
}

/*Checks for any syntax errors within the user input.*/
int	not_valid_input(char *input, t_data *data)
{
	if (check_spaces(input))
	{
		data->exit_status = 2;
		return (1);
	}
	if (check_quotes(input))
	{
		data->exit_status = 2;
		return (1);
	}
	if (check_pipes(input))
	{
		data->exit_status = 2;
		return (1);
	}
	if (check_redirects(input))
	{
		data->exit_status = 2;
		return (1);
	}
	return (0);
}
