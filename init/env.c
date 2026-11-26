/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 20:45:04 by nicpinar          #+#    #+#             */
/*   Updated: 2025/02/24 21:46:30 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	increment_shlvl(t_shell *shell)
{
	char	**tmp;
	int		lvl;
	int		pos;

	pos = get_env_index(shell->envp, "SHLVL");
	if (pos == -1)
		shell->envp = set_env_var(shell->envp, "SHLVL=", "2");
	else
	{
		tmp = get_key_and_value(shell->envp[pos]);
		lvl = ft_atoi(tmp[1]);
		lvl++;
		free(tmp[1]);
		tmp[1] = ft_itoa(lvl);
		shell->envp = set_env_var(shell->envp, tmp[0], tmp[1]);
		free_tab(tmp);
	}
}

t_shell	init_env(char **envp)
{
	t_shell	shell;
	char	*pwd;

	shell.envp = get_dict(envp, 0);
	shell.sections = NULL;
	shell.pid = 0;
	if (!shell.envp)
	{
		shell.envp = (char **)malloc(sizeof(char *) * 4);
		if (!shell.envp)
			malloc_error("malloc error", NULL, NULL);
		pwd = getcwd(NULL, 0);
		shell.envp[0] = ft_strjoin("PWD=", pwd);
		free(pwd);
		shell.envp[1] = ft_strdup("SHLVL=1");
		shell.envp[2] = ft_strjoin("PATH=",
				"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
		shell.envp[3] = NULL;
		if (!shell.envp[0] || !shell.envp[1] || !shell.envp[2])
		{
			free_strs(envp);
			malloc_error("malloc error main.c 88\n", NULL, NULL);
		}
	}
	return (shell);
}
