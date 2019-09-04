/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   minishell.h                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/11 11:01:59 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/04 13:14:17 by mtaquet     ###    #+. /#+    ###.fr     */
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
# include <sys/ioctl.h>
# include <dirent.h>
# include <termios.h>
# include <termcap.h>

# define PROMPT "Super Shell: "
# define MAX_MEM 100
# define K_UP 4283163
# define K_DOWN 4348699
# define K_RIGHT 4414235
# define K_LEFT 4479771
# define K_RETURN 10
# define K_BACKSPACE 127
# define K_TAB		'\t'

struct					s_shell
{
	char				*home;
	char				*last_dir;
	char				**shell_env;
	char				*mem[MAX_MEM + 1];
	char				status;
	int					nb_co;
	int					nb_li;
	struct termios			old_term;
	struct termios			shell_term;
};

typedef struct s_shell	t_shell;
int						run_non_builtin(char **av, t_shell *shell);
int						change_env(char **env, char *var, char *new);
int						ft_cd(char **av, t_shell *shell);
int						ft_setenv(char **av, t_shell *shell);
int						run_builtins(char **av, t_shell *shell);
void					free_shell(t_shell *shell);
int						free_av(char **av);
int						shell_init(t_shell *shell, char **env, char *name);
char					**get_av(char *line);
int						treat_av(char **av, t_shell *shell);
char                    *get_line(void);
int                     get_strlen(char *str);
t_shell        *get_shell(t_shell *shell);
void    remove_one_char(char *line1, char *line2);
int go_to_right(char **line1, char **line2);
int go_to_left(char **line1, char **line2);
int    add_to_mem(char *line);
int     next_mem(char **line1, char **line2, int *mem);
int     prev_mem(char **line1, char **line2, int *mem);
int oputchar(int c);
char **get_all_path(char **env);

#endif
