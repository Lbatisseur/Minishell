/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 03:19:49 by nbelouar42        #+#    #+#             */
/*   Updated: 2025/02/24 21:41:47 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(t_shell *shell_env, char *str, int malloc)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strs_len(shell_env->envp);
	while (i < len)
	{
		if (shell_env->envp[i][ft_strlen(str)] == '='
			&& ft_strncmp(shell_env->envp[i], str, ft_strlen(str)) == 0)
		{
			len = ft_strlen(str);
			if (malloc == 1)
				free(str);
			return (shell_env->envp[i] + len + 1);
		}
		i++;
	}
	if (malloc == 1)
		free(str);
	return (NULL);
}

int	ft_env(char **env, char **token, int export_mode)
{
	int	i;
	int	len;

	i = 0;
	if (token != NULL && token[1])
		return (ft_error("env: too many arguments\n"));
	if (!env)
		return (ft_error("env: environment not set\n"));
	len = ft_strs_len(env);
	while (env[i])
	{
		if (export_mode)
			ft_putstr_fd("export ", STDOUT_FILENO);
		if (ft_strncmp(env[i], "PATH=", 5) != 0 || len > 10)
			ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
