/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   key_event.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 15:06:43 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/10 14:19:39 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

void	remove_one_char(t_shell *shell)
{
	int len;
	int tmp_len;

	len = ft_strlen(shell->pre_cursor);
	if (len)
	{
		tmp_len = get_strlen(shell->pre_cursor);
		shell->pre_cursor[len - 1] = 0;
		move_cursor(shell, tmp_len, get_strlen(shell->pre_cursor));
		tputs(tgetstr("cd", 0), 1, oputchar);
		write(1, shell->after_cursor, ft_strlen(shell->after_cursor));
		move_cursor(shell, get_strlen(shell->pre_cursor) + get_strlen(shell->after_cursor) - get_strlen(""), get_strlen(shell->pre_cursor));
	}
}

int		go_to_right(t_shell *shell)
{
	int		len;
	char	*tmp;

	len = ft_strlen(shell->after_cursor);
	if (len)
	{
		len = ft_strlen(shell->pre_cursor);
		if (!(tmp = malloc(len + 2)))
			return (ft_super_free(2, shell->pre_cursor, shell->after_cursor));
		ft_strcpy(tmp, shell->pre_cursor);
		tmp[len] = shell->after_cursor[0];
		tmp[len + 1] = 0;
		move_cursor(shell, get_strlen(shell->pre_cursor), get_strlen(tmp));
		free(shell->pre_cursor);
		shell->pre_cursor = tmp;
		ft_strcpy(shell->after_cursor, &(shell->after_cursor)[1]);
	}
	return (1);
}

int		go_to_left(t_shell *shell)
{
	int		len1;
	int		len2;
	char	*tmp;

	len1 = ft_strlen(shell->pre_cursor);
	if (len1)
	{
		len2 = ft_strlen(shell->after_cursor);
		if (!(tmp = malloc(len2 + 2)))
			return (ft_super_free(2, shell->pre_cursor, shell->after_cursor));
		ft_strcpy(&tmp[1], shell->after_cursor);
		tmp[0] = shell->pre_cursor[len1 - 1];
		len2 = get_strlen(shell->pre_cursor);
		shell->pre_cursor[len1 - 1] = 0;
		move_cursor(shell, len2, get_strlen(shell->pre_cursor));
		free(shell->after_cursor);
		shell->after_cursor = tmp;
	}
	return (1);
}
