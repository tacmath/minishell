/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 14:58:43 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/11/06 17:16:52 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int		oputchar(int c)
{
	return (write(2, &c, 1));
}

int		get_strlen(char *str)
{
	int		n;
	int		len;
	t_shell *shell;
	char	*prompt;

	shell = get_shell(0);
	prompt = PROMPT;
	len = 0;
	n = -1;
	while (prompt[++n])
		if (prompt[n] == '\t')
			len += 8 - len % 8;
		else
			len++;
	n = -1;
	while (str[++n])
		if (str[n] == '\t')
		{
			len += 8 - (len % shell->nb_co) % 8;
			if (len % shell->nb_co < 8)
				len -= len % shell->nb_co;
		}
		else
			len++;
	return (len);
}

void	remove_last_name(char *str)
{
	int n;
	int ret;

	n = -1;
	ret = 0;
	while (str[++n])
		if (str[n] == '/')
			ret = n + 1;
	str[ret] = 0;
}

int		free_av(char **av)
{
	int n;

	n = -1;
	while (av[++n])
		free(av[n]);
	free(av);
	return (0);
}

t_shell	*get_shell(t_shell *shell)
{
	static t_shell *mem;

	if (shell)
		mem = shell;
	return (mem);
}
