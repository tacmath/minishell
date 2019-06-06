/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:10:54 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/06 15:45:14 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

#include <signal.h>

void test(int t)
{
	t = 0;
	signal(SIGINT, test);
	write(1, "\n", 1);
}

int main(void)
{
	t_shell	*shell;
	char *line;
	char **av;
	int n;

	signal(SIGINT, test);
	if (!(shell = ft_memalloc(sizeof(t_shell))))
		return (-1);
	shell_init(shell);
	while (1)
	{
		write(1, "Super shell: ", 13);
		get_next_line(0, &line);
		av = get_av(line);
		free(line);
		if (treat_av(av, shell) && av[0] && av[0][0])
			if (run_builtins(av, shell))
				run_non_builtin(av, shell);
		n = -1;
		while (av[++n])
			free(av[n]);
		free(av);
	}
	return (0);
}
