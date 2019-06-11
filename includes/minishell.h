/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   minishell.h                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/11 11:01:59 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/11 11:44:53 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include "libft.h"
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>

struct					s_shell
{
	char				*home;
	char				*last_dir;
	char				**shell_env;
};

typedef struct s_shell	t_shell;
int						run_non_builtin(char **av, t_shell *shell);
int						change_env(char **env, char *var, char *new);
int						ft_cd(char **av, t_shell *shell);
int						ft_setenv(char **av, t_shell *shell);
int						ft_unsetenv(char **av, t_shell *shell);
int						run_builtins(char **av, t_shell *shell);
void					free_shell(t_shell *shell);
void					free_av(char **av);
int						shell_init(t_shell *shell, char **env, char *name);
char					**get_av(char *line);
int						treat_av(char **av, t_shell *shell);

#endif
