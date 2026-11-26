/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_manip.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:37:38 by nicpinar          #+#    #+#             */
/*   Updated: 2025/02/24 21:01:22 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	join_path(t_token **token, int i, t_parserstate *state, int j)
{
	char	*home;
	char	*tmp;
	char	*new;
	char	*prefix;

	home = ft_strdup("/home/nicpinar");
	if (!home)
		malloc_error("malloc failed join_path 70\n", state, NULL);
	if (j > 0)
		prefix = ft_substr(token[i]->str, 0, j);
	else
		prefix = ft_strdup("");
	if (!prefix)
		malloc_error("malloc failed join path 76\n", state, (void **)&home);
	tmp = ft_strjoin(prefix, home);
	free(prefix);
	if (!tmp)
		malloc_error("malloc failed join_path 80\n", state, (void **)&home);
	new = ft_strjoin(tmp, token[i]->str + j + 1);
	free(tmp);
	free(home);
	free(token[i]->str);
	token[i]->str = new;
	token[i]->size = ft_strlen(new);
}

char	**get_dict(char **envp, int copy)
{
	int		i;
	char	**dict;

	i = 0;
	if (!envp || !envp[0])
		return (NULL);
	while (envp[i])
		i++;
	dict = (char **)malloc(sizeof(char *) * (i + 1));
	if (!dict)
		malloc_error("dict malloc failed\n", NULL, NULL);
	i = 0;
	while (envp[i])
	{
		dict[i] = ft_strdup(envp[i]);
		if (!dict[i])
			malloc_error("dict malloc failed\n", NULL, (void **)dict);
		i++;
	}
	if (copy)
		free_strs(envp);
	dict[i] = NULL;
	return (dict);
}

char	**add_to_array(char **array, char *str)
{
	int		i;
	char	**new;

	i = 0;
	if (!str)
		return (NULL);
	while (array && array[i])
		i++;
	new = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new)
		return (NULL);
	i = 0;
	while (array && array[i])
	{
		new[i] = array[i];
		i++;
	}
	new[i] = ft_strdup(str);
	if (!new[i])
		return (free_strs(new), NULL);
	if (array)
		free(array);
	new[i + 1] = NULL;
	return (new);
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size,
		t_parserstate *state)
{
	void	*new_ptr;

	new_ptr = NULL;
	if (!ptr)
	{
		new_ptr = malloc(new_size);
		if (new_ptr == NULL)
			malloc_error("Error: realloc 63\n", state, NULL);
		return (new_ptr);
	}
	if (new_size == 0)
		malloc_error("Error: realloc 67\n", state, (void **)&ptr);
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		malloc_error("Error: realloc 70\n", state, (void **)&ptr);
	if (old_size > new_size)
		old_size = new_size;
	if (!ft_memcpy(new_ptr, ptr, old_size))
	{
		free(new_ptr);
		malloc_error("Error: realloc 76\n", state, (void **)&ptr);
	}
	free(ptr);
	return (new_ptr);
}
