/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_binpath.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:09:47 by kyra              #+#    #+#             */
/*   Updated: 2025/03/07 21:27:30 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	verify_binpath(char *bin_path, t_sections *curr, char **path_split)
{
	if (access(bin_path, X_OK) == 0)
	{
		curr->path_bin = bin_path;
		free_strs(path_split);
		return (1);
	}
	free(bin_path);
	return (0);
}

static char	*extract_path(char **dict)
{
	int		i;
	char	*path;

	i = 0;
	if (!dict)
		return (NULL);
	path = NULL;
	if (!dict)
		return (NULL);
	while (dict[i])
	{
		if (ft_strnstr(dict[i], "PATH=", 5))
		{
			path = ft_strchr(dict[i], '=');
			path++;
			return (path);
		}
		i++;
	}
	return (NULL);
}

static void	extract_binpath(t_sections **sections, t_sections *curr,
		char **dict)
{
	int		i;
	char	*path;
	char	*bin_path;
	char	**path_split;

	i = -1;
	path = extract_path(dict);
	if (!path)
		return ;
	path_split = ft_split(path, ':');
	if (!path_split)
		malloc_error2("path_split malloc failed\n", NULL, sections, dict);
	while (path_split[++i])
	{
		path = ft_strjoin(path_split[i], "/");
		if (!path)
			malloc_error2("path malloc failed\n", path_split, sections, dict);
		bin_path = ft_strjoin(path, curr->args[0]);
		free(path);
		if (!bin_path)
			malloc_error2("path malloc failed\n", path_split, sections, dict);
		if (verify_binpath(bin_path, curr, path_split))
			return ;
	}
	free_strs(path_split);
}

void	check_absolute(t_sections **sections, t_sections *curr, char **dict)
{
	int			i;
	int			len;
	char		*new_arg;

	if (curr->args[0] == NULL)
		return ;
	len = ft_strlen(curr->args[0]);
	i = len - 1;
	if (curr->path_bin == NULL)
		curr->path_bin = ft_strdup(curr->args[0]);
	if (curr->args[0][i] == '/')
		return ;
	while (curr->args[0][i] != '/' && i != 0)
		i--;
	if (i == 0 && curr->args[0][i] != '/')
		return ;
	new_arg = (char *)malloc(sizeof(char) * (len - i + 1));
	if (!new_arg || !curr->path_bin)
		malloc_error2("malloc error\n", &new_arg, sections, dict);
	len = 0;
	i++;
	while (curr->args[0][i])
		new_arg[len++] = curr->args[0][i++];
	new_arg[len] = '\0';
	free_and_reassign(curr, new_arg);
}

void	define_binpath(t_sections **sections, char **dict)
{
	t_sections	*current;

	current = *sections;
	while (current)
	{
		if (current->args && is_only_whitespaces(current->args[0]) == 0)
		{
			if (is_builtin(current->args[0]))
			{
				current->path_bin = ft_strdup(current->args[0]);
				if (!current->path_bin)
					malloc_error2("malloc failed\n", NULL, sections, dict);
			}
			else
			{
				extract_binpath(sections, current, dict);
				check_absolute(sections, current, dict);
				if (!verify_binpath(current->path_bin, current, NULL))
					current->path_bin = NULL;
			}
		}
		else
			current->path_bin = NULL;
		current = current->next;
	}
}
