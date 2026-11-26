/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 03:19:29 by nbelouar42        #+#    #+#             */
/*   Updated: 2025/02/24 21:12:23 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_is_dir(char *path)
{
	struct stat	path_stat;

	if (ft_strncmp(path, "./minishell", 11) == 0)
		return (0);
	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
			return (1);
		else
			return (ft_error("minishell: cd: "), ft_error(path),
				ft_error(": Not a directory\n"));
	}
	else
		return (ft_error("minishell: cd: "), ft_error(path),
			ft_error(": No such file or directory\n"));
}

static int	change_dir(t_shell *shell_env, char *path)
{
	t_sections	*shell;
	char		tab[4096];

	shell = shell_env->sections;
	if (chdir(path) == -1)
		return (ft_error(strerror(errno)));
	if (shell->path)
	{
		free(shell->path);
	}
	shell->path = getcwd(tab, 4096);
	if (!shell->path)
		return (ft_error("bash: cd: could not retrieve current directory\n"));
	shell->path = ft_strdup(shell->path);
	if (!shell->path)
		return (ft_error("bash: cd: "), ft_error(strerror(errno)),
			ft_error("\n"));
	shell_env->envp = set_env_var(shell_env->envp, "OLDPWD",
			get_env_value(shell_env, "PWD", 0));
	shell_env->envp = set_env_var(shell_env->envp, "PWD", shell->path);
	return (0);
}

int	ft_cd(t_shell *shell_env)
{
	char		*path;
	t_sections	*shell;

	shell = shell_env->sections;
	path = NULL;
	if (shell->args[1] && shell->args[2])
	{
		printf("shell->args[1] = %s\n", shell->args[1]);
		return (ft_error("bash: cd: too many argumentts\n"));
	}
	if (!shell->args[1] || !ft_strncmp(shell->args[1], "-", 2)
		|| !ft_strncmp(shell->args[1], "--", 3) || !ft_strncmp(shell->args[1],
			"~", 2))
	{
		path = get_env_value(shell_env, "HOME", 0);
		if (!path)
			return (ft_error("bash: cd: HOME not set\n"));
	}
	else if (shell->args[1])
		path = shell->args[1];
	if (ft_is_dir(path))
		return (change_dir(shell_env, path));
	else
		return (-1);
}
