/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeier <mmeier@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:24:39 by mmeier            #+#    #+#             */
/*   Updated: 2024/06/04 15:18:44 by mmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

/*Frees relevant parts of the main struct. Last if-statement still
  to be investigated if sufficient.*/
void	free_all(t_data *data)
{
	int	j;
	int	i;

	j = 0;
	i = 0;
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	if (data->tokens && data->tokens[j])
	{
		while (data->tokens[j])
		{
			free(data->tokens[j]);
			data->tokens[j] = NULL;
			j++;
		}
		free(data->tokens);
		data->tokens = NULL;
	}
	if (data->prcs && data->prcs[i])
	{
		while (data->prcs[i])
		{
			free(data->prcs[i]);
			data->prcs[i] = NULL;
			i++;
		}
		free(data->prcs);
		data->prcs = NULL;
	}
	if (data->token_list)
	{
		free(data->token_list->entry);
		free(data->token_list);
		data->token_list = NULL;
	}
}

/*The 'readline' function enables writing commands to program during execution. 
  In case 'env' is typed to shell, env. variables are printed to the terminal, 
  otherwise command that user inputs is printed. The 'add_history' function 
  enables visibility of previous inserted commands (using keyboards "arrow up" 
  after a command has been typed)*/
static int	ft_input(t_data *data)
{
	char		*environment;
	//int			i;

	//i = 0;
	environment = "env";
	while (1)
	{
		setup_signal();
		data->input = readline("minishell> ");
		if (!data->input)
		{
			printf("EOF or ERROR");
			return (1);
		}
		if (ft_strlen(data->input) > 0)
			add_history(data->input);
		if (not_valid_input(data->input))
		{
			free_all(data);
			continue;
		}
		if (ft_strncmp(data->input, environment, 3) == 0)
			print_env(data->temp_env);
		else
			printf("You entered %s\n", data->input);
		if (split_in_prcs(data))
			return (1);
		data->tokens = ft_tokenize(data->input, ' ', '"');
		if (!data->tokens)
			return (1);
		if (!ft_malloc_token(data))
			return (1);
		// while (data->tokens[i])
		// {
		// 	data->token_list->entry[i].cnt = data->tokens[i];
		// 	ft_token_type(data, i);
		// 	printf("%s\n", data->token_list->entry[i].cnt);
		// 	printf("%d\n", data->token_list->entry[i].type);
		// 	i++;
		// }
		//i = 0;
		//parse_cmds(data);
		free_all(data);
	}
	return (0);
}

/*Creates copy of env var in order to being able of modifying this copy
  later on in the course of the shell execution*/
static char	**ft_copy_env(char **env, char **cpy_env)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (env[j])
		j++;
	cpy_env = (char **) malloc (sizeof(char *) * (j + 1));
	if (!cpy_env)
		return (NULL);
	while (env[i])
	{
		cpy_env[i] = ft_substr(env[i], 0, ft_strlen(env[i]));
		if (!cpy_env[i])
			return (free_arr_rev(cpy_env, i));
		i++;
	}
	return (cpy_env);
}

void	init_data(t_data *data)
{
	data->temp_env = NULL;
	data->tokens = NULL;
	data->input = NULL;
	data->pipe_count = 0;
	data->prcs_buf = NULL;
	data->prcs = NULL;
	data->token_list = NULL;
	data->cmds = NULL;
}

/*String array 'env' holds by default environment variables of the system. 
  Array is copied with ft_copy_env so that any alterations done to the
  env variables during minishell execution do not affect the original env.*/
int	main(int ac, char *av[], char *env[])
{
	t_data	data;	

	init_data(&data);
	data.temp_env = ft_copy_env(env, data.temp_env);
	if (ac > 1 || av[1])
	{
		printf("Error. File does not take input.\n");
		return (1);
	}
	else
		ft_input(&data);
	return (0);
}
