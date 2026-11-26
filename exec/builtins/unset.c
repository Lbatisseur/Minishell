/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 03:21:24 by nbelouar42        #+#    #+#             */
/*   Updated: 2025/02/24 21:41:52 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**remove_env(char **env, int pos)
{
	int		len;
	int		i;
	char	**new_env;

	len = ft_strs_len(env);
	new_env = malloc(sizeof(char *) * len);
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < pos)
	{
		new_env[i] = env[i];
		i++;
	}
	while (env[i + 1])
	{
		new_env[i] = env[i + 1];
		i++;
	}
	new_env[i] = NULL;
	free(env[pos]);
	free(env);
	return (new_env);
}

int	ft_unset(t_shell *shell_env)
{
	int			i;
	int			pos;
	t_sections	*shell;

	shell = shell_env->sections;
	if (!shell->args || !shell->args[1])
		return (0);
	i = 1;
	while (shell->args[i])
	{
		pos = get_env_index(shell_env->envp, shell->args[i]);
		if (pos != -1)
			shell_env->envp = remove_env(shell_env->envp, pos);
		i++;
	}
	return (0);
}
