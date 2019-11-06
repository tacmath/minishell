/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   minishell.h                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/11 11:01:59 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/11/06 17:18:22 by mtaquet     ###    #+. /#+    ###.fr     */
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

struct					s_file
{
	char				*name;
	int					type;
};

typedef struct s_file	t_file;

struct					s_comp
{
	t_file				*list;
	int					start;
};

typedef struct s_comp	t_comp;

struct					s_shell
{
	char				*home;
	char				*last_dir;
	char				**shell_env;
	char				*mem[MAX_MEM + 1];
	int					mem_nb;
	char				*pre_cursor;
	char				*after_cursor;
	t_comp				*comp;
	char				status;
	int					nb_co;
	int					nb_li;
	struct termios		old_term;
	struct termios		shell_term;
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
char					*get_line(t_shell *shell);
int						get_strlen(char *str);
t_shell					*get_shell(t_shell *shell);
void					remove_one_char(t_shell *shell);
int						go_to_right(t_shell *shell);
int						go_to_left(t_shell *shell);
int						add_to_mem(char *line);
int						next_mem(t_shell *shell);
int						prev_mem(t_shell *shell);
int						oputchar(int c);
char					**get_all_path(char **env);
int						get_all_from_path(t_file **list,
						char *path, char *start);
int						get_all_command(t_shell *shell, char *command);
int						get_all_command_and_dir_from_path(t_file **list,
						char *path, char *command);
int						get_all_command_from_path(t_file **list,
						char *path, char *command);
int						add_to_list(t_file **list, t_file new);
int						get_list(t_shell *shell);
long int				auto_comp(t_shell *shell);
char					is_command(char *line);
int						get_line_path(char *line, char **path,
						char *type, int *start);
void					move_cursor(t_shell *shell, int start,
						int end, char mode);
void					print_choice(t_shell *shell);
int						free_comp(t_comp *comp);
void					print_new_prompt(t_shell *shell);
void					clear_line(t_shell *shell, int start, int end);
void					remove_last_name(char *str);
void					print_file_error(char *error, char *file);

#endif
