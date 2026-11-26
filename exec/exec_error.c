/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:56:07 by nicpinar          #+#    #+#             */
/*   Updated: 2025/03/07 20:57:43 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	usage_error(char *msg, char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
}

static void	ft_exec_error(char *msg, char *str, int code, int prompt)
{
	if (prompt == 1)
		ft_putstr_fd("minishell: ", 2);
	if (is_only_whitespaces(str))
	{
		ft_putstr_fd("Command ", 2);
		ft_putstr_fd("''", 2);
		ft_putstr_fd(" not found\n", 2);
	}
	else
	{
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(msg, 2);
	}
	g_exit_code = code;
	exit(g_exit_code);
}

static void	copy_without_last_slash(char *src, char *dst)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(src) - 1;
	while (src[len] == '/')
		len--;
	len++;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

void	special_cases(t_sections *shell, char *tmp)
{
	int		len;
	char	*cut;

	len = ft_strlen(shell->args[0]) - 1;
	if (is_only_slashes(shell->args[0]) == 1)
		ft_exec_error("Is a directory\n", shell->args[0], 126, 1);
	if (shell->args[0][len] == '/' || shell->args[0][0] == '/')
	{
		cut = (char *)malloc(sizeof(char) * len + 1);
		copy_without_last_slash(shell->args[0], cut);
		if (access(cut, F_OK) == 0)
		{
			if (tmp)
				free(tmp);
			free(cut);
			ft_exec_error("Not a directory\n", shell->args[0], 126, 1);
		}
		if (tmp)
			free(tmp);
		free(cut);
		ft_exec_error("No such file or directory\n", shell->args[0], 127, 1);
	}
	if (tmp == NULL && shell->path_bin == NULL)
		ft_exec_error("No such file or directory\n", shell->args[0], 127, 1);
}

void	check_error_ex(t_sections *shell, char **env)
{
	char	*tmp;

	if (!ft_strncmp(shell->args[0], ".", 2))
	{
		usage_error("filename argument required\n", shell->args[0]);
		ft_exec_error("usage: . filename [arguments]\n", shell->args[0], 2, 0);
	}
	tmp = parse_dict("PATH", env);
	if (!ft_strncmp(shell->args[0], "..", 3) && tmp == NULL)
		ft_exec_error("Is a directory\n", shell->args[0], 127, 1);
	special_cases(shell, tmp);
	free(tmp);
	if (!ft_strncmp(shell->args[0], "..", 3))
		ft_exec_error("command not found\n", shell->args[0], 127, 0);
	if (!shell->path_bin && ft_strchr(shell->args[0], '/') == NULL)
		ft_exec_error("command not found\n", shell->args[0], 127, 0);
	if ((ft_strchr(shell->args[0], '/') != NULL && shell->path_bin))
	{
		if (ft_is_dir(shell->path_bin) == 1)
			ft_exec_error("Is a directory\n", shell->args[0], 126, 1);
		else if (access(shell->path_bin, X_OK) == -1)
			ft_exec_error("Permission denied\n", shell->args[0], 126, 1);
	}
}
