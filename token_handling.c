/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeier <mmeier@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 11:36:01 by mmeier            #+#    #+#             */
/*   Updated: 2024/07/23 16:40:44 by mmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Sets all token values to -1 in order to avoid garbage values when assigning
  these tokens to 2d array in ft_token_type function*/
static void	init_token_type(t_data *data, int num_tokens)
{
	int	i;

	i = 0;
	while (num_tokens > 0)
	{
		data->token_list[i].type = INIT_VAL;
		num_tokens--;
		i++;
	}
}

/*Allocates memory for t_token structs. Calculates first amount of 
  tokens available which is relevant for allocating right amount of t_token 
  structs.*/

int	ft_malloc_token(t_data *data)
{
	int	num_tokens;

	num_tokens = 0;
	while (data->tokens[num_tokens])
		num_tokens++;
	data->token_list = (t_token *) malloc(num_tokens * sizeof(t_token));
	if (!data->token_list)
		return (0);
	init_token_type(data, num_tokens);
	return (1);
}

/*Assigns type (e.g. 'pipe', 'command' etc.) to each identified token.*/
void	ft_token_type(t_data *data, int i)
{
	if (data->token_list[i].type < COMMAND || data->token_list[i].type > REMOVE)
		data->token_list[i].type = COMMAND;
	if (ft_strncmp(data->tokens[i], "<<", 2) == 0)
	{
		data->token_list[i].type = REMOVE;
		data->token_list[i + 1].type = HEREDOC;
	}
	else if (ft_strncmp(data->tokens[i], ">>", 2) == 0)
	{
		data->token_list[i].type = REMOVE;
		data->token_list[i + 1].type = REDIRECT_OUT_APP;
	}
	else if (ft_strncmp(data->tokens[i], "<", 1) == 0)
	{
		data->token_list[i].type = REMOVE;
		data->token_list[i + 1].type = REDIRECT_IN;
	}
	else if (ft_strncmp(data->tokens[i], ">", 1) == 0)
	{
		data->token_list[i].type = REMOVE;
		data->token_list[i + 1].type = REDIRECT_OUT;
	}
	else if (data->tokens[i][0] == '|'
		&& ft_strlen(data->tokens[i]) == 1)
		data->token_list[i].type = PIPE;
	else if (i > 0 && ((data->token_list[i - 1].type == REDIRECT_IN)
		|| (data->token_list[i - 1].type == REDIRECT_OUT)))
		data->token_list[i].type = COMMAND;
	else if (i > 0 && data->token_list[i - 1].type == PIPE)
		data->token_list[i].type = COMMAND;
	else if (data->tokens[i][0] == '"' || data->tokens[i][0] == 39)
		data->token_list[i].type = COMMAND;
	else if (i > 0 && data->token_list[i - 1].type == COMMAND)
		data->token_list[i].type = COMMAND;
}

static int	count_cmd(t_data *data)
{
	int	i;
	int	cmd;

	i = 0;
	data->count_cmd = 0;
	data->count_other = 0;
	cmd = 0;
	while (data->token_list[i].type != PIPE && data->tokens[i])
	{
		if (data->token_list[i].type == COMMAND)
			data->count_cmd++;
		if (data->token_list[i].type > COMMAND && data->token_list[i].type < REMOVE)
			data->count_other++;
		i++;
	}
	if (data->count_cmd > 0)
		cmd = 1;
	if (cmd + data->count_other > 0)
		data->execs = (t_exec *) malloc ((data->count_other + cmd) * sizeof(t_exec));
	if (!data->execs)
		return (1);
	printf("AMOUNT OTHER: %d\n", data->count_other);
	printf("AMOUNT COMMAND ARRAY: %d\n", cmd);
	return (0);
}

int	fill_other_structs(t_data *data)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 1;
	if (data->count_other > 0)
	{
		while (data->token_list[i].type != PIPE && data->tokens[i])
		{
			if (data->token_list[i].type > COMMAND && data->token_list[i].type < REMOVE)
			{
				data->execs[k].arr = malloc (2 * sizeof(char *));
				if (!data->execs[k].arr)
					return (1);
				data->execs[k].arr[j] = ft_strdup(data->tokens[i]);
				data->execs[k].arr[j + 1] = 0;
				data->execs[k].type = data->token_list[i].type;
				printf("OTHER STRUCT: %s\n", data->execs[k].arr[j]);
				printf("OTHER STRUCT TYPE: %d\n", data->execs[k].type);
				k++;
			}
			i++;
		}
	}
	return (0);
}

static int	fill_cmd_struct(t_data *data)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	if (data->count_cmd > 0)
		data->execs[k].arr = malloc ((data->count_cmd + 1) * sizeof(char *));
	if (!data->execs[k].arr)
		return (1);
	while (data->token_list[i].type != PIPE && data->tokens[i])
	{
		if (data->token_list[i].type == COMMAND)
		{
			data->execs[k].arr[j] = ft_strdup(data->tokens[i]);
			j++;
		}
		i++;
	}
	data->execs[k].arr[j] = 0;
	data->execs[k].type = COMMAND;
	// printf("COMMAND ARRAY IS:\n");
	// j = 0;
	// while(data->execs[k].arr[j])
	// {
	// 	printf("%s\n", data->execs[k].arr[j]);
	// 	j++;
	// }
	// printf("ARRAY Type IS:\n");
	// printf("%d\n", data->execs[k].type);
	return (0);
}

int	fill_exec_structs(t_data *data)
{
	if (count_cmd(data))
		return (1);
	if (fill_cmd_struct(data))
		return (1);
	if (fill_other_structs(data))
		return (1);
	return (0);
}
