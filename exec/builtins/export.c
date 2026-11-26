/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 03:20:52 by nbelouar42        #+#    #+#             */
/*   Updated: 2025/03/04 18:21:02 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_env_index(char **env, char *key)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && (env[i][key_len] == '='
			|| !env[i][key_len]))
			return (i);
		i++;
	}
	return (-1);
}

char	**realloc_env(char **env, int len)
{
	int		i;
	char	**res;

	i = -1;
	res = malloc(sizeof(char *) * (len + 1));
	res[len] = NULL;
	if (!res)
		return (NULL);
	while (env[++i] && i < len)
	{
		res[i] = ft_strdup(env[i]);
		free(env[i]);
	}
	free(env);
	return (res);
}

char	**set_env_var(char **env, char *key, char *value)
{
	int		i;
	char	*new_value;

	i = 0;
	i = get_env_index(env, key);
	if (!value)
		value = "";
	new_value = ft_strjoin_bis("=", value);
	if (i != -1 && env[i])
	{
		free(env[i]);
		env[i] = ft_strjoin_bis(key, new_value);
		free(new_value);
	}
	else
	{
		i = ft_strs_len(env);
		env = realloc_env(env, i + 1);
		if (!env)
			return (NULL);
		env[i] = ft_strjoin_bis(key, new_value);
		free(new_value);
	}
	return (env);
}

char	**get_key_and_value(char *str)
{
	char	*pos;
	char	**tmp;

	tmp = malloc(sizeof(char *) * 3);
	if (!tmp)
		return (NULL);
	pos = ft_strchr(str, '=');
	if (pos == NULL)
	{
		tmp[0] = ft_strdup(str);
		tmp[1] = NULL;
		return (tmp);
	}
	tmp[0] = ft_substr(str, 0, pos - str);
	tmp[1] = ft_substr(pos, 1, ft_strlen(pos));
	tmp[2] = NULL;
	return (tmp);
}

int	ft_export(t_shell *shell_env, char **token)
{
	int		i;
	char	**tmp;

	if (!token[1])
		return (ft_env(shell_env->envp, NULL, 1));
	i = 0;
	while (token[++i])
	{
		if (!is_valid_key(token[i]))
		{
			ft_error("export: `");
			ft_error(token[i]);
			ft_error("': not a valid identifier\n");
			continue ;
		}
		tmp = get_key_and_value(token[i]);
		if (!tmp)
			return (1);
		if (tmp[1])
			shell_env->envp = set_env_var(shell_env->envp, tmp[0], tmp[1]);
		free_tab(tmp);
	}
	return (0);
}
