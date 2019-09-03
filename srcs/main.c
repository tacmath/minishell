/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:10:54 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/02 17:02:45 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

#include <signal.h>

int        treat_line(char *line, t_shell *shell)
{
    char    **av;
    int        n;
    int        m;
    
    n = 0;
    m = 0;
    while (line[n++])
        if (!line[n] || line[n] == ';')
        {
            if (line[n] == ';')
            {
                line[n] = 0;
                if (!(av = get_av(&line[m])))
                    return (0);
                m = ++n;
            }
            else if (!(av = get_av(&line[m])))
                return (0);
            if (treat_av(av, shell) && av[0] && av[0][0])
                if (run_builtins(av, shell))
                    run_non_builtin(av, shell);
            free_av(av);
        }
    free(line);
    return (1);
}

void	ft_sigint(int sig)
{
	t_shell	*shell = get_shell(0);
	
	(void)sig;
	write(1, "\n", 1);
	if (!shell->status || shell->status == 2)
	{
		write(1, PROMPT, ft_strlen(PROMPT));
		shell->status = 2;
	}
}

int		main(int ac, char **av, char **env)
{
	t_shell	*shell;
	char	*line;

	signal(SIGINT, ft_sigint);
	ac = 0;
	if (!(shell = ft_memalloc(sizeof(t_shell))))
		return (-1);
	if (!shell_init(shell, env, av[0]))
	{
			free_shell(shell);
			return (-1);
	}
	get_shell(shell);
	while (1)
	{
		write(1, PROMPT, ft_strlen(PROMPT));
		shell->status = 0;
		if (!(line = get_line()))
		{
			free_shell(shell);
			return (-1);
		}
		shell->status = 1;
		if (!treat_line(line, shell))
		{
			free_shell(shell);
			return (-1);
		}
	}
	return (ac);
}
