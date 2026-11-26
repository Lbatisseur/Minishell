/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 03:16:28 by nbelouar42        #+#    #+#             */
/*   Updated: 2025/02/24 21:13:05 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(void)
{
	char	path[4096];

	if (getcwd(path, 4096) == NULL)
		return (ft_error(strerror(errno)));
	ft_putendl_fd(path, STDOUT_FILENO);
	return (0);
}
